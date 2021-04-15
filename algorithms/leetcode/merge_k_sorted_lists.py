import heapq

class SortedListHead(object):
    def __init__(self, sorted_list):
        self._list = sorted_list

    # this works in python 2 only. In python 3 use __lt__ special method
    def __cmp__(self, other):
        return cmp(self._list[0], other._list[0])

    
if __name__ == "__main__":
    l1 = [2, 3, 6, 8, 20]
    l2 = [3, 9, 21, 25, 32]
    l3 = [4, 5, 12, 19, 31]

    ls = [l1, l2, l3]
    pq = []
    
    heapq.heappush(pq, SortedListHead(l1))
    heapq.heappush(pq, SortedListHead(l2))
    heapq.heappush(pq, SortedListHead(l3))

    while pq:
        sorted_list = heapq.heappop(pq)
        p = sorted_list._list.pop(0)
        print(p)
        if sorted_list._list:
            heapq.heappush(pq, sorted_list)

    
