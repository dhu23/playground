class Base(object):
    def __init__(self):
        print('running Base.__init__')
        super(Base, self).__init__()
        print('Base.__init__ done')


class First(Base):
    def __init__(self):
        print('running First.__init__')
        super(First, self).__init__()
        print('First.__init__ done')


class Second(Base):
    def __init__(self):
        print('running Second.__init__')
        super(Second, self).__init__()
        print('Second.__init__ done')


class Third(First):
    def __init__(self):
        print('running Third.__init__')
        super(Third, self).__init__()
        print('Third.__init__ done')


class Fourth(Second, Third):
    def __init__(self):
        print('running Fourth.__init__')
        super(Fourth, self).__init__()
        print('Fourth.__init__ done')

        #print(super(Fourth, self))
        #print(super(Fourth, self).__init__)


class Fifth(object):
    def __init__(self):
        print('running Fifth.__init__')
        super(Fifth, self).__init__()
        print('Fifth.__init__ done')


class Sixth(Fourth, Fifth):
    def __init__(self):
        print('running Sixth.__init__')
        super(Sixth, self).__init__()
        print('Sixth.__init__ done')


class A(object):
    def __init__(self, a):
        self.a = a

class B(A):
    def __init__(self, b, **kwargs):
        super(B, self).__init__(**kwargs)
        self.b = b


if __name__ == '__main__':
    sixth = Sixth()
    bobj = B(a=1, b=2)
