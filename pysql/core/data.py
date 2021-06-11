from collections import namedtuple, OrderedDict, UserList

import datetime
from abc import ABC, abstractmethod
import functools
import logging

from core.util import (
    enumerate_dates,
    get_days_ago,
    get_weekdays_ago,
    last_weekday,
    first_date_of_month,
    time_of,
    past_minutes_bin_of,
    topological_order,
    graph_connection,
    left_join_dataframes,
    to_jsonlike,
    to_set,
    to_list,
)
from core.computation import transform_dataframe


logger = logging.getLogger(__name__)

###################### Acquisition: an interface #############################

Acquisition = namedtuple('Acquisition', ['querygen', 'fetchergen'])


@to_jsonlike.register(Acquisition)
def _(x):
    # TODO add fetchergen and keygen
    return to_jsonlike(x.querygen)


####################### DataFetcher interface and util #######################

class DataFetcher(ABC):
    '''
    provides fetcher function for a given table of type [columns] -> dataframe
    The use case of this class is to provide ways for client code to maintain
    state information between generating different fetcher functions for 
    different table names, which improves efficiency of data fetching
    '''
    @abstractmethod
    def fetch_data(self, env, table_name, columns, by_dt):
        'return a dataframe. by_dt can be date, datetime or a date range'
        pass

    @abstractmethod
    def support(self, table_name):
        'return True or False to indicate if it supports the given table name'
        pass


def make_stub_data_fetcher(class_name, data):
    '''utility to make a simple data fetcher class on the fly.'''

    def _fetch_data_func(self, env, table_name, columns, by_dt):
        return data[table_name][columns].copy()

    def _support_func(self, table_name):
        return table_name in data

    # make a DataFetcher derived class on the fly
    return type(
        class_name, (DataFetcher,),
        {
            'fetch_data': _fetch_data_func,
            'support': _support_func,
        }
    )

####################### expressive data types for query #####################

# mutually recursive data structures to support general queries
DJoin = namedtuple('DJoin', ['source', 'joinby'])

# treat DSelect and DSequence as two polymorphic types for 'source'
DSelect = namedtuple('DSelect', ['table', 'columns', 'transforms'])

# contain a list of DJoin
class DSequence(UserList): pass


@functools.singledispatch
def validate_data_source(x):
    raise RuntimeError('invalid data source:%s' % type(x))


@validate_data_source.register(DSelect)
def _(x):
    table, columns, _ = x
    return {table : to_set(columns)}


@validate_data_source.register(DSequence)
def _(x):
    ret = {}
    for i, (source, joinby) in enumerate(x):
        if i == 0: 
            assert joinby is None
        else:
            assert joinby is not None

        for t, cols in validate_data_source(source).items():
            ret.setdefault(t, set()).update(cols)
    return ret


@functools.singledispatch
def fetch(x, data_fetcher, env, by_dt):
    raise RuntimeError('cannot fetch data via type:%s' % type(x))


@fetch.register(DSelect)
def _(x, data_fetcher, env, by_dt):
    if not data_fetcher.support(x.table):
        raise RuntimeError('fetch func not provided for %s' % x.table)
    
    logger.debug('fetch data on %s %s, %s, %s' % (env, x.table, x.columns, by_dt))
    df = data_fetcher.fetch_data(env, x.table, x.columns, by_dt)
    if not x.transforms:
        return df
    else:
        return transform_dataframe(x.transforms, df)


@fetch.register(DSequence)
def _(x, data_fetcher, env, by_dt):
    for source, joinby in x:
        _data = fetch(source, data_fetcher, env, by_dt)
        logger.debug('fetched:\n%s' % repr(_data))
        #_data.info()
        if joinby is None:
            df = _data
        else:
            logger.debug('before join:\n%s' % repr(df))
            #df.info()
            df = left_join_dataframes(joinby, df, _data, dup_resolve='left')
            logger.debug('after join:\n%s' % repr(df))
            #df.info()
    return df
 

@to_jsonlike.register(DJoin)
def _(x):
    return {
        'struct_type': 'DJoin',
        'source': to_jsonlike(x.source),
        'joinby': to_jsonlike(x.joinby)
    }


