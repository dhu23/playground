from singledispatch import run, _run
from collections import UserDict

class Color(object):
    def __init__(self, r, g, b):
        self.r, self.g, self.b = r, g, b

@run.register(Color)
def _(x, **kwargs):
    print('running Color:%d %d %d' % (x.r, x.g, x.b))


class FancyColor(Color):
    def __init__(self, r, g, b):
        self.r, self.g, self.b = 2*r, 2*g, 2*b


@run.register(UserDict)
def _(x):
    print('is a user dict')


class FancyUserDict(UserDict):
    pass

# without this one, FancyColor uses Color's run function
@run.register(FancyColor)
def _(x, **kwargs):
    print('fancy color')

if __name__ == '__main__':

    _run(Color(1, 2, 3))
    _run(FancyColor(2, 3, 4))
    _run([Color(1, 2, 3), 5, 10, FancyColor(11, 22, 33)])
    _run(FancyUserDict({1:2, 3:4}))
