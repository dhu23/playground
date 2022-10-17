import datetime
# n value    0 1 2 3 4 5 6 ...
# fib value  0 1 1 2 3 5 8 ...

def fibonacci_roll(n):
    if n == 0: return 0
    if n == 1: return 1
    a, b = 0, 1

    while n > 1:
        a, b = b, a+b
        n -= 1
    return b


# |f(n+1)|  =  | 1  1 |  | f(n)   |
# |f(n)  |     | 1  0 |  | f(n-1) |

# then we have
# |f(n+1)|  =  | 1  1 |^(n) | f(1) |
# |f(n)  |     | 1  0 |     | f(0) |

# so the question is to calculate the power of the characteristic matrix

# m is a tuple of 4 value (ma, mb, mc, md) for [[ma, mb], [mc, md]] repr
# | m1a  m1b |      | m2a  m2b |
# | m1c  m1d |  by  | m2c  m2d |
def matrix_mul2(m1, m2):
    m1a, m1b, m1c, m1d = m1
    m2a, m2b, m2c, m2d = m2
    ma = m1a*m2a + m1b*m2c
    mb = m1a*m2b + m1b*m2d
    mc = m1c*m2a + m1d*m2c
    md = m1c*m2b + m1d*m2d
    return (ma, mb, mc, md)

def matrix_pow2(m, n):
    if n == 0: return (1, 0, 0, 1)
    if n == 1: return m
    h = n // 2

    pow = matrix_pow2(m, h)
    pow2 = matrix_mul2(pow, pow)
    if n % 2 == 0:
        return pow2
    else:
        return matrix_mul2(pow2, m)

def fibonacci_binary(n):
    m = (1, 1, 1, 0)
    _, _, c, _ = matrix_pow2(m, n)
    return c


def fib_recursive(n):
    if n < 2: 
        return n
    return fib_recursive(n-1) + fib_recursive(n-2)


def test1():
    for i in range(10):
        print(i, fibonacci_roll(i), fibonacci_binary(i))
        
    for i in range(10):
        print(i, matrix_pow2((1, 1, 1, 0), i))

    now = datetime.datetime.now
    for i in range(50000):
        t1 = now()
        f1 = fibonacci_roll(i)
        t2 = now()
        f2 = fibonacci_binary(i)
        t3 = now()
        assert f1 == f2
        print(i, t2-t1, t3-t2)


def timeTest1():
    for _ in range(100):
        for i in range(1, 1000):
            fibonacci_roll(i)


def timeTest2(v):
    ret = 0
    for i in range(1, v):
        ret += fib_recursive(i)
    return ret


if __name__ == "__main__":
    print(timeTest2(40))