@to_jsonlike.register(DSelect)
def _(x):
    return {
        'query_type': 'DSelect',
        'table': x.table,
        'columns': to_jsonlike(x.columns),
        'transforms': to_jsonlike(x.transforms),
    }


@to_jsonlike.register(DSequence)
def _(x):
    return {
        'query_type': 'DSequence',
        'sequence': [to_jsonlike(q) for q in x]
    }


############################ Query Generator interface #######################

class QueryGenerator(ABC):

    @abstractmethod
    def resolve(self, columns): # resolve to data query without fetching data
        pass


##################### Query Generator instance: DataModel ####################

# describe Table data. 
# joinby describes the column that joins the table into the full data set

ForeignKey = namedtuple('ForeignKey', ['table', 'column'])
TableMeta = namedtuple('TableMeta', ['name', 'columns', 'foreign'])


def make_foreignkey(table, column):
    return ForeignKey(table, to_set(column))


@to_jsonlike.register(ForeignKey)
def _(x):
    return to_jsonlike(dict(x._asdict()))


@to_jsonlike.register(TableMeta)
def _(x):
    ret = {
        'name': to_jsonlike(x.name),
        'columns': to_jsonlike(x.columns),
        'foreign': to_jsonlike(x.foreign)
    }
    return ret


class DataModel(QueryGenerator):
    '''
    Manages data source for one data category. 
    Each data category supports a list of columns which come from different
    data tables in the backend. An object of this type maintains the internal
    data source dependency and generate minimal data query for user columns

    For example, 

    T0: C1|C2|C3|C4, # Table T0 has four columns C1, C2, C3, C4
    T1: C1|C20|C30, joining T0 by C1
    T2: C2|C50|C60|C200|C300, joining T0 by C2
    T3: C50|C500, joining T2 by C50
    T4: C60|C600, joining T2 by C60

    1. internally T0-T5 forms a directed tree. (therefore a dag as well)
    2. Foreign keys show up only once in the diagram. (more in the future?)
    3. non foreign key columns from each table are disjoint

    For user query (C1|C3), only table T0 is needed
    For user query (C1|C500), table T0, T2, T3 are needed for minimal query
    For user query (C2|C60|C500), table T2, T3 are needed for minimal query
    For user query (C50|C600) , table T2, T4 are needed for minimal query
    '''
    def __init__(self, table_metas):
        
        logger.info('initializing DataModel...')
        self.tables = {} # table name to table meta, representing a graph
        self.foreign_map = {} # joining foreign table -> foreign key

        _unseen_table_names = set()
        _temp_graph = {} # to figure out topological order of tables
        for tm in table_metas:
    
            # make sure we haven't seen this table before
            if tm.name in self.tables:
                raise RuntimeError('table %s already exists' % tm.name)

            _foreign_tables = self._update_foreign_map(tm)
            _temp_graph[tm.name] = _foreign_tables

            self.tables[tm.name] = tm
            _unseen_table_names.discard(tm.name)
            _unseen_table_names.update(_foreign_tables - self.tables.keys())

        if _unseen_table_names:
            raise RuntimeError(
                'table metas contain unknown tables:%s' % _unseen_table_names)
        logger.debug('foreign key map:%s' % self.foreign_map)
        # perform topological sort to obtain foreign table chain
        def _get_adjacent_list(dag, node):
            return dag.get(node, set())

        self.adj_func = _get_adjacent_list
        self.graph = _temp_graph
        
        logger.info('internal graph is built')
        self.leaf_first_order = topological_order(
            self.graph, self.adj_func, self.tables.keys(),
            source_first=False, include_connection=True)

        self.access_map = self._compute_access_map()

    def _update_foreign_map(self, tm):
        if tm.foreign is None:
            return set() # empty foreign tables

        _tm_foreign_map = dict(tm.foreign)

        _tm_foreign_tables = set(_tm_foreign_map.keys())
        _tm_foreign_columns = set()
        for _val in _tm_foreign_map.values():
            if isinstance(_val, str):
                _tm_foreign_columns.add(_val)
            else:
                _tm_foreign_columns.update(_val)

        assert len(_tm_foreign_tables) == len(tm.foreign), \
            'contains duplicated foreign tables:%s' % (tm,)

        # make sure foreign keys present in the column list
        assert _tm_foreign_columns.issubset(set(tm.columns)), \
            'table columns do not include all foreign keys:%s' % (tm,)

        # make sure foreign tables do not include itself
        assert tm.name not in _tm_foreign_tables, \
            'has itself as a foreign table:%s' % (tm,)

        _merged_size = len(self.foreign_map) + len(tm.foreign)
        self.foreign_map.update(_tm_foreign_map)

        assert len(self.foreign_map.keys()) == _merged_size, \
            'encountered duplicated foreign tables'
        assert len(self.foreign_map.values()) == _merged_size, \
            'encountered duplicated foreign columns'
    
        return _tm_foreign_tables

    def _compute_access_map(self):
        '''
        use memorization to find the access map instead of going through
        topological order of the graph
        '''
        access_map = {}

        def _calculate_access(table_name):

            if table_name in access_map: 
                return access_map[table_name]

            sub = self.adj_func(self.graph, table_name)
            ret = set(self.tables[table_name].columns)
            if sub:
                for each in sub:
                    ret.update(_calculate_access(each))
            access_map[table_name] = ret
            return ret
        
        for table_name in self.tables:
            _calculate_access(table_name)

        return access_map

    def resolve(self, columns):

        selection = OrderedDict()

        def _add_columns(table_name, columns):
            selection.setdefault(table_name, set()).update(columns)

        def _select(table_name, columns):

            covered_columns = self.access_map[table_name]
            if not covered_columns.issuperset(columns):
                raise RuntimeError('cannot select %s through %s' % (columns, table_name))

            join_tables = self.adj_func(self.graph, table_name)
            
            # if the table contains all columns already, just use the table
            available_columns = set(self.tables[table_name].columns)
            if not join_tables or available_columns.issuperset(columns):
                _add_columns(table_name, columns)
            else:
                still_needed = columns - available_columns
                
                some_coverage = {} # table to partially covered columns set
                full_coverage = None
                for each in join_tables:
                    each_covered_columns = self.access_map[each]
                    if each_covered_columns.issuperset(columns):
                        full_coverage = each
                        break

                    each_selection = each_covered_columns.intersection(still_needed)
                    if each_selection:
                        some_coverage[each] = each_selection
                        some_coverage[each].update(self.foreign_map[each])
                        
                if full_coverage:
                    _select(full_coverage, columns)
                else:
                    _add_columns(table_name, available_columns.intersection(columns))
                    for each, cover in some_coverage.items():
                        _select(each, cover)
                        _add_columns(table_name, self.foreign_map[each])

        _select(self.leaf_first_order[-1].name, set(columns))
         
        
        queries = DSequence()
        for i, (table_name, table_cols) in enumerate(selection.items()):
            queries.append(
                DJoin(
                    source=DSelect(table=table_name, columns=table_cols, transforms=None),
                    joinby=None if i == 0 else self.foreign_map[table_name]
                )
            )
        return queries


