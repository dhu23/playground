# input:  m, n (m < n)
# output: give m distinct integers from 1 to n that maximizes the total
#         number of 1-bits in pairwise XORs

# total # of bits in all pairwise xors given an array of integers x:
# F(x) = sum(number_of_bits(xor(x[i], x[j])) for i = 1 to m, j=(i+1) to m)

# example:
# if x = [4, 6, 8], NoB(num) return number of 1 bits in the number
# F(x) = NoB(4 xor 6) + NoB(4 xor 8) + NoB(6 xor 8)
#      = NoB(2) + NoB(12) + NoB(14) = 1 + 2 + 3
# 4  --->   00000100
# 6  --->   00000110
# 8  --->   00001000
# 4 xor 6 : 00000010  (2)
# 4 xor 8 : 00001100  (12)
# 6 xor 8 : 00001110  (14)

import itertools
def num_of_bits(x):
    ret = 0
    while x:
        x, bit = divmod(x, 2)
        ret += bit
    return ret

def pairwise_xor(data_list):
    xors = []
    for i in range(len(data_list)):
        for j in range(i+1, len(data_list)):
            xors.append(data_list[i] ^ data_list[j])
    return xors

def total_bits(nums):
    return sum(num_of_bits(i) for i in nums)

def total_bits_pairwise_xor(data_list):
    return total_bits(pairwise_xor(data_list))

def naive(m, n):
    combinations = itertools.combinations(range(1, n+1), m)
    #print(list(map(total_bits_pairwise_xor, combinations)))

    for c in combinations:
        print(c, total_bits_pairwise_xor(c))                   


if __name__ == "__main__":
    print(total_bits(pairwise_xor([4, 6, 8])))

    naive(30, 50)
    
