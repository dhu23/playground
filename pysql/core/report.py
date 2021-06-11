from collections import namedtuple, UserDict
from string import Template
import functools
import os
import sys
import traceback
import logging
import shutil
import csv

from pandas import read_pickle, concat


from core.computation import (
    transform_dataframe,
    RQuery,
    RRowFilter,
    RColumnFilter,
    RRowAdjust,
    RColumnAdjust,
    RColumnAdd,
)

from core.data import (
    pretty_srcdt,
    pretty_srcdt2,
    pretty_date,
    AcquisitionKey, 
    acquisition_key_to_str, 
    acquisition_key_from_string,
    make_acquisition_key,
    fetch,
)

from core.util import (
    make_directory,
    map_over_dict_value,
    to_jsonlike, 
    to_list,
    Context,
)

logger = logging.getLogger(__name__)


@functools.singledispatch
def make_report_context(
    x, hierarchy_name, report_spec_name, asof, **kwargs
):
    raise RuntimeError('cannot make report context from %s' % type(x))


@make_report_context.register(AcquisitionKey)
def _(x, hierarchy_name, report_spec_name, asof, **kwargs
):
    _kwargs = dict(kwargs)
    _kwargs.update({
        'acquisition_key': x,
        'hierarchy_name': hierarchy_name,
        'report_spec_name': report_spec_name,
        'srcdt': x.srcdt, 
        'pretty_srcdt': pretty_srcdt(x.srcdt),
        'pretty_srcdt2': pretty_srcdt2(x.srcdt),
        'pretty_date': pretty_date(x.srcdt),
        'env': x.env
    })
    return Context(asof, **_kwargs)


@make_report_context.register(list)
def _(x, hierarchy_name, report_spec_name, asof, **kwargs):
    _kwargs = dict(kwargs)
    _srcdts = [each.srcdt for each in x]
    _kwargs.update({
        'acquisition_key': x,
        'hierarchy_name': hierarchy_name,
        'report_spec_name': report_spec_name,
        'srcdt': _srcdts,
        'pretty_srcdt': pretty_srcdt(_srcdts),
        'pretty_srcdt2': pretty_srcdt2(_srcdts),
        'pretty_date': pretty_date(_srcdts)
    })
    return Context(asof, **_kwargs)

## -------------------- contextual string generator --------------------
@functools.singledispatch
def make_str(x, context): # by default x behaves like a callable
    if not callable(x):
        raise RuntimeError('%s is not callable to make string' % type(x))
    return x(context)


@make_str.register(Template)
def _(x, context):
    return x.safe_substitute(to_jsonlike(context))


@make_str.register(str)
def _(x, context):
    return x


################# Contextual DataFrame transforming types ####################
CtxtRowFilter = namedtuple('CtxtRowFilter', ['name', 'ctxtfunc'])
CtxtColumnFilter = namedtuple('CtxtColumnFilter', ['column', 'ctxtfunc'])
CtxtRowAdjust = namedtuple('CtxtRowAdjust', ['name', 'ctxtfunc', 'deps'])
CtxtColumnAdjust = namedtuple('CtxtColumnAdjust', ['name', 'ctxtfunc', 'deps'])
CtxtColumnAdd = namedtuple('CtxtColumnAdd', ['name', 'ctxtvaluefunc', 'loc'])


@to_jsonlike.register(CtxtRowFilter)
def _(x):
    return {
        'transformation_type': 'CtxtRowFilter',
        'name': to_list(x.name),
        'ctxtfunc': x.ctxtfunc.__qualname__
    }


@to_jsonlike.register(CtxtColumnFilter)
def _(x):
    return {
        'transformation_type': 'CtxtColumnFilter', 
        'column': x.column,
        'func': x.ctxtfunc.__qualname__
    }


@to_jsonlike.register(CtxtRowAdjust)
def _(x):
    return {
        'transformation_type': 'CtxtRowAdjust',
        'name': to_jsonlike(x.name),
        'func': x.ctxtfunc.__qualname__,
        'deps': to_jsonlike(x.deps)
    }


@to_jsonlike.register(CtxtColumnAdjust)
def _(x):
    return {
        'transformation_type': 'CtxtRowAdjust',
        'name': to_jsonlike(x.name),
        'func': x.ctxtfunc.__qualname__,
        'deps': to_jsonlike(x.deps)
    }


@to_jsonlike.register(CtxtColumnAdd)
def _(x):
    return {
        'transformation_type': 'CtxtColumnAdd',
        'name': x.name,
        'ctxtvaluefunc': x.ctxtvaluefunc.__qualname__,
        'loc': x.loc
    }


