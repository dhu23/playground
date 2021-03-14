def foo(x, y, optional1=True, optional2=False):
    print(x, y)
    print(optional1, optional2)


if __name__ == '__main__':
    foo(1, 2) # True, False,
    foo(1, 2, False) # False, False 
    foo(1, 3, False, True) # False, True
    foo(1, 3, optional2=True) # True, True
