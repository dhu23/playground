#!/usr/bin/python


data = [2, 3, 19, 6, 12, 1, 45, 22, 35, 21, 17]

def swap(data, i, j):
    if i == j: 
        pass
    else: 
        temp = data[i]
        data[i] = data[j]
        data[j] = temp


for i in xrange(len(data)):
    minValue = data[i]
    minIndex = i
    for j in xrange(i+1, len(data), 1):
        if data[j] < minValue:
            minValue = data[j]
            minIndex = j
    swap(data, minIndex, i)


print data
