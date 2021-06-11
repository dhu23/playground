from datetime import datetime, timedelta
from collections import namedtuple
import functools
import operator
import logging

from sklearn.linear_model import LinearRegression
import numpy as np
import pandas as pd


from core.util.base import (
    func_compose,
    remove_from_list,
    less_than, 
    greater_than,
    equal_to,
    to_jsonlike, 
    to_list,
    to_set
)

logger = logging.getLogger(__name__)

######################## timestamp/datetime util ############################
def make_datetime64(*args):
    return np.datetime64(datetime(*args))


def make_timestamp(*args):
    return pd.Timestamp(datetime(*args))


def time_after(tp, true_for_nat=False, inclusive=False):
    def _inner(x): # pd timestamp object
        if pd.isnull(x):
            return true_for_nat
        _after = greater_than(tp, inclusive)
        return _after(pd.to_datetime(x).time())
    return _inner


def time_before(tp, true_for_nat=True, inclusive=False):
    def _inner(x): # pd timestamp object
        if pd.isnull(x):
            return true_for_nat
        _before = less_than(tp, inclusive)
        return _before(pd.to_datetime(x).time())
    return _inner


def date_after(dt, true_for_nat=False, inclusive=False):
    def _inner(x): # pd timestamp object
        if pd.isnull(x):
            return true_for_nat
        _after = greater_than(dt, inclusive)
        return _after(pd.to_datetime(x).date())
    return _inner


def date_before(dt, true_for_nat=True, inclusive=False):
    def _inner(x): # pd timestamp object
        if pd.isnull(x):
            return true_for_nat
        _before = less_than(dt, inclusive)
        return _before(pd.to_datetime(x).date())
    return _inner

def date_equal(dt, true_for_nat=False):
    def _inner(x): #pd timestamp object
        if pd.isnull(x):
            return true_for_nat
        _equal = equal_to(dt)
        return  _equal(pd.to_datetime(x).date())
    return _inner

# contextual versions of the same higher-order
def contextual_time_after(key, true_for_nat=False, inclusive=False, *args):
    '''args is a list of composable uary functions, for flexibility'''
    func = func_compose(*args)
    def _inner(context):
        return time_after(func(getattr(context, key)), true_for_nat, inclusive)
    return _inner


def contextual_time_before(key, true_for_nat=True, inclusive=False, *args):
    '''args is a list of composable uary functions, for flexibility'''
    func = func_compose(*args)
    def _inner(context):
        return time_before(func(getattr(context, key)), true_for_nat, inclusive)
    return _inner


def contextual_date_after(key, true_for_nat=False, inclusive=False, *args):
    '''args is a list of composable uary functions, for flexibility'''
    func = func_compose(*args)
    def _inner(context):
        return date_after(func(getattr(context, key)), true_for_nat, inclusive)
    return _inner


def contextual_date_before(key, true_for_nat=True, inclusive=False, *args):
    '''args is a list of composable uary functions, for flexibility'''
    func = func_compose(*args)
    def _inner(context):
        return date_before(func(getattr(context, key)), true_for_nat, inclusive)
    return _inner

def contextual_days_ago(key, true_for_nat=False, *args):
    '''args is a list of composable uary functions, for flexibility'''
    func = func_compose(*args)
    def _inner(context):
        return date_equal(func(getattr(context,key)), true_for_nat)
    return _inner

def nlargest_unique(n):
    def _inner(s): # pd series
        return s.isin(s.sort_values().drop_duplicates().tail(n))
    return _inner


def nsmallest_unique(n):
    def _inner(s): # pd series
        return s.isin(s.sort_values().drop_duplicates().head(n))
    return _inner


def last_element(s): # pd series
    return s.iloc[-1]

def last_valid_element(s): # pd series
    return s[s != "0"].iloc[-1]

def first_element(s): # pd series
    return s.iloc[0]


def wavg(x, w):
    '''weighted average'''
    try:
        # use np.sum instead of python native sum to handle nan gracefully
        return np.sum(x*w)/np.sum(w)
    except ZeroDivisionError:
        return x.mean()


def wsum(x, w):
    '''weighted sum'''
    return sum(x*w)


def sum_ratio(x, y):
    '''the ratio of sum of x over sum of y'''
    return sum(x)/sum(y)


def concat_by(delim):
    def _inner(s): # pd series
        return delim.join(s)
    return _inner

#Linear regression
def lr(coeff):
    def _return_coeff(y,*argv):
        n = len([*argv])
        Y = y.values.reshape(-1,1)
        X = pd.concat([*argv]).values.reshape(-1,n)
        reg = LinearRegression().fit(X,Y)
        return reg.coef_[0][coeff]
    return _return_coeff
    
##################### data frame related functions ###########################

class DataFrameError(RuntimeError):
    pass


def assert_dataframe_columns(df, columns):
    '''columns can be str/list/tuple'''
    column_set = set(df.columns)
    check_set = to_set(columns)
    if not check_set.issubset(column_set):
        logger.debug('df does not have columns:%s\n%s' % (columns, repr(df)))
        raise DataFrameError(
            'cannot find columns=%s in df, df has %s' % (
                check_set-column_set, column_set)
        )

@functools.singledispatch
def series_to_dataframe(x, s):
    raise DataFrameError(
        'cannot understand dataframe columns as type %s' % type(x))


@series_to_dataframe.register(str)
def _(x, s): 
    return pd.DataFrame({x:s})


@series_to_dataframe.register(list)
def _(x, s): # s is a series of tuples with size matching len(x)
    def _extract(pos, name):
        '''extract position i from a series of tuples'''
        _s = s.apply(operator.itemgetter(pos))
        _s.name = name
        return _s
    
    return pd.concat([_extract(i, name) for i, name in enumerate(x)], axis=1)


