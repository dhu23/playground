# given an input file with four billion non-negative integers, provide an
# algorithm to generate an integer that is not contained in the file. Assume
# you have 1 GB of memory available for this task.

# follow up:
# what if you have only 10 MB of memory? Assume that all the values are
# distinct and we now have no more than one billion non-negative integers.

import bisect
import random
# simple case. generate number that's not contained in a sorted array
def generate_non_existing_from_sorted(arr):
    assert arr
    i = arr[0]+1
    
def find_missing_number_distinct(arr, left, right):
    if left >= right: return None
    # when there are 2 numbers
    if left+1 == right:
        if arr[right]-arr[left] == 1:
            return None
        else:
            return arr[right]-1   
    
    arr_len = right-left+1
    num_range = arr[right]-arr[left]+1

    if arr_len >= num_range: return None
    # there are holes somewhere
    mid = (left+right) // 2
    found_left = find_missing_number_distinct(arr, left, mid)
    if found_left is not None:
        return found_left
    else:
        return find_missing_number_distinct(arr, mid+1, right)
    

# 4 billion non-negative integers. for integers with 32 bit there are
# 2**31 non-negative integers out there. therefore they can be represented
# by 2**28 bytes
# 1 GB memory has more than 8 billion bits
def problem_1(arr):
    bitset = bytearray(2**28) # 256 MB
    print(len(bitset))
    for i in arr:
        byte_idx, bit_idx = divmod(i, 8)
        mask = 1 << bit_idx
        if bitset[byte_idx] & mask == 0:
            bitset[byte_idx] |= mask
    print(bin(bitset[0]))

    for i in range(len(bitset)):
        for k in range(0, 8):
            mask = 1 << k
            if bitset[i] & k == 0 and i*8 + k > 0: # python & needs no ()
                return i*8+k
    return None

# with only 10MB memory,
# 2**20 bytes ~ 1MB, or a lower limit of 2**23 bytes
# divide the whole range into N buckets of size S, when stored in
# memory, it is a vecotr<int> of size N, S is stored as an int which is
# of 4 byte, so N can be at most 2**21
# each int represents positive value up to 2**31, which is large enough
# we found out which bucket has missing value, then use the problem_1 method
# again to find out the missing one with a smaller check range. That is, only
# integers in a certain range are processed in the for loop
def problem_2(arr):
    histogram = [0] * (2**21)
    bin_size = 2**10

    for i in arr:
        histogram[i // bin_size] += 1

    missing_bucket = None
    for i, h in enumerate(histogram):
        print('loop==>', i, h, bin_size)
        if (i == 0 and h < bin_size-1) or (i > 0 and h < bin_size):
            missing_bucket = i
            break
    print('missing bucket=', missing_bucket)
    if missing_bucket is None:
        return None
    check_range = (missing_bucket*bin_size, (missing_bucket+1)*bin_size)
    print(check_range)

    # now use a bitset of size bin_size which is very small
    bitset = bytearray(bin_size // 8)
    for i in arr:
        if i < check_range[0] or i >= check_range[1]: continue
        byte_idx, bit_idx = divmod(i-check_range[0], 8)
        bitset[byte_idx] |= (1 << bit_idx)
    print(bin(bitset[0]))

    for i in range(len(bitset)):
        print('in bucket=', bin(bitset[i]))
        for k in range(0, 8):
            mask = 1 << k
            if bitset[i] & mask == 0 and i*8 + k + check_range[0] > 0:
                return i*8 + k + check_range[0]
    return None
    
    
if __name__ == "__main__":
    print(find_missing_number_distinct([1, 2, 3, 4, 5, 6, 8, 20], 0, 7))
    print(problem_1([2, 4, 7, 25, 1000]))
    print('missing', problem_2([1, 2, 4, 7, 25, 1000]))
    print('missing', problem_2(list(range(1, 1026))))
