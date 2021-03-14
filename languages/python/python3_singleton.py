

class SingletonBase(object):
    _instance = None

    def __new__(cls, *args, **kwargs):
        print('SingletonBase.__new__')
        if not isinstance(cls._instance, cls):
            cls._instance = object.__new__(cls)
        return cls._instance

    def __init__(self, *args, **kwargs):
        print('SingletonBase.__init__', args, kwargs)


# metaclass, replace self with cls
class SingletonMeta(type):
    _instance = {}

    # when the instance of SingletonMeta, which is a singleton class
    # is called, a.k.a., to create the singleton instance
    # cls is self, which is a concrete Singleton class
    def __call__(cls):
        if cls not in SingletonMeta._instance:
            # print(super(SingletonMeta))
            SingletonMeta._instance[cls] = super(SingletonMeta, cls).__call__()
            # cls() would cause infinite recursion
        return SingletonMeta._instance[cls]


Foo = SingletonMeta('Foo', (object,), {})
