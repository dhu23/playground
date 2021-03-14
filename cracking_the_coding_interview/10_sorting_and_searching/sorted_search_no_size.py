# you are given an array like data structure Listy which lacks a size method
# it does however have an elementAt(i) method that returns the element at index
# i in O(1) time. If i is beyond the bounds of the data structure, it returns
# -1. (For this reason, the data structure only supports positive integers).
# Given a Listy which contains sorted, positive integers, find the index at
# which an element x occurs. If x occurs multiple times, you many return any
# index. 

class Listy(object):
    def __init__(self, data):
        assert all(x > 0 for x in data)
        self.data = sorted(list(data))

    def element_at(self, i):
        return self[i]
        
    def __getitem__(self, i):
        if i >= len(self.data):
            return -1
        else:
            return self.data[i]

def build_listy():
    return Listy([1, 10, 20, 25, 30, 35, 50])


def find_dumb(listy, x, stride=20):
    if listy.element_at(0) == -1: return None # listy is empty
    if x < listy.element_at(0): return None

    right = 0
    while listy.element_at(right) != -1 and listy.element_at(right) < x:
        right += stride
    left = right-stride
    assert left >= 0 and listy.element_at(left) > 0

    while left <= right:
        mid = (left+right) // 2
        if listy.element_at(mid) == x:
            return mid
        elif listy.element_at(mid) == -1:
            right = mid-1
        elif listy.element_at(mid) < x:
            left = mid+1
        else: # listy[mid] > x
            right = mid-1
    return None # didn't find it
    

def find(listy, x):
    idx = 0
    def next_idx(idx): return (idx+1)*2-1 # 2*idx+1
    
    while (listy.element_at(idx) < x and listy.element_at(idx) != -1):
        idx = next_idx(idx)

    return binary_search(listy, x, idx//2, idx)

def binary_search(listy, x, left, right):
    if left > right: return None
    mid = (left+right) // 2
    mid_val = listy.element_at(mid)
    if mid_val == x:
        return mid
    elif mid_val > x or mid_val == -1:
        return binary_search(listy, x, left, mid-1)
    else:
        return binary_search(listy, x, mid+1, right)



if __name__ == "__main__":
    l = build_listy()
    print(l[3], l[4], l[20])
    print(l.element_at(3), l.element_at(4), l.element_at(20))
    
    print(find(l, 10)) # 1
    print(find(l, 50)) # 6
    print(find(l, 22)) # none
