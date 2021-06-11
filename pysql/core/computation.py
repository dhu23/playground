'''
Computation related structure, algorithms and interfaces
'''


__all__ = [
    'ComputationError', 
    'compute_by_row', 
    'compute_by_column',
    'transform_dataframe',
    'RSelect',
    'RDelete',
    'RRowFilter',
    'RColumnFilter',
    'RRename',
    'RMultiRename',
    'RRowAdjust',
    'RColumnAdjust',
    'RAggregate',
    'RRowSort',
    'RColumnAdd',
    'SelectBuilder',
    'FilterBuilder',
    'RQuery',
    'make_custom_computation',
    'ComputationSequence',
    'make_computation_key',
    'Hierarchy',
]


from collections import namedtuple, UserDict
import functools
import itertools
import pandas as pd
from pprint import pformat

import logging

from core.util import (
    make_function,
    append_unique_list,
    assert_dataframe_columns,
    series_to_dataframe,
    concat_dataframes,
    to_jsonlike,
    to_list,
    to_set,
    to_frozenset,
    topological_order,
    map_over_dict_value,
    map_over_dict,
)

from core import util # for function mapping

logger = logging.getLogger(__name__)


class ComputationError(RuntimeError): pass


############## Lift regular functions into DataFrame context ################


## --------------- compute_by_row: lift to row by row context ---------------
# 1) a given list/str/tuple of deps, as columns from a dataframe
# 2) a function that takes a list of argument that matches deps
# 3) a dataframe
# lift the function into the dataframe row by row and return a of series


@functools.singledispatch
def compute_by_row(x, func, df): # return a pandas series
    raise ComputationError('cannot compute by row on type %s input' % type(x))


@compute_by_row.register(str)
def _(x, func, df): 
    assert_dataframe_columns(df, x)
    return df[x].apply(func)


@compute_by_row.register(list)
def _(x, func, df):
    if not x: 
        raise ComputationError('cannot compute without dependency provided')
    assert_dataframe_columns(df, x)
    if len(x) == 1: 
        return compute_by_row(x[0], func, df)

    def _f(row): 
        return func(*[row[each] for each in x])
    return df.apply(_f, axis=1)


@compute_by_row.register(tuple)
def _(x, func, df):
    return compute_by_row(list(x), func, df)


## -------------- compute_by_column: lift to column context ------------------
# 1) a given list/str/tuple of deps, as columns from a dataframe
# 2) a function that takes a list of argument that matches deps
# 3) a dataframe
# similar to compute_by_row, except that the dataframe is no treated as a list
# of rows, but a collection of columns
@functools.singledispatch
def compute_by_column(x, func, df):
    raise ComputationError('cannot compute by column on type %s' % type(x))


# such as count, it is independent from any column
compute_by_column.register(type(None), lambda x, func, df: func(df))


@compute_by_column.register(str)
def _(x, func, df): 
    assert_dataframe_columns(df, x)
    return func(df[x])


@compute_by_column.register(list)
def _(x, func, df): 
    if not x:
        raise ComputationError('cannot compute without dependency provided')
    assert_dataframe_columns(df, x)

    if len(x) == 1: 
        return compute_by_column(x[0], func, df)
    return func(*[df[each] for each in x])


@compute_by_column.register(tuple)
def _(x, func, df): 
    return compute_by_column(list(x), func, df)


###### transform_dataframe: DataFrame transforming types and functions #######

# a function that applies a dataframe transformation function and apply it
@functools.singledispatch
def transform_dataframe(x, df):
    raise ComputationError('cannot apply type %s to df' % type(x))


@transform_dataframe.register(list)
def _(x, df):
    ret = df
    for each in x:
        logger.debug('applying %s on\n%s' % (to_jsonlike(each), repr(ret)))
        ret = transform_dataframe(each, ret)
        logger.debug('returning\n%s' % repr(ret))
    return ret


# None type does no transformation
transform_dataframe.register(type(None), lambda x, df : df)


## ------------------------------ RSelect -----------------------------------
RSelect = namedtuple('RSelect', ['columns'])


@to_jsonlike.register(RSelect)
def _(x):
    return {
        'transformation_type': 'RSelect',
        'columns': to_list(x.columns)
    }


@transform_dataframe.register(RSelect)
def _(x, df):
    assert_dataframe_columns(df, x.columns)
    return df[to_list(x.columns)]


## ------------------------------- RDelete ---------------------------------
RDelete = namedtuple('RDelete', ['columns'])


