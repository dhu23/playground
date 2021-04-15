# a child is running up a staircase with n steps and can hop either 1 step,
# 2 steps, or 3 steps at a time. Implement a method to count how many possible
# ways the child can run up the stairs

# the following way is NOT the most efficient way of thinking of this problem

# n steps of stairs
# the child can jump up in steps provided by ways
# building such a table as the following that
#                     0  1  2  3  ... n-2  n-1  n
# 1step(only)         1  1  1  1        1    1  1
# 1,2 steps(only)     1  
# 1,2,3 steps(only)   1

import pprint
def jump_up_stairs(ways, n):
    ways.sort()
    table = [[None]*(n+1) for _ in ways]

    # init first column as 1
    for r in range(len(ways)):
        table[r][0] = 1

    # fill the first row
    for i in range(1, n+1):
        table[0][i] = 1 if i%ways[0] == 0 else 0

    for r in range(1, len(ways)):
        for i in range(1, n+1):
            # largest available step
            # tricky here:
            # how many ways to do it now? ways[r], ways[r-1], ... ways[0]
            # s = ways[r]
            table[r][i] = 0
            # table[r][i] = table[r-1][i]
            for s in ways[:r+1]:
                if i >= s:
                    table[r][i] += table[r][i-s]

    pprint.pprint(table)
    return table[-1][-1]

# this one internally incorporates the steps information. Not general
def naive(n):
    if n < 0: return 0
    elif n == 0: return 1
    else: return naive(n-1) + naive(n-2) + naive(n-3)

def naive_general(n, steps):
    if n < 0: return 0
    elif n == 0: return 1
    else: return sum(naive_general(n-s, steps) for s in steps)

# this can be made efficient through memorization
def general_dp(n, steps, mem):
    if n < 0:
        return 0
    elif n == 0:
        return 1
    elif n in mem:
        return mem[n]
    else:
        total = 0
        for s in steps:
            total += general_dp(n-s, steps, mem)
        mem[n] = total
        return total
    
# without memorization for [3, 2, 1] steps. The fastest so far
def triple123(n):
    if n < 0: return 0
    elif n == 0: return 1
    else:
        a, b, c = 0, 0, 1 # -2, -1, 0
        while n > 0:
            a, b, c = b, c, a+b+c
            n -= 1
        return c
    
if __name__ == "__main__":
    import time

    n = 20
    
    start = time.time()
    for i in range(n):
        print(naive(i))
    print('cost', time.time()-start)

    start = time.time()
    for i in range(n):
        print(naive_general(i, [3, 2, 1]))
    print('cost', time.time()-start)

    start = time.time()
    for i in range(n):
        print(general_dp(i, [2, 3, 1], {}))
    print('cost', time.time()-start)

    start = time.time()
    for i in range(n):
        print(triple123(i))
    print('cost', time.time()-start)
    
    #for i in range(1, 11):
    #    print(jump_up_stairs([1, 2, 3], i))