@to_jsonlike.register(DataModel)
def _(x):
    return {
        'query_gen_type': 'DataModel',
        'table_meta': dict(
            (_table_name, to_jsonlike(_meta))
            for _table_name, _meta in x.tables.items()
        )
    }


################ Query Generator instance: DataQuery #######################

class DataQuery(QueryGenerator):
    '''
    provides the full data query sequence to fetch the entire data set
    it can generate a sequence of data left joins. If used for fetching
    data, a full flat table would be generated on the 'data model'. 
    It covers everything that data model can do but this class generates
    full data query instead of on selected columns

    examples:

    1)
    For simple cases that DataModel supports:
    T0:C1|C2
    T1:C2|C3, join onto T0 on C2, 

    it generates:
    DSequence([
        DJoin(source=DSelect(table=T0, columns=[C1, C2], transforms=None), joinby=None),
        DJoin(source=DSelect(table=T1, columns=[C2, C3], transforms=None), joinby=[C2])
    ])

    2)
    A more complicated case would be, not possible with the current DataModel
    T0:C1|C2
    T1:C2|C3, join onto T0 on C2
    T2:C1|C3|C4, join on the result of T0, T1 on C1,C3
    
    it generates:
    DSequence([
        DJoin(source=DSelect(table=T0, columns=[C1, C2], transforms=None), joinby=None),
        DJoin(source=DSelect(table=T1, columns=[C2, C3], transforms=None), joinby=[C2]),
        DJoin(source=DSelect(table=T2, columns=[C1, C3, C4], transforms=None), joinby=[C1,C3])
    ])

    3)
    A mutually recursive structure that replicates more complicated cases:
    T2 joins onto T1 on C200, with their result joining onto T0 on C2,C3 together
    T0:C1|C2|C3
    T1:C2|C200|C201
    T2:C3|C200|C301

    It should generate:
    DSequence([
        DJoin(source=DSelect(table=T0, columns=[C1,C2,C3], transforms=None), joinby=None),
        DJoin(
            source=DSequence([
                DJoin(
                    source=DSelect(table=T1, columns=[C2,C200,C201], transforms=None),
                    joinby=None),
                DJoin(
                    source=DSelect(table=T2, columns=[C3,C200,C301], transforms=None),
                    joinby=[C200])
            ]),
            joinby=[C2,C3])
    ])

    example 3) has the most general cases. Example 1) and 2) both fit there
    '''
    def __init__(self, query):
        self.query = query
        self.table_col_info = validate_data_source(self.query)
        self.all_cols = set()
        for _, cols in self.table_col_info.items():
            self.all_cols.update(cols)

    def resolve(self, columns):
        _columns = to_set(columns)
        if self.all_cols.issuperset(_columns):
            return self.query
        
        missing = _columns - self.all_cols
        raise RuntimeError(
            'missing following columns in query:%s' % (_columns-self.all_cols)
        )
        

