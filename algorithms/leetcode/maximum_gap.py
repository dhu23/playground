# given an unsorted array, find the max difference between the successive
# elements in its sorted form
# Try to solve it in linear time/space
# return 0 if the array contains less than 2 elements
# You may assume all elements in the array are non-negative integers and
# fit in the 32-bit signed interger range

def find_max_gap_naive(a):
    if len(a) < 2:
        return 0
    aa = sorted(list(a))
    return max((y-x) for x, y in zip(a, a[1:]))

# assuming the numbers are between 0 and 255 for demonstration purpose
def find_max_gap_radix(a):
    radix = [0 for i in range(256)]
    for i in a:
        assert 0 <= i <= 255
        radix[i] += 1
    # count the largest gap of non-zeros in radix array
    max_number = 0
    for i in range(256):
        if radix[i] != 0:
            max_number = i
    max_gap, max_gap_so_far = 0, 0
    for i in range(256):
        if i == max_number:
            break
        if radix[i] == 0:
            max_gap_so_far += 1
        else:
            max_gap_so_far = 0

        max_gap = max(max_gap_so_far, max_gap)
    print('max gap=', max_gap)
    return max_gap+1
            

if __name__ == "__main__":
    data = [23, 15, 22, 43, 15, 2, 34, 60, 95, 56]
    print(find_max_gap_naive(data) == find_max_gap_radix(data))