@to_jsonlike.register(RDelete)
def _(x):
    return {
        'transformation_type': 'RDelete',
        'columns': to_list(x.columns)
    }


@transform_dataframe.register(RDelete)
def _(x, df):
    return df.drop(columns=to_list(x.columns))


## -------------------------------- RRowFilter -----------------------------
RRowFilter = namedtuple('RRowFilter', ['name', 'func'])


@to_jsonlike.register(RRowFilter)
def _(x):
    return {
        'transformation_type': 'RRowFilter',
        'name': x.name,
        'func': x.func.__qualname__
    }


@transform_dataframe.register(RRowFilter)
def _(x, df):
    if x.name:
        # NOTE this is a loss of efficiency there by not vectoring
        # but it supports general operation that's not vectorized
        return df.loc[compute_by_row(x.name, x.func, df)].reset_index(drop=True)
    else:
        return df # nothing to filter on


## ----------------------------- RColumnFilter ------------------------------
# look at the entire column collectively to filter out certain positions
RColumnFilter = namedtuple('RColumnFilter', ['column', 'func'])


@to_jsonlike.register(RColumnFilter)
def _(x):
    return {
        'transformation_type': 'RColumnFilter',
        'column': x.column,
        'func': x.func.__qualname__
    }


@transform_dataframe.register(RColumnFilter)
def _(x, df):
    column, func = x
    if not column in df.columns:
        raise ComputationError(
            'RColumnFilter column not in dataframe:%s' % colume)
    return df.loc[func(df[column])].reset_index(drop=True)


## ------------------------------- RRename ---------------------------------
RRename = namedtuple('RRename', ['name', 'to'])


@to_jsonlike.register(RRename)
def _(x):
    ret = dict(x._asdict())
    ret.update({'transformation_type': 'RRename'})
    return to_jsonlike(ret)


@transform_dataframe.register(RRename)
def _(x, df): return df.rename(columns={x.name:x.to})


## ---------------------------- RMultiRename ------------------------------
class RMultiRename(UserDict): pass


@to_jsonlike.register(RMultiRename)
def _(x):
    return {
        'transformation_type': 'RMultiRename',
        'renames': to_jsonlike(x.data)
    }


@transform_dataframe.register(RMultiRename)
def _(x, df): return df.rename(columns=x.data)


## --------------------------- RRowAdjust ------------------------------
# Used for destructively update the column
# can be used for two different cases:
# 1. final row level adjustment to calculate a new column
# 2. final row level adjustment to destructively adjust the column itself
RRowAdjust = namedtuple('RRowAdjust', ['name', 'func', 'deps'])


@to_jsonlike.register(RRowAdjust)
def _(x):
    return {
        'transformation_type': 'RRowAdjust',
        'name': to_jsonlike(x.name),
        'func': x.func.__qualname__,
        'deps': to_jsonlike(x.deps)
    }


@transform_dataframe.register(RRowAdjust)
def _(x, df):
    if df.empty:
        return pd.DataFrame(
            columns=append_unique_list(list(df.columns), to_list(x.name)))
        
    s = compute_by_row(x.deps, x.func, df)
    return concat_dataframes(df, series_to_dataframe(x.name, s))


# -------------------------- RColumnAdjust --------------------------------
# technically the same as RRowAdjust, except that the function 
# is applied to the data in a different fashion. You can technically
# achieve the same effect with both of them. In other words, this
# can be vectorized version of RRowAdjust
RColumnAdjust = namedtuple('RColumnAdjust', ['name', 'func', 'deps'])


@to_jsonlike.register(RColumnAdjust)
def _(x):
    return {
        'transformation_type': 'RColumnAdjust',
        'name': to_jsonlike(x.name),
        'func': x.func.__qualname__,
        'deps': to_jsonlike(x.deps)
    }


@transform_dataframe.register(RColumnAdjust)
def _(x, df):
    s = compute_by_column(x.deps, x.func, df)
    return concat_dataframes(df, series_to_dataframe(x.name, s))


# -------------------------- RAggregate ---------------------------------
RAggregate = namedtuple('RAggregate', ['by', 'aggs'])


@to_jsonlike.register(RAggregate)
def _(x):
    return {
        'transformation_type': 'RAggregate',
        'by': to_jsonlike(x.by),
        'aggs': to_jsonlike(x.aggs)
    }


@functools.singledispatch
def _pair_to_df_dict(x, y):
    '''turn x, and y into a dictionary, { x : y } like structure'''
    return { x : [y] }