@series_to_dataframe.register(tuple)
def _(x, s): return series_to_dataframe(list(x), s)


def concat_dataframes(df, right_df, overwrite=True):
    '''
    add columns from right_df to the left side, return df for better chaining
    If overwrite is True, duplicated columns on the right overwrite original
    otherwise, duplicated columns on the right are discarded
    '''

    if right_df is None: return df
    if df is None: return df

    for c in right_df.columns:
        if c in df:
            if not overwrite:
                continue
        df[c] = right_df[c]
    return df


@functools.singledispatch
def left_join_dataframes(x, left_df, right_df, dup_resolve=None):
    raise RuntimeError('cannot merge by type %s' % type(x))


@left_join_dataframes.register(list)
def _(x, left_df, right_df, dup_resolve=None):
    assert dup_resolve in (None, 'left', 'right')
    on_cols = x
    if dup_resolve is None:
        return pd.merge(left_df, right_df, on=on_cols, how='left')
    
    left_cols = left_df.columns.difference(on_cols)
    right_cols = right_df.columns.difference(on_cols)

    if dup_resolve == 'left':
        right_cols_to_use = right_cols.difference(left_cols).union(on_cols)
        return pd.merge(left_df, right_df[right_cols_to_use], on=on_cols, how='left')

    elif dup_resolve == 'right':
        left_cols_to_use = left_cols.difference(right_cols).union(on_cols)
        return pd.merge(left_df[left_cols_to_use], right_df, on=on_cols, how='left')

    raise DataFrameError('dup resolve:%s is not recognized' % dup_resolve)


JoinBy = namedtuple('JoinBy', ['left', 'right'])

@to_jsonlike.register(JoinBy)
def _(x):
    return {
        'joinby_left': to_list(x.left),
        'joinby_right': to_list(x.right)
    }


@left_join_dataframes.register(str)
def _(x, left_df, right_df, dup_resolve=None):
    return left_join_dataframes(to_list(x), left_df, right_df, dup_resolve)


@left_join_dataframes.register(set)
def _(x, left_df, right_df, dup_resolve=None):
    return left_join_dataframes(to_list(x), left_df, right_df, dup_resolve)


@left_join_dataframes.register(tuple)
def _(x, left_df, right_df, dup_resolve=None):
    return left_join_dataframes(to_list(x), left_df, right_df, dup_resolve)

@left_join_dataframes.register(JoinBy)
def _(x, left_df, right_df, dup_resolve=None):
    left_on_cols, right_on_cols = x
    left_on_cols = to_list(left_on_cols)
    right_on_cols = to_list(right_on_cols)

    if left_on_cols == right_on_cols:
        return left_join_dataframes(left_on_cols, left_df, right_df, dup_resolve)

    assert dup_resolve in (None, 'left', 'right')
    if dup_resolve is None:
        return pd.merge(
            left_df, right_df, 
            left_on=left_on_cols, right_on=right_on_cols, how='left')
    
    left_cols = left_df.columns.difference(left_on_cols)
    right_cols = right_df.columns.difference(right_on_cols)

    extra = [
        _r_on for _l_on, _r_on in zip(left_on_cols, right_on_cols) 
        if _l_on != _r_on
    ]

    if dup_resolve == 'left':
        right_cols_to_use = right_cols.difference(left_cols).union(right_on_cols)
        ret = pd.merge(
            left_df, right_df[right_cols_to_use],
            left_on=left_on_cols, right_on=right_on_cols, how='left')
        return ret.drop(columns=extra)

    elif dup_resolve == 'right':
        left_cols_to_use = left_cols.difference(right_cols).union(left_on_cols)
        ret = pd.merge(
            left_df[left_cols_to_use], right_df, 
            left_on=left_on_cols, right_on=right_on_cols, how='left')
        return ret.drop(columns=extra)

    raise DataFrameError('dup resolve:%s is not recognized' % dup_resolve)


def tuples_from_dataframe(columns):
    def _convert(df):
        def _run_func(row):
            return tuple(row[col] for col in columns)
        return df.apply(_run_func, axis=1)
    return _convert


def is_equal_series(s1, s2):

    if len(s1) != len(s2): return False
    
    is_float = lambda x : x.dtype in (np.dtype('float64'),)

    is_s1_float, is_s2_float = is_float(s1), is_float(s2)

    if is_s1_float and is_s2_float: 
        ret = np.allclose(s1, s2) # all(abs(s1-s2) < 0.000001)
    elif is_s1_float or is_s2_float: 
        ret = False
    else:
        ret = all(s1 == s2)

    if not ret:
        logger.debug("%s\n%s" % (repr(s1), repr(s2)))

    return ret


def is_equal_dataframe(df1, df2, check_order=True):
    
    def _is_same_axis(axis1, axis2, check_order):
        if axis1.dtype != axis2.dtype: return False

        if check_order:
            if len(axis1) != len(axis2): 
                return False
            return all(axis1 == axis2)
        else:
            return set(axis1) == set(axis2)


    if not _is_same_axis(df1.index, df2.index, True): 
        logger.debug('index not the same')
        return False

    if not _is_same_axis(df1.columns, df2.columns, check_order):
        logger.debug('columns not the same')
        logger.debug(df1.columns)
        logger.debug(df2.columns)
        return False

    ret = all(is_equal_series(df1[col], df2[col]) for col in df1.columns)
    return ret


def assert_dataframe(ut, df1, df2):
    ut.assertEqual(set(df1.columns), set(df2.columns))

    for _col in df1.columns:
        with ut.subTest(case=_col):
            # print(df1[_col])
            # print(df2[_col])
            ut.assertTrue(is_equal_series(df1[_col], df2[_col]))
            # ut.assertEqual(df1[_col].tolist(), df2[_col].tolist())
