import random


def binary_search(data_list, lo, hi, target, ascending=True):
    '''data is sorted'''
    print 'binary search %d in (%d, %d)' % (
            target, data_list[lo], data_list[hi])
    if lo > hi:
        return None

    if target == data_list[lo]:
        return lo
    if target == data_list[hi]:
        return hi

    if hi == lo or hi == lo + 1:# here data_list[lo] != target
        return None
    
    mid = (lo + hi) / 2
    if data_list[mid] == target:
        return mid
    elif data_list[mid] > target:
        if ascending:
            return binary_search(data_list, lo, mid, target)
        else: # descending order
            return binary_search(data_list, mid, hi, target, False)
    else:
        if ascending:
            return binary_search(data_list, mid, hi, target)
        else:
            return binary_search(data_list, lo, mid, target, False)



def binary_search_floor(data_list, lo, hi, target):
    '''data is sorted'''
    if lo > hi:
        return None

    if target < data_list[lo]:
        return None
    if target == data_list[lo]:
        return lo
    if target >= data_list[hi]:
        return hi

    # now target >= data_list[lo] and target < data_list[hi]
    if hi == lo or hi == lo + 1:
        return lo
    else:
        mid = (lo + hi) / 2
        if data_list[mid] == target:
            return mid
        elif data_list[mid] > target:
            return binary_search_floor(data_list, lo, mid, target)
        else:
            return binary_search_floor(data_list, mid, hi, target)


def test_binary_search(seed, func):
    random.seed(seed)
    data_list = random.sample(xrange(150), 40)
    data_list.sort()
    print data_list
    i = func(data_list, 0, 30-1, 95)
    if i:
        print 'found',data_list[i]
    else:
        print None

def find_bitonic_max(data_list, lo, hi):

    if lo > hi:
        return None

    if lo == hi:
        return lo
    elif lo + 1 == hi:
        return lo if data_list[lo] > data_list[hi] else hi
    else:
        mid = (lo + hi) / 2 # mid > lo and mid < hi
        if data_list[mid] > data_list[mid-1]: # on left side
            return find_bitonic_max(data_list, mid, hi)
        elif data_list[mid] < data_list[mid-1]:
            return find_bitonic_max(data_list, lo, mid-1)
        else:
            # this case won't happen as elements are distinct
            raise Exception("Shouldn't happen")


def build_bitonic_array():
    random.seed(123)
    total_size = random.randint(100, 150) # total size in [100, 150]

    # left_size is the increasing size without max
    random.seed(456)
    left_size = random.randint(1, total_size-2)
    right_size = total_size-1-left_size
    
    print 'left=%d, right=%d, total=%d' % (
            left_size, right_size, total_size)

    random.seed(789)
    total_data = random.sample(xrange(500), total_size)
    total_data.sort()

    # left indices can go from 0 to total_size-2, leaving out max
    # and total count of left indices is left_size
    random.seed(236)
    left_indices = random.sample(xrange(total_size-1), left_size)
    left_indices.sort()

    result = [total_data[l] for l in left_indices]
    result.extend(
            [total_data[r] for r in reversed(xrange(total_size)) \
                    if r not in left_indices])

    return result


def test_find_bitonic_max():
    input_data = build_bitonic_array()
    print input_data

    i = find_bitonic_max(input_data, 0, len(input_data)-1)
    if i:
        print 'max at', i, 'value', input_data[i]
    else:
        print 'did not find max'


def bitonic_search(data_list, lo, hi, target):
    print 'bitonic search %d in (%d, %d)' % (
            target, data_list[lo], data_list[hi])
    if lo > hi:
        return None
    if data_list[lo] > target and data_list[hi] > target:
        return None

    if target == data_list[lo]:
        return lo
    if target == data_list[hi]:
        return hi

    if lo == hi or lo + 1 == hi:
        return None
    
    mid = (lo + hi) / 2
    print mid, data_list[mid]
    if data_list[mid] == target:
        return mid
    
    if data_list[mid] > data_list[mid-1]: # on left side
        if data_list[hi] > target:
            # target won't be on right side
            if data_list[mid] > target:
                return binary_search(data_list, lo, mid, target)
            else:
                return bitonic_search(data_list, mid, hi, target)
        else: # can't decide which side target is 
            if target < data_list[mid]:
                r = binary_search(data_list, lo, mid, target)
                return r if r else bitonic_search(
                        data_list, mid, hi, target)
            else:
                return bitonic_search(data_list, mid, hi, target)
    elif data_list[mid] < data_list[mid-1]: # on right side
        if data_list[lo] > target:
            # target won't be on left side
            if data_list[mid] > target:
                return binary_search(data_list, mid, hi, target, False)
            else:
                return bitonic_search(data_list, lo, mid, target)
        else: # can't decide which side target is
            if target < data_list[mid]:
                r = binary_search(data_list, mid, hi, target, False)
                return r if r else bitonic_search(
                        data_list, lo, mid, target)
            else:
                return bitonic_search(data_list, lo, mid, target)
    else:
        raise Exception("Shouldn't happen")


def test_bitonic_search():
    test_data = build_bitonic_array()
    print test_data
    # target = test_data[random.randint(0, len(test_data)-1)]
    target = 488
    i = bitonic_search(test_data, 0, len(test_data)-1, target)
    if i:
        print 'found', test_data[i]
    else:
        print 'not found'


if __name__ == "__main__":
    # print build_bitonic_array()
    # test_binary_search(200, binary_search_floor)
    # test_find_bitonic_max()
    test_bitonic_search()
