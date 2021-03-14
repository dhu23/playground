
# find all inversions, aka a[i] < a[j] when i > j
import random

# brute force is efficient enough to retrieve all inversions
# as the result is of O(n^2) in terms of size
def brute_force(data_list):
    results = []
    for i, d1 in enumerate(data_list):
        for j, d2 in enumerate(data_list[i+1:]):
            if d1 > d2:
                results.append((d1, d2))
    return results


def merge(aux, lo, hi, mid, data_list):
    p1, p2 = lo, mid+1

    inv_count = 0

    for k in xrange(lo, hi+1):
        if p1 > mid:
            # left half is already exhausted
            # just copy the rest of the right half over
            # no inversions are created
            aux[k] = data_list[p2]
            p2 += 1
        elif p2 > hi:
            # right half is already exhausted
            # just copy the rest of the left half over
            # no inversions are created
            aux[k] = data_list[p1]
            p1 += 1
        elif data_list[p1][0] > data_list[p2][0]:
            # left size is great than right side. caught inversions
            # generate inversions for each number on left side after p2
            inv_count += mid-p1+1
            aux[k] = data_list[p2]
            p2 += 1
        else: # data[p1] < data[p2]
            # no inversions created when left side is smaller
            aux[k] = data_list[p1]
            p1 += 1
    return inv_count

def sort(data_list, lo, hi, aux):
    if lo >= hi:
        return 0
    if lo + 1 == hi:
        if data_list[lo] > data_list[hi]:
            data_list[lo], data_list[hi] = data_list[hi], data_list[lo]
            return 1
        else:
            return 0
    mid = lo + (hi-lo)/2
    c1 = sort(aux, lo, mid, data_list)
    c2 = sort(aux, mid+1, hi, data_list)
    return c1 + c2 + merge(data_list, lo, hi, mid, aux)


def merge_sort_count(data_list_input):
    data_list = list(data_list_input)
    aux = list(data_list)
    c = sort(data_list, 0, len(data_list)-1, aux)
    return data_list, c


if __name__ == "__main__":
    random.seed(12)
    data_list = random.sample(xrange(200), 100)
    print data_list

    # data_list = [2, 4, 1, 3, 5]

    results1 = brute_force(data_list)
    print len(results1)

    data_list2 = zip(data_list, range(len(data_list)))
    print merge_sort_count(data_list2)