@_pair_to_df_dict.register(list)
def _(x, y):
    return dict((_x, [_y]) for _x, _y in zip(x, y))


@_pair_to_df_dict.register(tuple)
def _(x, y):
    return _pair_to_df_dict(list(x), y)


@transform_dataframe.register(RAggregate)
def _(x, df):
    _by = to_list(x.by)

    if df.empty:
        _columns = list(_by)
        for _agg in x.aggs:
            _columns.extend(to_list(_agg.name))
        return pd.DataFrame(columns=list(_columns))

    if not _by:
        # aggregating without providing grouping, 
        # this is treated as a full column aggregate
        if not x.aggs:
            return df
        else:
            for i, _agg in enumerate(x.aggs):
                _x = _pair_to_df_dict(
                    _agg.name, compute_by_column(_agg.deps, _agg.func, df)
                )
                if i == 0: 
                    ret_dict = _x
                else:
                    ret_dict.update(_x)
                #print(ret_dict)
                #print(pd.DataFrame(ret_dict))
            return pd.DataFrame(ret_dict)

    else:
        grouped = df.groupby(_by)
        if not x.aggs: 
            # grouping without aggregation, 
            # aggregate count and then drop all but the grouping keys
            ret = grouped.count().reset_index()
            return ret[_by]
        else:
            for i, _agg in enumerate(x.aggs):
                for _agg_name in to_list(_agg):
                    if _agg_name in _by:
                        raise ComputationError(
                            'cannot rename aggregation key %s' % _agg_name)
                
                def _f(grouped_df):
                    return compute_by_column(_agg.deps, _agg.func, grouped_df)

                _x = series_to_dataframe(_agg.name, grouped.apply(_f))
                if i == 0: 
                    ret = _x
                else: 
                    ret = pd.concat([ret, _x], axis=1)

            return ret.reset_index()


## ------------------------------ RRowSort -------------------------------
RRowSort = namedtuple('RRowSort', ['by', 'ascending'])


@to_jsonlike.register(RRowSort)
def _(x):
    return {
        'transformation_type': 'RRowSort',
        'by': to_jsonlike(x.by),
        'ascending': to_jsonlike(x.ascending)
    }


@transform_dataframe.register(RRowSort)
def _(x, df):
    return df.sort_values(x.by, ascending=x.ascending).reset_index(drop=True)


@functools.singledispatch
def to_row_sort(x):
    raise RuntimeError('cannot convert to row sort from %s' % type(x))


@to_row_sort.register(dict)
def _(x):
    return RRowSort(by=x['by'], ascending=x.get('ascending', True))


@to_row_sort.register(RRowSort)
def _(x):
    return x


@to_row_sort.register(str)
def _(x):
    return RRowSort(by=x, ascending=True)


@to_row_sort.register(tuple)
def _(x):
    _by, _order = x
    return RRowSort(by=_by, ascending=bool(_order))


## ---------------------------- RColumnAdd ---------------------------------
# add a new column with given values. technically this can be achieved by 
# RRowAdjust but RColumnAdd makes the syntax a lot easier
# loc is an integer identifying the location of the column after the insertion
# negative values indicates counting from the back, similar to list slicing
RColumnAdd = namedtuple('RColumnAdd', ['name', 'value', 'loc'])


@to_jsonlike.register(RColumnAdd)
def _(x):
    return {
        'transformation_type': 'RColumnAdd', 
        'name': x.name,
        'value': x.value,
        'loc': x.loc,
    }


@transform_dataframe.register(RColumnAdd)
def _(x, df):
    if x.loc >= 0:
        pos = x.loc
    else:
        pos = len(df.columns) + 1 + x.loc
        
    df.insert(x.loc, x.name, x.value)
    return df


############## Query data/field Builder and its utlity functions #############

# Data type to store select column information in RQuery
SelectBuilder = namedtuple('SelectBuilder', ['name', 'rename']) 


@to_jsonlike.register(SelectBuilder)
def _(x):
    return to_jsonlike(x._asdict())


@functools.singledispatch
def to_select_builder(x):
    raise RuntimeError('cannot convert to select builder from %s' % type(x))


@to_select_builder.register(dict)
def _(x):
    return SelectBuilder(x['name'], x.get('rename', None))


@to_select_builder.register(SelectBuilder)
def _(x):
    return x


@to_select_builder.register(str)
def _(x):
    return SelectBuilder(x, None)


@to_select_builder.register(tuple)
def _(x):
    return SelectBuilder(*x)