@to_jsonlike.register(DataQuery)
def _(x):
    return {
        'query_gen_type': 'DataQuery',
        'query': to_jsonlike(x.query),
        # 'table_col_info': to_jsonlike(x.table_col_info),
        # 'all_cols': to_jsonlike(x.all_cols),
    }


## -------------- DateRange/DateTimePoint related for data key --------------
## These data structures are used as data source timestamp/duration etc

class DateRange(object):
    
    DATE_FORMAT = '%Y%m%d'
    PRETTY_DATE_FORMAT = '%Y.%m.%d'

    def __init__(self, sd, ed=None):
        self.sd = sd
        self.ed = sd if ed is None else ed

    def __str__(self):
        return (
            'dr' + 
            '-'.join([
                datetime.datetime.strftime(self.sd, DateRange.DATE_FORMAT),
                datetime.datetime.strftime(self.ed, DateRange.DATE_FORMAT)
            ])
        )

    def __repr__(self):
        return repr((self.sd, self.ed))
    
    def __hash__(self):
        return hash((self.sd, self.ed))

    def __eq__(self, other):
        return self.sd == other.sd and self.ed == other.ed

    def cover(self, other):
        if not isinstance(other, DateRange):
            return False
        return self.sd <= other.sd and self.ed >= other.ed

    def dates(self):
        return enumerate_dates(self.sd, self.ed)

    ## util/higher order functions for DateRange generation
    @staticmethod
    def past_days(start_ago, end_ago):
        '''make date range in the past, while negative values mean future days'''
        assert start_ago >= end_ago, 'start date shall come before end date'
        def _inner(dt): # datetime.date or datetime.datetime object
            _start = get_days_ago(dt, start_ago)
            _end = get_days_ago(dt, end_ago)
            return DateRange(_start, _end)
        return _inner
    
    @staticmethod
    def past_weekdays(start_ago, end_ago):
        '''make date range in the past week days, while negative mean future week days'''
        assert start_ago >= end_ago, 'start date shall come before end date'
        def _inner(dt):
            _start = get_weekdays_ago(dt, start_ago)
            _end   = get_weekdays_ago(dt, end_ago)
            return DateRange(_start, _end)
        return _inner
    
    @staticmethod
    def just_today(d): # datetime.date or datetime.datetime object
        return DateRange(get_days_ago(d, 0))

    @staticmethod
    def just_tomorrow(d): # datetime.date or datetime.datetime object
        return DateRange(get_days_ago(d, -1))

    @staticmethod
    def past_month(months_ago):
        '''
        get the date range object that represents a past month. 
        For months_ago=0, 
        it generates the dates from the beginning of the month to the input date
        For positive input, it generates full dates for a month in the past
        '''
        assert months_ago >= 0
        def _in(d): # datetime.datetime object or a datetime.date object
            _this = get_days_ago(d, 0)
            _1st_of_month = first_date_of_month(_this)
            
            _months_ago = months_ago
            if _months_ago == 0:
                return DateRange(_1st_of_month, _this)
            else:
                while _months_ago:
                    _last_of_month = get_days_ago(_1st_of_month, 1)
                    _1st_of_month = first_date_of_month(_last_of_month)
                    _months_ago -= 1
    
                return DateRange(_1st_of_month, _last_of_month)
    
        return _in

    @staticmethod
    def past_weekday(d): # datetime.date or datetime.datetime object
        _prev_date = last_weekday(d)
        return DateRange(_prev_date, _prev_date)

    @staticmethod   
    def since(since_date):
        def _inner(d): # datetime.date or datetime.datetime object
            this_day = get_days_ago(d, 0)
            return DateRange(since_date, this_day)
        return _inner

    @staticmethod
    def year_to_last_month(d): # datetime.date or datetime.datetime object
        last_of_prev_month = get_days_ago(first_date_of_month(d), 1)
        return DateRange(
            datetime.date(last_of_prev_month.year, 1, 1),
            last_of_prev_month)


