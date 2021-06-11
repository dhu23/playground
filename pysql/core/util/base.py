'''
General util functions and higher order functions
'''

import enum
import operator
import math # for math related usage of make_function(), make_function0()
import datetime
import functools
from bisect import bisect
import os

###################### system related functions ###########################

def make_directory(dir_name):
    '''return True if the directory is created, otherwise it already exists'''
    if not os.path.isdir(dir_name):
        os.makedirs(dir_name, mode=0o777)
        assert os.path.isdir(dir_name)
        return True
    return False


def make_banner(msg, special='*'):
    assert len(msg) < 40, 'banner message way too long'
    total_space_len = 80-len(msg)-6 # starts with 3 special char, ends with 3
    if total_space_len%2 == 0:
        space_len1, space_len2 = total_space_len//2, total_space_len//2
    else:
        space_len1, space_len2 = total_space_len//2+1, total_space_len//2
    return '\n'.join([
        special*80,
        ''.join([
            '***',
            ' '*space_len1,
            msg,
            ' '*space_len2,
            '***',
        ]),
        special*80,
    ])


######################## utility functions for list #########################

def unique_list(s):
    '''make a unique list of the source iterable without change order'''
    temp = list(s)
    added = set()
    ret = []
    for x in temp:
        if x in added: continue
        ret.append(x)
        added.add(x)
    return ret

def list_to_str(l):
    lst = unique_list(l)
    string = '['
    for x in lst:
        string+=(x+',')
    string+=']'
    return string
    
def append_unique_list(x, l):
    if not x:
        return unique_list(l)
    else:
        return unique_list(x+l)


def remove_from_list(x, to_remove):
    ret = []
    remove_set = set(to_remove)
    for each in x:
        if each not in remove_set:
            ret.append(each)
    return ret


def map_over_dict_value(func, d):
    return dict((k, func(v)) for k, v in d.items())


def map_over_dict_key(func, d):
    return dict((func(k), v) for k, v in d.items())


def map_over_dict(keyfunc, valuefunc, d):
    '''map both key and value'''
    return dict((keyfunc(k), valuefunc(v)) for k, v in d.items())


def str_replace(old, new):
    def _inner(s):
        return s.replace(old, new)
    return _inner


def const(s):
    '''create a function that returns s all the time, ignoring input'''
    def _inner(x):
        return s
    return _inner


########################### for creating enum ##############################
def make_string_enum_class(
        enum_class_name, enums, 
        upper_case_enum=True, upper_case_value=True):
    assert enums, 'cannot make empty enum class'
    assert len(set(enums)) == len(enums), 'containing duplicate enum'
    def _str(upper_case):
        def _inner(s): return s.upper() if upper_case else s.lower()
        return _inner

    _enum_str = _str(upper_case_enum)
    _value_str = _str(upper_case_value)

    return enum.Enum(
        enum_class_name, 
        dict((_enum_str(e), _value_str(e)) for e in enums))


####################### higher order functions ###############################
def func_compose(*funcs):
    '''empty input yields the identity function'''
    def _in(x):
        ret = x
        for f in funcs:
            ret = f(ret)
        return ret
    return _in


def apply_binary2(f, value):
    '''partially apply the 2nd argument of a binary function, thus 2 suffix'''
    def _in(x): 
        return f(x, value)
    return _in


def logical_inverse(x):
    return not x


def _not(f):
    '''apply logical inverse to the given function'''
    return func_compose(f, logical_inverse)


## ------------------------ higher order filters -----------------------------
def equal_to(value): 
    return apply_binary2(operator.eq, value)
 

def different_from(value): 
    return apply_binary2(operator.ne, value)


def less_than(value, inclusive=False):
    if inclusive: 
        return apply_binary2(operator.le, value)
    else: 
        return apply_binary2(operator.lt, value)


def greater_than(value, inclusive=False):
    if inclusive: 
        return apply_binary2(operator.ge, value)
    else: 
        return apply_binary2(operator.gt, value)


