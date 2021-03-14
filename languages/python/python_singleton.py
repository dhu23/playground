# ways to achieve singleton in python

# method 1: using a decorator

def singleton_decorator(cls):
    _instances = {}
    def get_instance(): # would perfer no *args or **kwargs
        if cls not in _instances:
            _instances[cls] = cls()
        return _instances[cls]
    return get_instance

class MyClass1A(object):
    pass

@singleton_decorator
class MyClass1B(object):
    pass

mc1a_1 = MyClass1A()
mc1a_2 = MyClass1A()

print id(mc1a_1), id(mc1a_2)
assert id(mc1a_1) != id(mc1a_2)

mc1b_1 = MyClass1B()
mc1b_2 = MyClass1B()

print id(mc1b_1), id(mc1b_2)
assert id(mc1b_1) == id(mc1b_2)


# method 2: use a base class

class SingletonBase(object):
    _instance = None # to store the single instance that is tied to the class

    def __new__(cls): # without *args, **kwargs
        print 'SingletonBase.__new__', cls
        if not isinstance(cls._instance, cls):
            cls._instance = object.__new__(cls) # isn't the same as cls()
        return cls._instance

# class MyClass2A(SingletonBase, object): pass
# or:
MyClass2A = type("MyClass2A", (SingletonBase, object), {})

mc2a_1 = MyClass2A()
mc2a_2 = MyClass2A()

print id(mc2a_1), id(mc2a_2)
assert id(mc2a_1) == id(mc2a_2)

class MyClass2B(MyClass2A): pass

mc2b_1 = MyClass2B()
mc2b_2 = MyClass2B()

print id(mc2b_1), id(mc2b_2)

print MyClass2A._instance, id(MyClass2A._instance)
print MyClass2B._instance, id(MyClass2B._instance)

# method 3: metaclass

class SingletonMeta(type):
    _instances = {}
    def __call__(cls):
        print 'SingletonMeta.__call__', cls
        print 'super(SingletonMeta, cls)=', super(SingletonMeta, cls)
        if cls not in cls._instances:
            cls._instances[cls] = super(SingletonMeta, cls).__call__()
        return cls._instances[cls]

MyClass3A = SingletonMeta("MyClass3A", (object,), {})

mc3a_1 = MyClass3A()
mc3a_2 = MyClass3A()

print id(mc3a_1), id(mc3a_2)

class MyClass3B(object):
    __metaclass__ = SingletonMeta

mc3b_1 = MyClass3B()
mc3b_2 = MyClass3B()

print 'SingletonMeta instances contain: '
print SingletonMeta._instances, id(SingletonMeta._instances)
print MyClass3A._instances, id(MyClass3A._instances)
print MyClass3B._instances, id(MyClass3B._instances)
