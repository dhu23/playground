#!/usr/bin/python

data = [1, 3, 6, 2, 15, 4, 23, 56, 10, 80, 31]

def swap(data, i, j):
    temp = data[i]
    data[i] = data[j]
    data[j] = temp

def partition(data, left, right, pivot):
    '''this is an unordered partial sorting algorithm that does not only 
    yield the kth smallest element but also all elements that are smaller 
    than it. '''
    print data, left, right, pivot
    swap(data, pivot, right)
    pivot = right
    store = left
    for i, value in enumerate(data):
        if i < left: continue
        if i == pivot: break
        if value < data[pivot]:
            swap(data, store, i)
            store += 1
            print data
    swap(data, store, pivot)
    pivot = store
    print data
    return pivot

def select(data, left, right, k):
    pivot = (left+right)/2 # middle point
    print "init pivot", data[pivot], "at", pivot, 
    print "between", left, "and", right, "k=", k
    
    # partition the data list in a way such that 
    # all data that are smaller than the pivot value are to its left 
    # ad all data that are greater than the pivot value are to its right
    pivot = partition(data, left, right, pivot)
    print "pivot after partition", data[pivot], pivot
    if pivot == k:
        return pivot
    elif pivot < k:
        return select(data, pivot+1, right, k)
    else:
        return select(data, left, pivot-1, k)  



if __name__ == "__main__": 
    print data
    select(data, 0, len(data)-1, 5)