def within(lower, upper, lower_inclusive=True, upper_inclusive=True):
    
    _lower_bound = greater_than(lower, lower_inclusive)
    _upper_bound = less_than(upper, upper_inclusive)

    def _inner_within(x): 
        return _lower_bound(x) and _upper_bound(x)
    return _inner_within


## ---------------------higher order filters for string ---------------------
def element_of(values):
    def _in(x): 
        return x in values
    return _in


def starting_with(prefix):
    def _in(x):
        try:
            return x.startswith(prefix)
        except:
            return False
    return _in


def ending_with(suffix):
    def _in(x): 
        try:
            return x.endswith(suffix)
        except:
            return False
    return _in


def containing(sub):
    def _in(x): 
        return sub in x
    return _in


def of_size(val):
    def _in(x):
        return len(x) == val
    return _in


def not_element_of(values):
    return _not(element_of(values))


def not_starting_with(prefix):
    return _not(starting_with(prefix))


def not_ending_with(suffix):
    return _not(ending_with(suffix))


def not_containing(sub):
    return _not(containing(sub))


def not_of_size(val):
    return _not(of_size(val))


## ------------------- higher order arithmetics --------------------------
def add_by(val):
    return apply_binary2(operator.add, val)


def sub_by(val):
    return apply_binary2(operator.sub, val)


def mul_by(val):
    return apply_binary2(operator.mul, val)


def div_by(val):
    return apply_binary2(operator.truediv, val)


# converting from string expression to function
def make_function(expression, arg_names):
    '''
    convert a string expression with its argument list to a python function.
    for example, make_function('a+b', ('a', 'b')) is equivalent to:
    def _func(a, b): return a+b

    f1 = make_function('a*b+c', ('a', 'b', 'c'))
    f2 = lambda a, b, c : a*b+c 

    both f1(2, 3, 4) and f2(2, 3, 4) give 10
    '''

    # math module is loaded here to support passing input like 'math.sqrt(a)'
    def _func(*args):
        return eval(
            expression, 
            globals(), 
            {**locals(), **dict(zip(to_list(arg_names), args))})
    return _func


def make_function0(expression, arg_names):
    '''form an equivalent lambda with better performance than make_function'''
    return eval('lambda ' + ','.join(arg_names) + ':' + expression)


############### Contextual higher order function generation #################

def contextual_equal_to(key, *args):
    '''args is a list of composable uary functions, for flexibility'''
    func = func_compose(*args)
    def _inner(context):
        return equal_to(func(getattr(context, key)))
    return _inner


def contextual_different_from(key, *args):
    '''args is a list of composable uary functions, for flexibility'''
    func = func_compose(*args)
    def _inner(context):
        return different_from(func(getattr(context, key)))
    return _inner


def contextual_less_than(key, inclusive=False, *args):
    '''args is a list of composable uary functions, for flexibility'''
    func = func_compose(*args)
    def _inner(context):
        return less_than(func(getattr(context, key)), inclusive)
    return _inner


def contextual_greater_than(key, inclusive=False, *args):
    '''args is a list of composable uary functions, for flexibility'''
    func = func_compose(*args)
    def _inner(context):
        return greater_than(func(getattr(context, key)), inclusive)
    return _inner


def contextual_within(
    lower_value, upper_value, lower_inclusive, upper_inclusive,
    lower_value_as_key=True, upper_value_as_key=True):

    def _inner(context):
        return within(
            context[lower_value] if lower_value_as_key else lower_value,
            context[upper_value] if upper_value_as_key else upper_value,
            lower_inclusive,
            upper_inclusive)
    return _inner


def contextual_element_of(key):
    def _inner(context):
        return element_of(getattr(context, key))
    return _inner


####################### Miscellaneous functions #############################
def parse_kvpairs(s, delim='=', kvdelim=';'):
    '''parse a string input into key value pairs.
    - delim separates a key from its value
    - kvdelim separates key value pairs'''

    def _parse_kv(kvstr):
        return tuple(kvstr.split(delim))

    return dict(map(_parse_kv, filter(lambda x: bool(x), s.split(kvdelim))))


