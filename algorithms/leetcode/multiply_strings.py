# given two non-negative integers num1, and num2, represented as strings
# return the product

# there are only digits 0-9 and no leading zeros
# no built-in BigInteger lib or conversion to integers

# n is a string presenting an integer, and m is a 0-9 single digit

import itertools

def to_int(c):
    assert c in '0123456789'
    return ord(c)-ord('0')

def to_char(c):
    assert c in range(0, 10)
    return chr(c+ord('0'))

def multiply_by_single_digit(n, m):
    if m == '0':
        return '0'
    if m == '1':
        return n

    m_int = to_int(m)
    carry = 0

    ret = []
    for i in reversed(n):
        carry, single = divmod(to_int(i)*m_int+carry, 10)
        ret.append(to_char(single))
        
    if carry:
        ret.append(to_char(carry))
        
    return ''.join(reversed(ret))

# power = 1, is x10
# power = 2, is x100
def multiply_by_ten(n, power):
    if power == 0:
        return n
    assert power > 0
    return n+''.join(['0']*power)

def add(n1, n2):
    n1_len = len(n1)
    n2_len = len(n2)

    carry = 0
    ret = []
    for c1, c2 in itertools.zip_longest(reversed(n1), reversed(n2),
                                        fillvalue='0'):
        carry, single = divmod(to_int(c1)+to_int(c2)+carry, 10)
        ret.append(to_char(single))
        
    if carry:
        ret.append(to_char(carry))

    return ''.join(reversed(ret))

def multiply(n1, n2):

    ret = '0'
    for power, nc2 in enumerate(reversed(n2)):
        ret = add(ret, multiply_by_ten(multiply_by_single_digit(n1, nc2), power))
    return ret
        

if __name__ == "__main__":
    print(multiply_by_single_digit('12345', '9'))
    print(12345*9)

    print(add('43215', '59681'))
    print(43215+59681)

    print(multiply_by_ten('1234', 2))
    print(1234*10**2)

    print(multiply('12', '34'))
    print(12*34)

    print(multiply('121213515', '31215154'))
    print(int('121213515')*int('31215154'))

    