######## run_task: run DataFrame Transforming function in the reporter #######

@functools.singledispatch
def run_task(x, df, context, reporter):
    '''by default it assume the task is of the standard computation type'''
    return transform_dataframe(x, df) # context and reporter are ignored


@run_task.register(RQuery)
def _(x, df, context, reporter):
    hier = reporter.get_hierarchy(context.hierarchy_name)
    return hier.run(x, df)


@run_task.register(CtxtRowFilter)
def _(x, df, context, reporter):
    y = RRowFilter(x.name, x.ctxtfunc(context))
    return transform_dataframe(y, df)


@run_task.register(CtxtColumnFilter)
def _(x, df, context, reporter):
    y = RColumnFilter(x.column, x.ctxtfunc(context))
    return transform_dataframe(y, df)


@run_task.register(CtxtRowAdjust)
def _(x, df, context, reporter):
    y = RRowAdjust(x.name, x.ctxtfunc(context), x.deps)
    return transform_dataframe(y, df)


@run_task.register(CtxtColumnAdjust)
def _(x, df, context, reporter):
    y = RColumnAdjust(x.name, x.ctxtfunc(context), x.deps)
    return transform_dataframe(y, df)


@run_task.register(CtxtColumnAdd)
def _(x, df, context, reporter):
    y = RColumnAdd(x.name, x.ctxtvaluefunc(context), x.loc)
    return transform_dataframe(y, df)


DataCache = namedtuple('DataCache', ['key', 'location', 'data'])


@to_jsonlike.register(DataCache)
def _(x):
    return {
        'key': to_jsonlike(x.key),
        'location': to_jsonlike(x.location),
    }


## ---------------- Polymorphic on acquisition key/key list -----------------
@functools.singledispatch
def acquire_data(x, reporter, columns, cache_only): # returns DataCache
    raise RuntimeError('cannot acquire data through %s' % type(x))


@acquire_data.register(list)
def _(x, reporter, columns, cache_only):
    key_list, loc_list, data_list = [], [], []
    for each_key in x:
        each_cache = acquire_data(each_key, reporter, columns, cache_only)
        key_list.append(each_cache.key)
        loc_list.append(each_cache.location)
        data_list.append(each_cache.data)
    return DataCache(
        key=key_list, 
        location=loc_list, 
        data=concat(data_list, ignore_index=True)
    )


# ROutput can be used as the output field of a ReportNode object.
# this field normally would be a string, as the name of the output, but
# for certain usecases, it carries an indicator to use csv.QUOTE_NONE
ROutput = namedtuple('ROutput', ['name', 'noquoting'])

@functools.singledispatch
def _to_routput(x):
    raise RuntimeError('cannot make ROutput object from %s' % type(x))


@_to_routput.register(str)
def _(x):
    return ROutput(name=x, noquoting=False)


@_to_routput.register(ROutput)
def _(x):
    return x


ROutputData = namedtuple('ROutputData', ['data'])
ROutputError = namedtuple('ROutputError', ['error'])


@functools.singledispatch
def make_jsonlike_output(x, routput):
    raise RuntimeError('cannot make jsonlike output from %s' % type(x))


@make_jsonlike_output.register(ROutputData)
def _(x, routput):
    if routput.noquoting:
        csv_data = x.data.to_csv(index=False, quoting=csv.QUOTE_NONE)
    else:
        csv_data = x.data.to_csv(index=False)

    return {
        routput.name : {
            'csv': csv_data,
            'shape': x.data.shape
        }
    }


@make_jsonlike_output.register(ROutputError)
def _(x, routput):
    return { 
        routput.name : {'error': x.error },
    }


ReportData = namedtuple('ReportData', ['cache', 'reports'])


@to_jsonlike.register(ReportData)
def _(x):
    return {
        'cache': {
            'data_key': to_jsonlike(x.cache.key),
            'persistence_loc': to_jsonlike(x.cache.location),
        },
        'reports': map_over_dict_value(to_jsonlike, x.reports),
    }

# recursive report node
# tasks: a list of contextual and non-contextual dataframe transforming tasks
# output: a nullable output name
# nodes: a list of ReportNode for further calculations
ReportNode = namedtuple('ReportNode', ['tasks', 'output', 'subnodes'])


@to_jsonlike.register(ReportNode)
def _(x):
    return {
        'tasks': to_jsonlike(x.tasks),
        'output': to_jsonlike(x.output),
        'subnodes': to_jsonlike(x.subnodes),
    }


# TODO make data model smarter
# 1) node is a ReportSpec object
# 2) combine specifies extra step to merge outputs at row level
ReportSpec = namedtuple('ReportSpec', ['node', 'combine'])


