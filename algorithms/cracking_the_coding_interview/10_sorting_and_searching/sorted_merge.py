# you are given two sorted arrays, A and B, where A has a larger enough buffer
# at the end to hold B. Write a method to merge B into A in sorted order.


# this is only a demonstration of how it would be otherwise done in C++
# At the end of A there are a sequence of Nones to simulate buffer
# in other words, no append() is used but []

def merge_sorted(a, a_len, b, b_len):
    a_idx, b_idx = a_len-1, b_len-1
    idx = a_len+b_len-1 # the idx to work on

    while (a_idx >= 0 and b_idx >= 0):
        if a[a_idx] > b[b_idx]:
            a[idx] = a[a_idx]
            a_idx -= 1
        else:
            a[idx] = b[b_idx]
            b_idx -= 1
        idx -= 1

    # only one of a_idx and b_idx would be still greater than 0
    # if it is a_idx, nothing to do
    while (b_idx >= 0):
        a[idx] = b[b_idx]
        idx -= 1
        b_idx -= 1
    
    
        
            


if __name__ == "__main__":
    a, a_len = [5, 6, 10, 20, 33, None, None, None, None, None, None], 5
    b, b_len = [1, 3, 5, 12], 4

    # result returned in a
    merge_sorted(a, a_len, b, b_len)
    print(a)
