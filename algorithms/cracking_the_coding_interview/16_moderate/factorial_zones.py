# write an algorithm which computes the number of trailing zeros in n factorial

# considering there are way more 2's than 5's in the factor set
# number of 0 will be equal to number of factor 5

# from 1 to n,
# this is a multiplier of 5 for every 5 numbers starting from 5 -- #A
# there is a multiplier of 25 for every 25 numbers starting from 25 -- #B
# ....
# ret = #A + #B + ...

import math

# k=1, count multiplier of 5. k=2, 25
def count_multipler_of_five(n, k):
    return n // (5**k)

def count_zero(n):
    k = 1
    count = 0

    c = count_multipler_of_five(n, k)
    while c > 0:
        count += c
        k += 1
        c = count_multipler_of_five(n, k)
    return count

def count_trailing_zero(n):
    count = 0
    while n % 10 == 0:
        n = n // 10
        count += 1
    return count

if __name__ == "__main__":
    print(count_zero(10))
    def verify(n):
        print('verifying', n)
        if (count_zero(n) != count_trailing_zero(math.factorial(n))):
            print('bad', n)
        else:
            print('good', count_zero(n))

    for i in range(30):
        verify(i)
