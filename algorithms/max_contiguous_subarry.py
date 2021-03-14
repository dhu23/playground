#!/usr/bin/python

data = [-3, 2, 3, -6, 4, 6, -10, 2, 5, 9, -5, 2, -1, -4]


def max_subarray(data):
    maxSoFar, maxCurrent = 0, 0
    for i in data:
        maxCurrent = max(0, maxCurrent + i)
        maxSoFar = max(maxCurrent, maxSoFar)
        print i, '\t', maxCurrent, '\t', maxSoFar
    return maxSoFar


print max_subarray(data)