# Data type to store filters in RQuery
FilterBuilder = namedtuple('FilterBuilder', ['name', 'fn', 'arg'])


@to_jsonlike.register(FilterBuilder)
def _(x): 
    return to_jsonlike(x._asdict())


@functools.singledispatch
def to_filter_builder(x):
    raise RuntimeError('cannot convert to filter builder from %s' % type(x))


@to_filter_builder.register(dict)
def _(x):
    return FilterBuilder(x['name'], x.get('fn', 'equal_to'), x['arg'])


@to_filter_builder.register(FilterBuilder)
def _(x):
    return x


@to_filter_builder.register(tuple)
def _(x):
    return FilterBuilder(*x)


def _make_row_filter(filter_builder):
    _func_obj = getattr(util, filter_builder.fn, None)
    if _func_obj is None:
        raise ComputationError(
            'filter function %s not supported in library' % filter_builder.fn)

    # NOTE do not check against list type. list argument is taken as one
    if isinstance(filter_builder.arg, tuple):
        _func = _func_obj(*filter_builder.arg)
    else:
        _func = _func_obj(filter_builder.arg)

    return RRowFilter(name=filter_builder.name, func=_func)



class RQuery(object):
    '''
    build some simple type of reports. 
    for more sophisticated RQuery, build its constructor directly.
    The main use case of this is to convert string/text data to query.

    select: one or a list of SelectBuilder or tuple or empty
    - when there is one or more fields, select them at the end
    - when it is empty, it is equivalent to "selecting all"

    groupby: zero, one or a list of column names or None
    - when >=1 columns, groupby those columns
    - when ==0 column, treating the whole table as one group
    - when None, no grouping at all

    filterby: a list of FilterBuilder or tuple
    '''

    def __init__(
        self, 
        data=None,
        select=None, 
        groupby=None, 
        sortby=None,
        filterby=None,
    ): 
        '''data is a jsonlike representation'''
        if any([select is not None, 
                groupby is not None,
                sortby is not None,
                filterby is not None]):
            assert data is None, \
                'cannot accept data when select/groupby/sortby/filterby exists'
            self._columns = list(map(to_select_builder, to_list(select)))
            self._groupby = groupby
            self._sortby = to_row_sort(sortby) if sortby else None
            self._filterby = list(map(to_filter_builder, to_list(filterby)))

        else: # none of select/groupby/filterby is provided
            self._columns = []
            self._groupby = None
            self._sortby = None
            self._filterby = []

            if data:
                _data_select = data.get('select')
                if _data_select:
                    self._columns.extend(
                        to_select_builder(sel) for sel in _data_select)

                _data_groupby = data.get('groupby', None)
                if _data_groupby:
                    self._groupby = to_list(_data_groupby)

                _data_sortby = data.get('sortby', None)
                if _data_sortby:
                    self._sortby = to_row_sort(_data_sortby)

                _data_filterby = data.get('filterby')
                if _data_filterby:
                    self._filterby.extend(
                        to_filter_builder(ft) for ft in _data_filterby)
                
    def filterby(self, name=None, fn='equal_to', arg=None):
        if name is None and arg is None: 
            pass
        elif name is not None and arg is not None:
            self._filterby.append(FilterBuilder(name, fn, arg))
        else:
            raise RuntimeError('cannot filter by name=%s, arg=%s' % (name, arg))
            
        return self # for chaining

    @property
    def resolvable_cols(self):
        # print(to_jsonlike(self))
        ret = set()
        ret.update(to_set(self._groupby))
        #logger.info(to_jsonlike(self))
        ret.update(to_set(col.name for col in self._columns))
        for fb in self._filterby:
            ret.update(to_set(fb.name))
        return ret

    @property
    def row_filters(self):
        return [_make_row_filter(fb) for fb in self._filterby]

    @property
    def select_columns(self):
        return [_from for _from, _ in self._columns]

    @property
    def renamed_columns(self):
        return dict((_from, _to) for _from, _to in self._columns if _to)

    @property
    def is_selecting_all(self):
        return not bool(self._columns)

    @property
    def group_columns(self):
        return self._groupby

    @property
    def is_agg(self):
        return self._groupby is not None

    def batch_row_filters(self, df):
        '''
        row filters are applied aggressively on a dataframe. As long as the 
        dataframe has the columns to support running a filter, the filter
        would be performed. This function breaks the row filters into two
        groups, one that can be applied initially and one to be run after
        all the non-agg computation
        '''
        df_column_set = set(df.columns)

        _before_non_agg_batch, _after_non_agg_batch = [], []
        for ft in self.row_filters:
            if df_column_set.issuperset(to_set(ft.name)):
                _before_non_agg_batch.append(ft)
            else:
                _after_non_agg_batch.append(ft)

        return _before_non_agg_batch, _after_non_agg_batch