class DateTimePoint(object):
    '''one second granularity datetime object'''

    DATETIME_FORMAT = '%Y%m%d%H%M%S'
    PRETTY_DATETIME_FORMAT = '%Y.%m.%d %H:%M:%S'
    PRETTY_DATE_FORMAT = '%Y.%m.%d'
    
    def __init__(self, dt=None):
        if dt is None:
            dt = datetime.datetime.now()
        self.t = dt.replace(microsecond=0)

    def __str__(self):
        return (
            'dt' + 
            datetime.datetime.strftime(self.t, DateTimePoint.DATETIME_FORMAT)
        )

    def __repr__(self):
        return repr(self.t)

    def __hash__(self):
        return hash(self.t)

    def __eq__(self, other):
        return self.t == other.t

    def cover(self, other):
        if not isinstance(other, DateTimePoint):
            return False
        return self.t >= other.t

    @staticmethod
    def just_right_now(d): # datetime.datetiem object
        return DateTimePoint(d)

    @staticmethod
    def last_time_of(hour=0, minute=0, second=0):
        def _in(d): # datetime.date or datetime.datetime object
            return DateTimePoint(time_of(d, hour, minute, second))
        return _in

    @staticmethod
    def last_minutes_bin(bin_size, bins_ago):
        def _inner(dt): # a datetime object
            return DateTimePoint(past_minutes_bin_of(dt, bin_size, bins_ago))
        return _inner


# ----------------------------------------------------------------------------
@functools.singledispatch
def pretty_srcdt(x):
    raise RuntimeError('cannot output pretty srcdt format for %s' % type(x))


@pretty_srcdt.register(DateRange)
def _(x):
    if x.sd == x.ed:
        return datetime.datetime.strftime(
            x.sd, DateRange.PRETTY_DATE_FORMAT)
    
    else:
        return '-'.join([
            datetime.datetime.strftime(x.sd, DateRange.PRETTY_DATE_FORMAT),
            datetime.datetime.strftime(x.ed, DateRange.PRETTY_DATE_FORMAT),
        ])


@pretty_srcdt.register(DateTimePoint)
def _(x):
    return datetime.datetime.strftime(x.t, DateTimePoint.PRETTY_DATE_FORMAT)


@pretty_srcdt.register(list)
def _(x):
    return ','.join(pretty_srcdt(each) for each in x)


# ----------------------------------------------------------------------------
@functools.singledispatch
def pretty_srcdt2(x):
    '''just output the 2nd source date'''
    raise RuntimeError('cannot output pretty srcdt #2 format for %s' % type(x))