def padding(s, size, pad=' ', left=True):
    '''
    padding a string to provided size. 
    If the string is already longer, do nothing
    by default it pads on the left side(front)
    '''

    slen = len(s)
    if slen >= size:
        return s

    extra = pad*(size-slen)
    if left:
        return extra+s
    else:
        return s+extra


######################## date/datetime related ##############################

def enumerate_dates(start, end, inclusive=True):
    assert start <= end
    ret = []
    dt = start
    while (dt <= end if inclusive else dt < end):
        ret.append(dt)
        dt += datetime.timedelta(days=1)
    return ret


@functools.singledispatch
def get_days_ago(d, days_ago): # d is assumed to be a datetime.date object
    return d - datetime.timedelta(days=days_ago)


@get_days_ago.register(datetime.datetime)
def _(d, days_ago):
    return get_days_ago(d.date(), days_ago)

@functools.singledispatch
def get_weekdays_ago(d, days_ago): #d is assumed to be a datetime.date object
    for i in range(days_ago):
        d = last_weekday(d)
    return d 

@get_weekdays_ago.register(datetime.datetime)
def _(d, days_ago):
    return get_weekdays_ago(d.date(), days_ago)

def past_date(days_ago):
    '''make a date in the past'''
    def _inner(dt): # d is a datetime.date or datetime.datetime object
        return get_days_ago(dt, days_ago)
    return _inner

def past_weekday(days_ago):
    '''make a weekday in the past'''
    def _inner(dt):
        return get_weekdays_ago(dt, days_ago)
    return _inner

@functools.singledispatch
def most_recent_monday(d): # d is a datetime.date object
    return d - datetime.timedelta(days=d.weekday())


@most_recent_monday.register(datetime.datetime)
def _(d):
    return most_recent_monday(d.date())


def past_day_of_week(dt, weekday, weeks_ago): # datetime.date or datetime.datetime
    '''
    weekday: uses python datetime convention, 0 is Monday.
    weeks_ago: 0 is this week, positive number means the past week.
    When weeks_ago is 0, it may generate a day in the past or the future
    '''
    assert weekday in range(0, 7), 'weekday has to be in interval [0, 6]'
    _monday = most_recent_monday(dt) - datetime.timedelta(weeks=weeks_ago)
    return _monday + datetime.timedelta(days=weekday)


@functools.singledispatch
def time_of(d, hour=0, minute=0, second=0): # d is datetime.date object
    return datetime.datetime(d.year, d.month, d.day, hour, minute, second)


@time_of.register(datetime.datetime)
def _(d, hour=0, minute=0, second=0):
    return time_of(d.date(), hour, minute, second)


@functools.singledispatch
def last_weekday(d): # d is a datetime.date object
    '''
    find the last weekday of a given day
    Sunday -> Friday, 2 days ago
    Saturday -> Friday, 1 day ago
    Friday -> Thursday, 1 day ago
    Thursday -> Wednesday, 1 day ago
    Wednesday -> Tuesday, 1 day ago
    Tuesday -> Monday, 1 day ago
    Monday -> Friday of previous week, 3 days ago
    '''
    if d.weekday() == 0:
        _ago = 3
    elif d.weekday() == 6:
        _ago = 2
    else:
        _ago = 1

    return d - datetime.timedelta(days=_ago)


@last_weekday.register(datetime.datetime)
def _(d):
    return last_weekday(d.date())

@functools.singledispatch
def next_weekday(d): # d is a datetime.date object
    '''
    find the next weekday of a given day
    Sunday -> Monday, 1 day after
    Saturday -> Monday, 2 days after
    Friday -> Monday, 3 days after
    Thursday -> Friday, 1 day after
    Wednesday -> Thrusday, 1 day after
    Tuesday -> Wednesday, 1 day after
    Monday -> Tuesday, 1 day after
    '''
    if d.weekday() == 4:
        _days = 3
    elif d.weekday() == 5:
        _days = 2
    else:
        _days = 1

    return d + datetime.timedelta(days=_days)


@next_weekday.register(datetime.datetime)
def _(d):
    return next_weekday(d.date())


