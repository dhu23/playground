# print out all the ways to factor a number
# for example, 24
# 24
# 2*12
# 2*2*6
# 2*2*2*3
# 2*3*4
# 3*8
# 4*6


def factor(n, init=2):
    assert n >= 2, "please provide a number greater than or equal to 2"
    factor_stack = [[n]]

    start = init
    while True:
        if n % start != 0:
            pass
        else:
            rem = n // start
            if rem < start: 
                break
            
            rem_factors = factor(rem, start)

            for each_rem_factoring in rem_factors:
                ret = [start]
                ret.extend(each_rem_factoring)
                factor_stack.append(ret)

        start += 1
            
    return factor_stack
