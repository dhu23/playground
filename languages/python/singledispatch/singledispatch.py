import functools
from collections import namedtuple

Position = namedtuple('Position', 'x y')


def _run(x, **kwargs):
    try:
        run(x, **kwargs)
    except Exception as e:
        print('encountered error:%s' % str(e))

# class Test(object):

@functools.singledispatch
def run(x, **kwargs):
    raise RuntimeError('not impl')

@run.register(int)
def _(x):
    print('received int x:%d' % x)
    run((x, 1))

@run.register(tuple)
def _(x):
    print('received general tuple %s' % ((x,)))

@run.register(Position)
def _(x, **kwargs):
    print('kwargs:%s' % kwargs)
    print('received position namedtuple %s' % ((x,)))
    print('need coordinate type')
    print('type is %s' % kwargs['coortype'])

@run.register(list)
def _(x):
    print('got a list, try for each')
    for each in x:
        print('trying for ', each)
        run(each)

def nothing(x):
    print('is NONE')

run.register(type(None), nothing)

@run.register(dict)
def _(x):
    print('got a dict, try mapping values:%s' % x)


if __name__ == '__main__':
    # t = Test()

    _run(5)
    _run(5.5)
    _run(Position(5, 5), coortype='polar')
    _run((3, 2, 1))
    _run([(3, 2, 1), 5, 5.5])
    _run(None)