@functools.singledispatch
def first_date_of_month(d): # datetime.date object
    return datetime.date(d.year, d.month, 1)


@first_date_of_month.register(datetime.datetime)
def _(d):
    return first_date_of_month(d.date())


def past_minutes_bin_of(dt, bin_size, bins_ago): # a datetime.datetime object
    '''
    bin_size indicates the number of minutes in a bin. 
    For 0 < bin_size < 60, it divides a full hour evenly into n-minute bins
    For bin_size >= 60, it attempts to divide time into n/60-hour bins.

    bins_ago: if it is 0, it returns the start of the current bin, and
    negative values indicate bins in the future.

    dt: the relative 

    examples:

    >> f = past_minute_bin(10, 1) # given a timestamp, returns the last bin
    >> dt1 = datetime.datetime(2020, 2, 20, 13, 5, 25)
    >> f(dt1) == datetime.datetime(2020, 2, 20, 12, 50, 0)  # True
    >> dt2 = datetime.datetime(2020, 2, 20, 13, 10, 25)
    >> f(dt2) == datetime.datetime(2020, 2, 20, 13, 0, 0) # True
    
    '''

    evenly_divide_hour = 0 < bin_size < 60 and 60%bin_size == 0
    multiplier_of_hour = bin_size >= 60 and bin_size%60 == 0
    assert any([
        evenly_divide_hour,
        multiplier_of_hour
    ]), 'invalid bin size number:%d' % bin_size

    if evenly_divide_hour:
        bin_slots = list(range(0, 60, bin_size))
        pos = bisect(bin_slots, dt.minute)-1 # 0 bin ago position
        # 0 bin ago datetime 
        dt0 = dt.replace(minute=bin_slots[pos], second=0, microsecond=0)
        return dt0 - datetime.timedelta(minutes=bins_ago*bin_size)

    elif multiplier_of_hour:
        bin_slots = list(range(0, 24, bin_size//60))
        pos = bisect(bin_slots, dt.hour)-1 # 0 bin ago position
        # 0 bin ago datetime
        dt0 = dt.replace(hour=bin_slots[pos], minute=0, second=0, microsecond=0)
        return dt0 - datetime.timedelta(hours=bins_ago*(bin_size//60))
        
    else:
        raise RuntimeError('this would not happen')


def past_minutes_bin(bin_size, bins_ago):
    def _inner(dt): # datetime.datetime object
        return past_minutes_bin_of(dt, bin_size, bins_ago)
    return _inner


def const_date(d): # datetime.date object
    '''no matter what the input is, always just return that date'''
    def _inner(_): # datetime.datetime or datetime.date object
        return d
    return _inner


########### to bridge bewteen list, set, frozen set, str and dict ############

## ----------------------------- to_list -----------------------------------
@functools.singledispatch
def to_list(x): 
    return list(x)


@to_list.register(str)
def _(x): 
    return [x]


@to_list.register(list)
def _(x): 
    return x


to_list.register(type(None), lambda x: [])

# ------------------------------- to_set -----------------------------------
@functools.singledispatch
def to_set(x):
    return set(x)


@to_set.register(str)
def _(x):
    return {x}


@to_set.register(set)
def _(x): 
    return x


to_set.register(type(None), lambda x: set())


# ---------------------------- to_frozenset --------------------------------
@functools.singledispatch
def to_frozenset(x):
    return frozenset(x)


@to_frozenset.register(str)
def _(x):
    return frozenset([x])


# ----------------------------- to_jsonlike --------------------------------
@functools.singledispatch
def to_jsonlike(x):
    return x


to_jsonlike.register(type(None), lambda x: None)


@to_jsonlike.register(list)
def _(x):
    return [to_jsonlike(each) for each in x]


# in certain cases, key itself is not jsonifiable
@to_jsonlike.register(dict)
def _(x):
    return dict((k, to_jsonlike(v)) for k, v in x.items())


@to_jsonlike.register(set)
def _(x): 
    return sorted(x)


@to_jsonlike.register(frozenset)
def _(x):
    return sorted(x)


############################# Context structure ##############################
class Context(object):

    def __init__(self, asof, **kwargs):
        self.asof = asof
        self.__dict__.update(kwargs)

    @property
    def asof_by_dot(self): 
        return datetime.datetime.strftime(self.asof, '%Y%m%d.%H%M%S')

    @property
    def asofyear(self): # yyyy as string
        return str(self.asof.year)

    @property
    def asofmonth(self): # mm as string
        return padding(str(self.asof.month), 2, pad='0')

    @property
    def asofday(self):
        return padding(str(self.asof.day), 2, pad='0')

    @property
    def asofhour(self):
        return padding(str(self.asof.hour), 2, pad='0')

    @property
    def asofminute(self):
        return padding(str(self.asof.minute), 2, pad='0')

    @property
    def asofsecond(self):
        return padding(str(self.asof.second), 2, pad='0')

    @property
    def asofdate(self):
        return datetime.datetime.strftime(self.asof, '%Y%m%d')

    @property
    def asofdate_dash(self):
        return datetime.datetime.strftime(self.asof, '%Y-%m-%d')

    @property
    def asofdate_dot(self):
        return datetime.datetime.strftime(self.asof, '%Y.%m.%d')

    @property
    def asoftime(self):
        return datetime.datetime.strftime(self.asof, '%H%M%S')

    @property
    def asoftime_colon(self):
        return datetime.datetime.strftime(self.asof, '%H:%M:%S')

    @property
    def lastmonth(self):
        last_of_prev_month = get_days_ago(first_date_of_month(self.asof), 1)
        return datetime.datetime.strftime(last_of_prev_month, '%B')

    @property
    def lastyearmonth_dash(self):
        last_of_prev_month = get_days_ago(first_date_of_month(self.asof), 1)
        return datetime.datetime.strftime(last_of_prev_month, '%Y-%m')

    @property
    def yearoflastmonth(self):
        last_of_prev_month = get_days_ago(first_date_of_month(self.asof), 1)
        return datetime.datetime.strftime(last_of_prev_month, '%Y')

    @property
    def prevweekday_dash(self):
        return datetime.datetime.strftime(last_weekday(self.asof), '%Y-%m-%d')
    
    @property
    def prevweekday_dot(self):
        return datetime.datetime.strftime(last_weekday(self.asof), '%Y.%m.%d')
    
    @property
    def prevweekday(self):
        return datetime.datetime.strftime(last_weekday(self.asof), '%Y%m%d')
    
    @property
    def nextweekday_dash(self):
       return datetime.datetime.strftime(next_weekday(self.asof), '%Y-%m-%d')
    
    @property
    def nextweekday_dot(self):
      return datetime.datetime.strftime(next_weekday(self.asof), '%Y.%m.%d')
      
    @property
    def nextweekday(self):
        return datetime.datetime.strftime(next_weekday(self.asof), '%Y%m%d')

    @property
    def recent30mbin(self):
        return datetime.datetime.strftime(
            past_minutes_bin_of(self.asof, 30, 0), '%Y.%m.%d-%H:%M:%S')

    @property
    def recent15mbin(self):
        return datetime.datetime.strftime(
            past_minutes_bin_of(self.asof, 15, 0), '%Y.%m.%d-%H:%M:%S')
        


@to_jsonlike.register(Context)
def _(x):
    ret = dict(x.__dict__)
    ret.update(
        dict(
            (field, getattr(x, field))
            for field in (
                'asof_by_dot',
                'asofyear', 'asofmonth', 'asofday', 
                'asofhour', 'asofminute', 'asofsecond',
                'asofdate', 'asofdate_dash', 'asofdate_dot',
                'asoftime', 'asoftime_colon',
                'lastmonth', 'lastyearmonth_dash', 'yearoflastmonth',
                'prevweekday_dash', 'prevweekday_dot', 'prevweekday',
                'nextweekday_dash', 'nextweekday_dot', 'nextweekday',
                'recent30mbin', 'recent15mbin',
            )
        )
    )
    return ret

