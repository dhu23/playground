# divide and conquer
def expo1(x, n):
    if n == 0:
        return 1, 0

    if n == 1:
        return x, 0

    if n < 0:
        val, count = expo1(x, -n)
        return 1.0/val, count

    half, remainder = divmod(n, 2)
    ret, count = expo1(x, half)
    if remainder == 0:
        return ret*ret, count+1
    else:
        return ret*ret*x, count+2

# use binary representation of n
# for example 2**100 = 2**64 * 2**32 * 2**4
# because 100 = 64+32+4
# map contains:
# idx  power        val
# 0    2**0 = 1     x**1
# 1    2**1 = 2     x**2
# 2    2**2 = 4     x**4
# 3    2**3 = 8     x**8
def build_cache_map(x, n):
    assert n >= 1
    
    m = {}
    power, m[1] = 2, x
    count = 0
    while power <= n:
        m[power] = m[power/2]*m[power/2]
        power *= 2
        count += 2
        
    return m, power/2, count

def expo2(x, n):
    if n == 0:
        return 1
    
    if n < 0:
        return 1.0/expo2(x, -n)

    cache_map, max_power, c = build_cache_map(x, n)
  
    ret, power_to_calc, power_next = 1, n, max_power
    while n > 0 and power_next >= 1:
        if power_next <= power_to_calc:
            ret *= cache_map[power_next]
            c += 1
            power_to_calc -= power_next
        power_next /= 2
    
    return ret, c

if __name__ == "__main__":
    print expo1(2, 5), 2**5
    print expo1(2, -5), 2**(-5)
    print expo1(3, 8), 3**8

    # print build_cache_map(2, 100)
    print expo1(2, 30), expo2(2, 30), 2**30
    print expo1(2, 100), expo2(2, 30), 2**100
