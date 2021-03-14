from bitonic import binary_search, binary_search_floor
import data_building
import random


def brute_force(data_list1, data_list2, k):
    i1, i2 = 0, 0
    data_indicator = None
    for i in xrange(k):
        if data_list1[i1] <= data_list2[i2]:
            i1 += 1
            data_indicator = 1
        else:
            i2 += 1
            data_indicator = 2
    if data_indicator == 1:
        return data_list1[i1-1], data_indicator, i1-1
    elif data_indicator == 2:
        return data_list2[i2-1], data_indicator, i2-1
    else:
        raise Exception("Data indicator should not be None")


def check_if_kth(data_list1, data_list2, k, i):
    '''the two lists are sorted ascendingly'''
    # verify
    if i >= len(data_list1):
        raise Exception("data one does not have index %d" % i)

    j = k-(i+1)-1 
    if j < 0: # i is too big already
        return False, False
    
    if data_list2[j] < data_list1[i] and \
            (j+1 >= len(data_list2) or data_list2[j+1] > data_list1[i]):
        return True, None
    else:
        if data_list2[j] > data_list1[i]:
            return False, True # i is too small
        else:
            return False, False # i is too big
            

def binary_search(data_list1, data_list2, lo, hi, k):
    if lo > hi:
        return None, None

    if check_if_kth(data_list1, data_list2, k, lo)[0]:
        return data_list1[lo], lo
    if check_if_kth(data_list1, data_list2, k, hi)[0]:
        return data_list1[hi], hi

    if lo == hi or lo + 1 == hi:
        return None, None
    else:
        mid = (lo + hi) / 2
        flag1, flag2 = check_if_kth(data_list1, data_list2, k, mid)
        if flag1 is True:
            return data_list1[mid], mid
        else:
            if flag2 is False:
                return binary_search(data_list1, data_list2, lo, mid, k)
            else:
                return binary_search(data_list1, data_list2, mid, hi, k)


def compare_algo(left, right):
    print check_if_kth(left, right, 10, 3)

    value, data_indicator, i = brute_force(left, right, 10)
    print value, data_indicator, i
    if data_indicator == 1:
        print 'in list 1'
        print check_if_kth(left, right, 10, i)
    elif data_indicator == 2:
        print 'in list 2'
        print check_if_kth(right, left, 10, i)

    print binary_search(left, right, 0, len(left)-1, 10)
    print binary_search(right, left, 0, len(right)-1, 10)


if __name__ == "__main__":
    left, right = data_building.build_two_arrays_of_distinct_data()

    left.sort()
    right.sort()

    print left
    print right

    compare_algo(left, right)
