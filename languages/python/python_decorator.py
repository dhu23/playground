def with_error_handling(fn):
    def _wrapped(*args, **kwargs):
        try:
            return fn(*args, **kwargs)
        except Exception as e:
            print('had error:', str(e))
            return None

    return _wrapped

@with_error_handling
def add0(a, b):
    return a+b


@with_error_handling
def div0(a, b):
    if b == 0:
        raise Exception('div by zero')
    return a // b


def do_twice(fn):
    def _inner(*args, **kwargs):
        fn(*args, **kwargs)
        fn(*args, **kwargs)

    return _inner



class Test(object):

    @staticmethod
    def try_again(fn):
        def _wrappee(self, *args, **kwargs):
            fn(self, *args, **kwargs)
            fn(self, *args, **kwargs)
        return _wrappee
    
    def error(self):
        print('has an error')

    @do_twice
    def foo(self):
        print('foo')

    def bar(self):
        print('bar')


if __name__ == '__main__':

    print(add0(1, 2))
    print(div0(6, 2))
    print(div0(4, 2))
    print(div0(2, 0))

    t = Test()
    t.foo()
    t.bar()
