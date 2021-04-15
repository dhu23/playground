# write a recursive function to multiply two positive integers without using
# the * operator. you can use addition, subtraction and bit shifting, but
# you should minimize the number of these operations

def recursive_multiply(a, b):
    assert a > 0 and b > 0
    if b > a: return recursive_multiply(b, a)
    if b == 1: return a
    if b == 2: return a+a
    
    odd_b = b & 1
    half_b = b >> 1
    
    x = recursive_multiply(a, half_b)
    return x+x+(a if odd_b else 0)


def iterative_multiply(a, b):
    assert a > 0 and b > 0
    if b > a: return iterative_multiply(b, a)
    if b == 1: return a
    if b == 2: return a+a
    
    table = []
    key, value = 1, a
    while key <= b:
        table.append((key, value))
        key += key
        value += value

    #print(table)
    i = len(table)-1

    s, remaining = 0, b
    while remaining > 0:
        while table[i][0] > remaining:
            i -= 1
        s += table[i][1]
        remaining -= table[i][0]

    #print(s)
    return s


if __name__ == "__main__":
    print(recursive_multiply(10, 20))
    print(iterative_multiply(10, 20))
