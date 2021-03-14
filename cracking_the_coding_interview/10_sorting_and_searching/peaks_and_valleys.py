# in an array of integers, a 'peak' is an element which is greater than or
# equal to the adjacent integers and a 'valley' is an element which is less
# or equal to the adjacent integers.
# For example in the array {5, 8, 6, 2, 3, 4, 6}, {8, 6} are peaks and
# {5, 2} are valleys. Given an array of integers, sort the array into
# alternating sequence of peaks and valleys.


# example
# input {5, 3, 1, 2, 3}
# output {5, 1, 3, 2, 3}
import itertools

# an O(n log n) algorithm
def make_peaks_and_valleys(arr):
    arr_len = len(arr)
    if arr_len in (0, 1, 2): return list(arr)
    arr.sort()
    if arr_len == 3: return [arr[0], arr[3], arr[2]]
    # pattern = [1, 0, 3, 2], in fact it is pair swap from [0, 1, 2, 3]

    idx = 0
    while idx+1 < arr_len:
        arr[idx], arr[idx+1] = arr[idx+1], arr[idx]
        idx += 2

    return list(arr)

# make it linear
# all possible config of 3 numbers. S(small), M(middle), L(large)
# S M L ==> S L M (S is still a valley).
# S L M ==> already a peak
# M L S ==> already a peak
# M S L ==> M L S (M is still a valley)
# L S M ==> S L M (L changed to S, making an even lower valley. still valid)
# L M S ==> M L S (L changed to M, making an even lower valley. still valid)
def make_peaks_and_valleys_linear(arr):

    def swap(arr, i, j):
        assert i >= 0 and i < len(arr) and j >= 0 and j < len(arr)
        arr[i], arr[j] = arr[j], arr[i]
    
    def make_peak(arr, i):
        assert i >= 1
        if i == len(arr)-1: # missing the 3rd number
            if arr[i] < arr[i-1]:
                swap(arr, i, i-1)
        else:
            if arr[i-1] <= arr[i] <= arr[i+1]: # case 1
                swap(arr, i, i+1)
            elif arr[i] <= arr[i-1] <= arr[i+1]:
                swap(arr, i, i+1)
            elif arr[i] <= arr[i+1] <= arr[i-1]:
                swap(arr, i-1, i)
            elif arr[i+1] <= arr[i] <= arr[i-1]:
                swap(arr, i-1, i)
                
    for i in range(1, len(arr), 2):
        make_peak(arr, i)

    return list(arr)

def verify(arr):
    def check_peak(arr, i):
        if len(arr) < 2: return True
        if i == 0: return arr[i] >= arr[i+1]
        if i == len(arr)-1: return arr[i] >= arr[i-1]
        return arr[i] >= arr[i-1] and arr[i] >= arr[i+1]

    def check_valley(arr, i):
        if len(arr) < 2: return True
        if i == 0: return arr[i] <= arr[i+1]
        if i == len(arr)-1: return arr[i] <= arr[i-1]
        return arr[i] <= arr[i-1] and arr[i] <= arr[i+1]

    def check_patterns(arr, patterns):
        return all(pattern(arr, i)
                   for i, (_, pattern) in enumerate(zip(arr, patterns)))
            
    return (check_patterns(arr, itertools.cycle((check_peak, check_valley))) or
            check_patterns(arr, itertools.cycle((check_valley, check_peak))))

if __name__ == "__main__":
    ret1 = make_peaks_and_valleys([5, 8, 6, 2, 3, 4, 6])
    print(ret1, verify(ret1))
    ret2 = make_peaks_and_valleys([5, 3, 1, 2, 3])
    print(ret2, verify(ret2))
    ret3 = make_peaks_and_valleys_linear([5, 8, 6, 2, 3, 4, 6])
    print(ret3, verify(ret3))
    ret4 = make_peaks_and_valleys_linear([5, 3, 1, 2, 3])
    print(ret4, verify(ret4))
