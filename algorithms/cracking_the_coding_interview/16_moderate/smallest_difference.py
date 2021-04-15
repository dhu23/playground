# given two arrays of integers, compute the pair of values (one value in each
# array) with the smallest (non-negative) difference. Return the difference:
# example
# input: {1, 3, 15, 11, 2}, {23, 127, 235, 19, 8}
# output: 3. THat is the pair (11, 8)


# sort them
# 1, 2, 3, 11, 15
# 8, 19, 23, 127, 235


def find_smallest_difference(l1, l2):
    idx1, idx2 = 0, 0
    len1, len2 = len(l1), len(l2)

    min_diff = None
    while idx1 < len1 and idx2 < len2:
        
        if l1[idx1] > l2[idx2]:
            # advance idx2 until the next element is greater than l1's or
            # the end of the array
            # in other words the condition to advance idx2 is when idx2+1
            # is valid and smaller than l1
            while (idx2 < len2 and l1[idx1] >= l2[idx2] and
                   (idx2+1 < len2 and l1[idx1] >= l2[idx2+1])):
                idx2 += 1

            curr_diff = l1[idx1]-l2[idx2]
            min_diff = min(min_diff, curr_diff) if min_diff else curr_diff
            
            idx2 += 1
        elif l1[idx1] < l2[idx2]:
            # same as above but for l1
            while (idx1 < len1 and l2[idx2] >= l1[idx1] and
                   (idx1+1 < len1 and l2[idx2] >= l1[idx1+1])):
                idx1 += 1
                
            curr_diff = l2[idx2]-l1[idx1]
            min_diff = min(min_diff, curr_diff) if min_diff else curr_diff

            idx1 += 1
                
    print(min_diff)

if __name__ == "__main__":
    find_smallest_difference(sorted([1, 3, 15, 11, 2]),
                             sorted([23, 127, 235, 19, 8]))

    find_smallest_difference(sorted([1, 2, 3, 4]),
                             sorted([10, 12, 13, 14]))
