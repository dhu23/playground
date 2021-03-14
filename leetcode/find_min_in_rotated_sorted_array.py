# suppose an array sorted in ascending order is rotated at some pivot unknown
# to you before hand. (ie, 0 1 2 3 4 5 6 7 might become 4 5 6 7 0 1 2)
# find the minimum element
# assume no duplicate exists in this array

# assumption:
# only usable for cases when the array is not rotated at a duplicated element
def find_min1(a):
    a_len = len(a)
    assert a_len > 0
    if a_len == 1:
        return a[0]
    if a_len == 2:
        return min(a[0], a[1])
    assert a[0] != a[-1] # assert assumption
    # not rotated
    if a[0] < a[-1]:
        return a[0]
    # this array is rotated
    lo, hi = 0, a_len-1
    while hi >= lo:
        if hi == lo:
            return a[lo]
        elif hi == lo+1:
            return min(a[lo], a[hi])
        mid = lo + (hi-lo) // 2
        # the target requirement
        if a[mid] <= a[mid+1] and a[mid] <= a[mid-1]:
            return a[mid]
        # in the first section
        if a[mid] >= a[0]:
            lo = mid
        elif a[mid] <= a[-1]:
            hi = mid
    raise Exception('should not happend')

if __name__ == "__main__":
    print('--------------- I --------------')
    d = list(range(10))
    print(find_min1(d[5:] + d[:5]))

# same problem, except that the array many contain duplicates

def find_min2(a):
    a_len = len(a)
    assert a_len > 3

    lo, hi = 0, a_len-1
    if a[0] < a[-1]:
        # not rotated
        return a[0]
    elif a[0] > a[-1]:
        # rotated but can use case 1
        return find_min1(a)

    # a[0] and a[1] are the same, can be something like input d2 or d3
    mid = lo + (hi-lo) // 2
    if a[mid] < a[0]:
        # a[:mid+1] is a rotated sorted array that should contain result
        return find_min1(a[:mid+1])
    elif a[mid] > a[0]:
        # a[mid+1:] is a rotated sorted array that should contain result
        return find_min1(a[mid+1:])
    else:
        # can go either way
        print('search both')
        return min(find_min1(a[:mid+1]), find_min1(a[mid+1:]))
        

if __name__ == "__main__":
    print('-------------- II --------------')
    d1 = [19, 19, 23, 25, 26, 30, 4, 4, 6, 8, 10]
    d2 = [19, 23, 25, 26, 30, 4, 4, 6, 8, 10, 19]
    d3 = [19, 19, 19, 19, 25, 4, 4, 19, 19, 19, 19]
    print(find_min2(d1))
    print(find_min2(d2))
    print(find_min2(d3))
    

    
