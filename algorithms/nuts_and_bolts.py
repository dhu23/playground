
# this sorts two arrays with distinct identical elements, though in different
# positions with a modified quick sort. And in this version, no eleemnts in 
# the same array are compared. 


import random

# 3 way quick partition
def _3_way_partition(data_list, lo, hi):
    # shuffling in interval [lo, hi]

    if lo >= hi:
        return

    if lo+1 == hi:
        if data_list[lo] > data_list[hi]:
            data_list[lo], data_list[hi] = data_list[hi], data_list[lo]
        return
    
    v = data_list[lo]
    p1, p2 = lo+1, hi
    lt, eq = lo+1, lo+1

    # maintain multiple pointers, 
    # start of < v section, start and end of unseen, 
    while p1 <= p2:
        d = data_list[p1]
        if d == v:
            p1 += 1
        elif d > v: 
            data_list[p2], data_list[p1] = data_list[p1], data_list[p2]
            p2 -= 1
        elif d < v:
            data_list[eq], data_list[p1] = data_list[p1], data_list[eq]
            eq += 1
            p1 += 1
    # place data_list[0] at the right place
    if eq-1 > 0:
        data_list[eq-1], data_list[0] = data_list[0], data_list[eq-1]


def order_nuts_and_bolts(nuts, bolts):
    # use nuts to partition bolts, very similar to a 2 or 3 way partition
    pass


if __name__ == "__main__":
    x = [random.randint(0, 9) for i in xrange(100)]
    print x
    _3_way_partition(x, 0, 99)
    print x
