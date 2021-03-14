#!/usr/bin/python

class Movie(object):
    def __init__(self, title, rating, runtime, budget, gross):
        # self._budget = None
        self.budget = budget

    @property
    def budget(self):
        return self._budget

    @budget.setter
    def budget(self, value):
        if value < 0:
            raise ValueError('Negative value not allowed: %s' % value)
        self._budget = value

m = Movie('Casablanca', 97, 102, 964000, 1300000)
print m.budget, m._budget
try:
    m.budget = -1
except ValueError:
    print 'Woops'

print m.budget, m._budget


from weakref import WeakKeyDictionary

class Price(object):
    def __init__(self):
        self.default = 0
        self.values = WeakKeyDictionary()

    def __get__(self, instance, owner):
        return self.values.get(instance, self.default)

    def __set__(self, instance, value):
        if value < 0 or value > 100:
            raise ValueError("Price must be between 0 and 100.")
        self.values[instance] = value

    def __delete__(self, instance):
        del self.values[instance]

# An instance of a descriptor must be added to a class as a class attribute
# not as an instance attribute. Therefore, to store different data for each
# instance, the descriptor needs to maintain a dictionary that maps instances
# to instance-specific values.

# A normal Python dictionary stores references to objects it uses as keys.
# Those references by themselves are enough to prevent the object from being
# garbage collected. To prevent Book instances from hanging around after
# we are finished with them, we use the WeakKeyDictionary. Once last strong
# reference to the instance passes away, the associated key-value pair will
# be discarded

class Book(object):
    price = Price()

    def __init__(self, author, title, price):
        self.author = author
        self.title = title
        self.price = price

    def __str__(self):
        return "{0} - {1}".format(self.author, self.title)

b = Book("William Faulkner", "The Sound and the Fury", 12)
print b.price

# An incorrect attempt of Price class
class BadPrice(object):
    def __init__(self):
        self.__price = 0

    def __get__(self, instance, owner):
        return self.__price

    def __set__(self, instance, value):
        if value < 0 or value > 100:
            raise ValueError("Price must be between 0 and 100.")
        self.__price = value

    def __delete__(self, instance):
        del self.__price


class BadBook(object):
    price = BadPrice()

    def __init__(self, author, title, price):
        self.author = author
        self.title = title
        self.price = price

    def __str__(self):
        return "{0} - {1}".format(self.author, self.title)
        
bad_b1 = BadBook("William Faulkner", "The Sound and the Fury", 12)
print bad_b1.price
bad_b2 = BadBook("John Dos Passos", "Manhattan Transfer", 13)
print bad_b1.price

# instead of defining a single class-level descriptor object that manages
# instance-specific values, property works by combining instance methods from
# the class.

class Publisher(object):
    def __init__(self, name):
        self.__name = name

    def get_name(self):
        print "getting name"
        return self.__name

    def set_name(self, value):
        print "setting name"
        self.__name = value

    def delete_name(self):
        print "deleting name"
        del self.__name

    name = property(get_name, set_name, delete_name, "publisher name")

p = Publisher("Faber & Faber")
print p.name
p.name = "Random House"
