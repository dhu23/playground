import sys

#       0  1  2   3   4   5   6   7   8   9  10  11  12  13
data = [2, 3, 9, 12, 13, 15, 19, 20, 25, 29, 30, 31, 33, 39]

N = 37

def binary_search(vec, lo, hi, target):
    if lo > hi: return None
    else:
        while lo <= hi:
            mid = (lo+hi)/2
            print lo, mid, hi
            if vec[mid] == target: return mid
            elif vec[mid] > target:
                hi -= 1
            else:
                lo += 1
        # lo == hi 
        # return lo+1 if vec[lo] <= target else lo
        return lo

print binary_search(data, 3, 13, 2)

sys.exit(1)

for i in xrange(len(data)):
    for j in xrange(i+1, len(data), 1):
        # print i, j, data[i], data[j], N-data[i]-data[j]
        tar = N-data[i]-data[j]
        pos = binary_search(data, j+1, len(data)-1, tar)
        

            