@to_jsonlike.register(RQuery)
def _(x):
    return {
        'transformation_type': 'RQuery',
        'select': to_jsonlike(x._columns), 
        'groupby': to_jsonlike(x._groupby),
        'filterby': to_jsonlike(x._filterby),
    }


def make_custom_computation(adjust_type, result_vals, func_string, arg_vals):
    '''converts any user-customized computation into a report computation'''
    return adjust_type(
        name=result_vals,
        func=make_function(func_string, arg_vals),
        deps=arg_vals)


ComputationSequence = namedtuple('ComputationSequence', ['non_agg', 'agg'])

RHierCompInfo = namedtuple('RHierCompInfo', ['to', 'isagg', 'deps'])
RHierComp = namedtuple('RHierComp', ['info', 'func'])


@functools.singledispatch
def to_hiercompinfo(x):
    raise ComputationError('%s is not a hierarchical computation' % type(x))


@to_hiercompinfo.register(RRowAdjust)
def _(x): 
    return RHierCompInfo(
        to=to_list(x.name), isagg=False, deps=to_list(x.deps))


@to_hiercompinfo.register(RColumnAdjust)
def _(x): 
    return RHierCompInfo(
        to=to_list(x.name), isagg=True, deps=to_list(x.deps))


def make_computation_key(x):
    return to_frozenset(to_list(x))


