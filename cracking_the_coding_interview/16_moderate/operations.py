# write methods to implement the multiply, subtract, and divide operation
# for integers. The results of all these are integers. And use only the add
# operator


import operator

def subtract(a, b):
    if a == b: return 0
    if a < b: return -subtract(b, a)
    # a > b and it is essentially a binary search problem of the result
    # in the range 0 to a

    lo, hi = 0, a
    while lo <= hi:
        mid = divide(lo + hi, 2) # can be replaced by right shift
        if b + mid > a:
            hi = mid
        elif b + mid < a:
            lo = mid
        else:
            return mid    
    
def subtract2(a, b):
    def negate(b):
        ret = 0
        
        compare = 1 if b >= 0 else -1
        op = -1 if b >= 0 else 1
        def is_good(b, num):
            assert (b >= 0) == (num >= 0)
            return b >= num if b >= 0 else b <= num
        
        # during the process of bring down(up) to 0, creat another number
        # 1, 2, 4, 8.... b for cases where b is positive
        # +1, +2, +4, +8, .... these are the compare numbers
        # -1, -2, -4, -8, .... these are the operation numbers

        stack = []
        while is_good(b, compare):
            stack.append((compare, op))
            compare += compare
            op += op

        # print(stack)
        while b != 0:
            # compare the last number with b
            assert stack
            # print(b, stack)
            while not is_good(b, stack[-1][0]):
                stack.pop()
            assert stack
            # print("2nd print" ,b, stack)
            
            b += stack[-1][1] # add op which is -compare
            ret += stack[-1][1]
            c += 1
            
        return ret

        
    return a+negate(b)

def multiply(a, b):
    if a == 0 or b == 0: return 0
    if a > 0 and b < 0: return multiply(a, -b)
    if a < 0 and b > 0: return multiply(-a, b)
    if a < 0 and b < 0: return multiply(-a, -b)

    # a, b are both positive numbers
    if a < b: return multiply(b, a)

    # add number a by b times. track
    # a*1, a*2, a*4, a*8....up to a*b' where b' < b
    m = [(1, a)]
    i = 2
    while i <= b:
        m.append((i, m[-1][1] + m[-1][1]))
        i = i + i
    acc, ret = 0, 0
    #print(m)
    while acc < b:
        assert m
        #print(m, acc)
        while m[-1][0] + acc > b:
            m.pop()
        acc += m[-1][0]
        ret += m[-1][1]
    assert acc == b
    return ret

def divide(a, b):
    assert a >= 0 and b >= 0
    if b == 0: return None
    if a == 0: return 0
    if b == 1: return a
    if a == b: return 1
    if a < b: return 0
    # now a > b, build data for the following
    # b*1, b*2, b*4, ...up to b*q where b*q <= a
    m = [(1, b)]
    i = 2
    prod = b + b
    while prod <= a:
        m.append((i, prod))
        i = i + i
        prod = prod + prod
    #print(m)

    q, acc = 0, 0
    while acc + b <= a:
        while acc + m[-1][1] > a:
            m.pop()
        assert m
        #print(m, acc, q)
        acc += m[-1][1]
        q += m[-1][0]
    return q


if __name__ == "__main__":
    print(multiply(5, 7), multiply(121, 3879), multiply(10, 8))
    print(divide(100, 20), divide(125, 25), divide(26, 4))
    print(subtract(9, 1))
    print(subtract2(100, 34))
    print(subtract2(-100, -34))
    print(subtract2(100, -34))
