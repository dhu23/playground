# implement an algorithm to find the kth to the last element of a singly
# linked list. k is 1-based. So 1st means the last.
# 2nd means the one before the last.
from linked_list import *

def kth_to_last(ll, k):
    ll_len = list_len(ll)
    if k > ll_len:
        raise Exception('%d longer than size %d' % (k, ll_len))
    return find_kth(ll, ll_len-k+1)

# for fun, using recursion
def kth_to_last_counter(ll, k, ret):
    if ll.next is None: # this is the last node
        if k == 1:
            ret[0] = ll
        return 1 # the first by counting backwards
    c = kth_to_last_counter(ll.next, k, ret)
    assert len(ret) == 1
    if ret[0] is None and k == c+1:
        ret[0] = ll
    return c+1
        
        
    
def kth_to_last_recursion(ll, k):
    ret = [None]
    kth_to_last_counter(ll, k, ret)
    assert len(ret) == 1
    return ret[0]


# runner method
def kth_to_last_runner(ll, k):
    n1, n2 = ll, ll

    for i in range(k-1):
        if n2.next is not None:
            n2 = n2.next
        else:
            return None

    # move until n2 reaches the last node
    while n2.next is not None:
        n2 = n2.next
        n1 = n1.next

    return n1

if __name__ == "__main__":
    print(kth_to_last(convert_list(range(10)), 3)) # print 3rd to last, node 7
    print(kth_to_last_recursion(convert_list(range(10)), 3))
    print(kth_to_last_runner(convert_list(range(10)), 3))
    