## -------------- Hierarchy: container of computation hierarchy -------------
class Hierarchy(object):
    '''
    mapping each unique column including both agg and non-agg to its 
    computation methods. Equivalently the internal of the class forms a
    DAG that determines the calculation hierarchy and dependency. 
    Each column has a unique name; the *same* column would have two distinct
    names in agg and non-agg computations

    RRowAdjust and RColumnAdjust can support broader user cases than what 
    Hierarchy class supports, which imposes uniqueness of columns/computations
    '''
    def __init__(self, computations=None):
        # NOTE multiple columns might share the same computation
        self.col_key_dict = {} # mapping from col to CompKey
        self.key_comp_dict = {} # mapping form CompKey to RHierComp object

        if computations:
            for each in computations:
                self.add(each)

        logger.debug(pformat(to_jsonlike(self)))

    def add(self, comp): # radjust is either row adjust or column adjust
        _info = to_hiercompinfo(comp) # potentially throws an exception
        # NOTE convert name to a frozenset as it is stored as computation key
        comp_key = make_computation_key(_info.to)

        # NOTE impose uniqueness column check. 
        # generated columns shall not have the same name as their dependency
        dep_set = to_set(_info.deps)
        _overwritten = dep_set.intersection(comp_key)
        if _overwritten:
            raise ComputationError(
                'Hierarchy cannot overwrite columns: %s' % _overwritten)

        if comp_key in self.key_comp_dict:
            logger.error('computation already exist:%s, which is %s' % (
                comp_key, to_jsonlike(self.key_comp_dict[comp_key])))
            #logger.info(pformat(to_jsonlike(self)))
            raise ComputationError('computation already exist:%s' % comp_key)
            
        for col in _info.to:
            if col in self.col_key_dict:
                raise ComputationError('column %s already exists' % col)
            self.col_key_dict[col] = comp_key
        
        self.key_comp_dict[comp_key] = RHierComp(_info, comp)

    def get_computation(self, by_col=None, by_comp_key=None):
        if by_col is not None and by_comp_key is None:
            comp_key = self.col_key_dict.get(by_col)
            if comp_key is None: 
                return (None, None)
            return comp_key, self.key_comp_dict[comp_key]

        elif by_comp_key is not None and by_col is None:
            comp_key = make_computation_key(by_comp_key)
            hier_comp = self.key_comp_dict.get(comp_key)
            if hier_comp is None: 
                return (None, None)
            else: 
                return (comp_key, hier_comp)

        else:
            raise ComputationError('cannot use comp_key and col together')

    def __contains__(self, col):
        key, _ = self.get_computation(by_col=col)
        return key is not None

    def resolve(self, cols, df=None):
        '''
        for a given list of column names, resolve the hierarchical dependency
        of the computation and return a list of computation sequence whose
        functions are to be performed to obtain the column names

        df provides a hint of columns that exist and need no calculation
        '''
        
        # pprint(('data=', self.data))
        # pprint(('cols=', rcols))

        def _get_adjacent_list(hier, col):
            _, hier_comp = hier.get_computation(by_col=col)
            if hier_comp is None: return []
            return hier_comp.info.deps

        # find out the topological order for non-agg computations
        # group all the rest of the agg computations together as well
        topo_order = topological_order(
            self, _get_adjacent_list, cols, 
            source_first=False, include_connection=True)

        # separate non-agg and agg computations
        non_agg_computation_order, _non_agg_computation_set = [], set()
        agg_computation_set = set()

        # print(topo_order)
        # print('hier to jsonlike', to_jsonlike(self))
        for col, _ in topo_order:
            comp_key, hier_comp = self.get_computation(col)
            if comp_key is None: # this col does not require computation
                continue

            info, _ = hier_comp
            if info.isagg:
                agg_computation_set.add(comp_key)
            else: # non-agg case
                if comp_key in _non_agg_computation_set:
                    continue

                # when df is provided as a hint, non_agg_computatoin that
                # would generate a column that's already in df would be ignored
                # print('rcomp', rcomp)
                if df is not None:
                    _to0_exist = info.to[0] in df.columns
                    # print('to0 exist', _to0_exist)
                    # NOTE if one columne exists, its co-columne should too
                    for _to in info.to[1:]:
                        assert _to0_exist == (_to in df.columns)

                    if _to0_exist:
                        continue

                non_agg_computation_order.append(comp_key)
                _non_agg_computation_set.add(comp_key)

        return ComputationSequence(
            non_agg=non_agg_computation_order,
            agg=agg_computation_set)

    def _collect_computations(self, comp_keys):
        ret = []
        for ck in comp_keys:
            key, hier_comp = self.get_computation(by_comp_key=ck)
            if key is not None:
                ret.append(hier_comp.func)
        return ret
  
    def run(self, rquery, df):
        comp_res = self.resolve(rquery.resolvable_cols, df)
        #logger.info(pformat(to_jsonlike(rquery)))
        #logger.info(pformat(comp_res))
    
        # NOTE verificating whether df has all external dependency columns
        # here is actually counter-productive. 

        # When RQuery is used in sub report node, the data source might not
        # have all the external dependencies it requires to perform the 
        # calculation if the calculation were on the original source, however
        # it could already have all the intermediate results calculated, so
        # computation sequence can be run without an issue
        _sequence = []

        # NOTE filters are applied as aggressively as possible.
        # That means if the dataframe contains columns to perform certain
        # filters, the filters would be applied. They won't be re-applied.
        initial_filters, post_non_agg_filters = rquery.batch_row_filters(df)

        # filters then non agg computation, then filters
        _sequence.extend(initial_filters)
        _sequence.extend(self._collect_computations(comp_res.non_agg))
        _sequence.extend(post_non_agg_filters)

        # Aggregation/grouping
        if rquery.is_agg:
            # as grouping is the last step. verify everything being grouped
            # together with the key can cover the select clause from rquery
            # however it is only needed when columns are select
            if not rquery.is_selecting_all:
                select_set = to_set(rquery.select_columns)
                group_set = to_set(rquery.group_columns)
                for ck in comp_res.agg:
                    group_set.update(to_set(ck))

                if not group_set.issuperset(select_set):
                    raise ComputationError(
                        'selecting %s, but %s not in grouping set' % (
                            select_set, select_set-group_set))

            _sequence.append(
                RAggregate(
                    by=rquery._groupby, 
                    aggs=self._collect_computations(comp_res.agg)
                )
            )

        if not rquery.is_selecting_all:
            _sequence.extend([
                RSelect(rquery.select_columns), # reselect columns
                RMultiRename(rquery.renamed_columns), # rename if needed
            ])

        if rquery._sortby:
            _sequence.append(rquery._sortby)

        logger.debug(pformat(to_jsonlike(_sequence)))
        return transform_dataframe(_sequence, df)

        
@to_jsonlike.register(Hierarchy)
def _(x):

    def _key_to_str(comp_key):
        return '|'.join(sorted(comp_key))

    def _get_comp(hier_comp):
        _, comp = hier_comp
        return comp

    return to_jsonlike({
        'col_to_key': map_over_dict_value(_key_to_str, x.col_key_dict),
        'key_to_comp': map_over_dict(_key_to_str, _get_comp, x.key_comp_dict),
    })

