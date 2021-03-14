import math

def catalan_exact(n):
    return (math.factorial(2*n) // math.factorial(n+1)) // math.factorial(n)

def catalan(n):
    assert n >= 0
    if n == 0: return 1
    if n == 1: return 1
    return sum(catalan(i)*catalan(n-1-i) for i in range(0, n))

if __name__ == "__main__":
    print(catalan_exact(5))
    print(catalan(5))