def _resolve_mincolumns(report_spec):
    return None # for now as we are not using Smarter DataModel yet


@to_jsonlike.register(ReportSpec)
def _(x):
    return {
        'node': to_jsonlike(x.node),
        'combine': to_jsonlike(x.combine),
    }


################################## Reporter ##################################
class Reporter(object):
    def __init__(
        self,
        acquisitions, 
        hierarchies, 
        specs, 
        persistence_root=None,
    ):

        logger.info('starting reporter')
        self.acq_dict = acquisitions # acquisition name -> Acquisition obj
        self.hier_dict = hierarchies # hierarchy name -> Hierarchy obj
        self.report_spec_dict = specs # report spec name -> ReportSpec obj

        self.persistence_root = persistence_root
        self.load_cache()

    def load_cache(self):
        self._data_cache = {} # from Acquisition key to data location

        if self.persistence_root:
            make_directory(self.persistence_root)
            
            for each_file in os.listdir(self.persistence_root):
                full_path = os.path.join(self.persistence_root, each_file)
                if os.path.isdir(full_path):
                    logger.info('skipping directory %s' % full_path)
                    continue

                logger.info('loading file=%s' % full_path)
                try:
                    ak = acquisition_key_from_string(each_file)
                    logger.info('reading %s' % to_jsonlike(ak))
                    self._data_cache[ak] = DataCache(
                        key=ak, 
                        location=full_path,
                        data=read_pickle(full_path)
                    )
                    logger.info('file %s loaded successfully' % each_file)
                except Exception as e:
                    logger.exception('Loading ERROR:', str(e), '...skipping...')

            logger.warning('finished acquisition cache loading')
        else:
            logger.warning('no persistence root specified')

    def expire_cache(self, acq_key):
        _cached = self._data_cache.get(acq_key)
        acq_key_str = acquisition_key_to_str(acq_key)

        if _cached is None:
            raise RuntimeError('cannot expire %s, not found' % acq_key_str)

        if self.persistence_root:
            expired_dir = os.path.join(self.persistence_root, 'expired')
            make_directory(expired_dir)

            _expired_full_path = shutil.move(
                _cached.location, os.path.join(expired_dir, acq_key_str))
            logger.info(
                'moved %s to %s' % (_cached.location, _expired_full_path))
        else:
            _expired_full_path = None

        del self._data_cache[acq_key]
        return DataCache(key=acq_key, location=_expired_full_path, data=None)

    def get_acquisition(self, acquisition_name):
        acq = self.acq_dict.get(acquisition_name)
        if acq is None:
            raise RuntimeError('unknown source name %s' % acquisition_name)
        return acq
    
    def get_hierarchy(self, hierarchy_name):
        hier = self.hier_dict.get(hierarchy_name)
        if hier is None:
            raise RuntimeError('unknown hierarchy name %s' % hierarchy_name)
        return hier

    def get_report_spec(self, report_spec_name):
        report_spec = self.report_spec_dict.get(report_spec_name)
        if report_spec is None:
            raise RuntimeError('unknown report name %s' % report_spec_name)
        return report_spec

    @property
    def cache(self): 
        return self._data_cache
    
    @property
    def acquisitions(self): 
        return self.acq_dict

    @property
    def hierarchies(self): 
        return self.hier_dict

    @property
    def reportspecs(self): 
        return self.report_spec_dict
    
    def acquire(self, acq_key, columns, cache_only=False):
        '''
        if cache_only is set to True, it will not try to fetch data from
        backend on a cache-miss.
        '''
        _cached = self._data_cache.get(acq_key)
        acq_key_str = acquisition_key_to_str(acq_key)

        if _cached is not None:
            logger.info('found the exact match %s' % acq_key_str)
            return _cached

        # search a covering acq_key 
        # NOTE for now do not allow covering key search, as it might find a
        # superset of what is needed. And right now the ReportSpec does not
        # have contextual time filter built. Tabular structure should be reduced
        # to just ReportSpec and files dictionary, if this covering acq_key were
        # to be re-enabled. The new ReportSpec shall include AcquisitionKeyGen
        #for _ak, _dc in self._data_cache.items():
        #    if _ak.acquisition != acq_key.acquisition: 
        #        continue
        #    if _ak.env != acq_key.env: 
        #        continue
        #    if _ak.srcdt.cover(acq_key.srcdt):
        #        logger.debug('found a covering key: %s' % acquisition_key_to_str(_ak))
        #        return _dc

        if cache_only:
            raise RuntimeError('cannot find key:%s' % acq_key_str)
            
        # fetch data from the backend/cache
        logger.warning('acquiring data for key:%s' % acq_key_str)
        _acq = self.get_acquisition(acq_key.acquisition)
        queries = _acq.querygen.resolve(columns) 

        data = fetch(queries, _acq.fetchergen(), acq_key.env, acq_key.srcdt)
        if self.persistence_root:
            try:
                file_name = acquisition_key_to_str(acq_key)
                full_path = os.path.join(self.persistence_root, file_name)
                data.to_pickle(full_path)
                logger.warning('saved data cache as %s' % full_path)
            except Exception as e:
                logger.exception('saving data cache failed:%s' % str(e))
                full_path = None
        else:
            full_path = None

        _data_cache = DataCache(key=acq_key, location=full_path, data=data)
        self._data_cache[acq_key] = _data_cache
        return _data_cache

    def traverse(self, context, df, report_node, parent_error=None):
        # TODO write in an iterative way likee in data.py and computation.py?

        _df = df.copy()
        outputs = {}

        # NOTE if one of its parent levels already encountered error, skip this
        # otherwise, try to run the task. On error, pass the error downwards
        if parent_error:
            _output = ROutputError(error='(parent error) %s' % parent_error)
        else:
            try:
                # Run the sequence of tasks, contextual and non-contextual
                for _task in to_list(report_node.tasks):
                    _df = run_task(_task, _df, context, self)

                err = None
                _output = ROutputData(data=_df)
            except Exception as e:
                err = 'encountered exception while running tasks:%s' % str(e)
                logger.exception(err)
                logger.exception(
                    ''.join(traceback.format_exception(*sys.exc_info()))
                )
                _output = ROutputError(error=err)
            
        if report_node.output:
            outputs.update(
                make_jsonlike_output(_output, _to_routput(report_node.output)))

        logger.debug('finished for node=%s' % repr(report_node.output))
        _error = parent_error if parent_error else err
        if report_node.subnodes:
            for sub_node in report_node.subnodes:
                outputs.update(self.traverse(context, _df, sub_node, _error))

        return outputs

    def report(self, context, cache_only=False):
        _report_spec = self.get_report_spec(context.report_spec_name)
    
        # NOTE mincolumns is ignored if the data query generation is of direct
        # data query type. In data model type, mincolumns is used.
        # in the future, what columns need to be fetched can be computed
        # based on what type of queries are specified in the ReportSpec object
        data_cache = acquire_data(
            context.acquisition_key, 
            self, 
            _resolve_mincolumns(_report_spec),
            cache_only)

        df = data_cache.data
    
        return ReportData(
            cache=data_cache,
            reports=self.traverse(context, df, _report_spec.node, None)
        )