@pretty_srcdt2.register(DateRange)
def _(x):
    '''output the end date for DateRange'''
    return datetime.datetime.strftime(x.ed, DateRange.PRETTY_DATE_FORMAT)


@pretty_srcdt2.register(DateTimePoint)
def _(x):
    '''just gives the same day'''
    return datetime.datetime.strftime(x.t, DateTimePoint.PRETTY_DATE_FORMAT)


@pretty_srcdt2.register(list)
def _(x):
    return pretty_srcdt2(x[-1])


# ----------------------------------------------------------------------------
@functools.singledispatch
def pretty_date(x):
    raise RuntimeError('cannot output pretty date format for %s' % type(x))


@pretty_date.register(DateRange)
def _(x):
    return pretty_srcdt(x)


@pretty_date.register(DateTimePoint)
def _(x):
    return datetime.datetime.strftime(x.t, DateTimePoint.PRETTY_DATE_FORMAT)


@pretty_date.register(list)
def _(x):
    return ','.join(pretty_date(each) for each in x)


#################### AcquisitionKey and util functions ######################

CSVloc = namedtuple(
    'CSVloc', ['user','host','path','name'])

AcquisitionKey = namedtuple(
    'AcquisitionKey', ['acquisition', 'env', 'srcdt'])

AcquisitionKeyGen = namedtuple(
    'AcquisitionKeyGen', ['acquisition', 'env', 'srcdtgen'])


@functools.singledispatch
def generate_srcdt(x, dt): # by default, x is treated as a callable
    return x(dt)


@generate_srcdt.register(DateRange)
def _(x, dt):
    return x


@generate_srcdt.register(DateTimePoint)
def _(x, dt):
    return x


@generate_srcdt.register(tuple)
def _(x, dt):
    stgen, etgen = x
    return DateRange(stgen(dt), etgen(dt))


# for generation of the key from job scheduler/current time context etc
@functools.singledispatch
def make_acquisition_key(x, dt): # dt :: datetime.datetime
    return AcquisitionKey(
        acquisition=x.acquisition,
        env=x.env,
        srcdt=generate_srcdt(x.srcdtgen, dt)
    )


@make_acquisition_key.register(list)
def _(x, dt):
    return [make_acquisition_key(each, dt) for each in x]


@functools.singledispatch
def acquisition_key_to_str(x):
    return '.'.join([x.acquisition, x.env, str(x.srcdt)])


@acquisition_key_to_str.register(list)
def _(x):
    return '.'.join(acquisition_key_to_str(each) for each in x)


@to_jsonlike.register(AcquisitionKey)
def _(x):
    return {
        'acquisition': x.acquisition,
        'env': x.env,
        'srcdt': str(x.srcdt),
    }


def acquisition_key_from_string(s):
    tokens = s.split('.')
    q, r = divmod(len(tokens), 3)
    if r != 0 or q == 0: 
        raise RuntimeError('cannot generate acquisition key from %s' % s)
    if q == 1:
        acq_name, env, srcdt_str = tokens
        return _acquisition_key_from_tokens(acq_name, env, srcdt_str)
    else:
        ret = []
        while tokens:
            acq_name, env, srcdt_str = tokens[:3]
            tokens = tokens[3:]
            ret.append(_acquisition_key_from_tokens(acq_name, env, srcdt_str))
        return ret


def _acquisition_key_from_tokens(acq_name, env, srcdt_str):
    if srcdt_str.startswith('dr'):
        sd, ed = srcdt_str[2:].split('-')
        srcdt = DateRange(
            datetime.datetime.strptime(sd, DateRange.DATE_FORMAT).date(),
            datetime.datetime.strptime(ed, DateRange.DATE_FORMAT).date())

    elif srcdt_str.startswith('dt'):
        srcdt = DateTimePoint(
            datetime.datetime.strptime(
                srcdt_str[2:], DateTimePoint.DATETIME_FORMAT))
        
    else:
        raise RuntimeError('unknown source date type:%s' % srcdt_str)

    return AcquisitionKey(acq_name, env, srcdt)
