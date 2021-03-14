class Magic:
    @property
    def __repr__(self):
        def inner():
            return "It works!"
        return inner

print repr(Magic())


# type(name, bases, dct)
# name - is a string giving the name of the class to be constructed
# bases - is a tuple giving the parent classes of the class to be constructed
# dct - is a dictionary of the attributes and methods of the class
# to be constructed

class Foo1(object):
    i = 4

class Bar1(Foo1):
    def get_i(self):
        return self.i

b1 = Bar1()
print b1.get_i()

Foo2 = type('Foo2', (), dict(i=4))
Bar2 = type('Bar2', (Foo2,), dict(get_i=lambda self: self.i))

b2 = Bar2()
print b2.get_i()