@acquire_data.register(AcquisitionKey)
def _(x, reporter, columns, cache_only):
    return reporter.acquire(x, columns, cache_only)


## ------------ control job specification related data structures -------------
# files is a list of Delivery objects
Tabular = namedtuple('Tabular', ['acq', 'hier', 'spec', 'files'])
TCA = namedtuple('TCA', ['env','saveas','table','tablename','methodids'])
Tabular_csv = namedtuple('Tabular_csv', ['acq','methodids'])

@functools.singledispatch
def create_analytics_context(x, asof):
    raise RuntimeError('cannot create analytical context from %s' % type(x))


@create_analytics_context.register(Tabular)
def _(x, asof):
    return make_report_context(
        make_acquisition_key(x.acq, asof), x.hier, x.spec, asof)

@create_analytics_context.register(Tabular_csv)
def _(x, asof):
    return Context(asof)

# archiving is right now at a common place without being configured
# 1) whether it is scheduled to run
# 2) trigger is the apscheduler cron trigger
# 3) analytics is of tabular analytics data type or tca type. It generates files
# 4) methods is a list of delivery methods
JobSpec = namedtuple('JobSpec', ['active', 'trigger', 'analytics', 'methods'])

# name is a str/template/contextual function that modifies the file name
# source chooses the file source, either as single source from analytics output
# or a combination source
# methodid is an int list that specifies the indices of the delivery methods
Delivery = namedtuple('Delivery', ['name', 'source', 'methodids'])
XlsxCombine = namedtuple('XlsxCombine', ['filelist', 'sheets'])
BreakDownDelivery = namedtuple('BreakDownDelivery', ['name', 'by', 'source', 'methodids'])

FileDrop = namedtuple('FileDrop', ['user', 'host', 'path'])
Email = namedtuple(
    'Email', 
    [
        'sender', 'receiver', 'cc', 'bcc', 
        'sub', 'msg', 'notradesub', 'notrademsg'
    ]
)
FTPSite = namedtuple('FTPSite', ['credential', 'host', 'cmd'])
