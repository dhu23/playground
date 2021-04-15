# given a set of non-overlapping intervals, insert a new one into the intervals
# you may assume the intervals were initially sorted according to the start

# example 1
# [1, 3], [6, 9], insert [2, 5] --> [1, 5], [6, 9]

# example 2:
# [1, 2], [3, 5], [6, 7], [8, 10], [12, 16], insert [4, 9] ->
#         [1, 2], [3, 10], [12, 16]

import bisect

def insert_interval_bisect(intervals, a, b):
    assert a < b
    if not intervals:
        return [[a, b]]

    x1, y1 = intervals[0]
    if b < x1:
        ret = [[a, b]]
        ret.extend(intervals)
        return ret
    if b == x1:
        ret = [[a, y1]]
        ret.extend(intervals[1:])
        return ret

    xn, yn = intervals[-1]
    if a > yn:
        ret = list(intervals)
        ret.append([a, b])
        return ret
    if a == yn:
        ret = list(intervals[:-1])
        ret.append([xn, b])
        return ret
    
    ends = []
    for (x, y) in intervals:
        ends.append(x)
        ends.append(y)
    #print(ends)

    i = bisect.bisect(ends, a)
    j = bisect.bisect(ends, b)
    
    to_merge_left = i if i%2 == 0 else i-1
    if j%2 != 0:
        to_merge_right = j
    else:
        if ends[j] == b:
            to_merge_right = j
        else:
            to_merge_right = j-1
            
    assert to_merge_left <= to_merge_right
    assert to_merge_right >= 0 and to_merge_left < 2*len(intervals)

    #print(to_merge_left, to_merge_right)

    ret = intervals[:to_merge_left//2]
    ret.append([intervals[to_merge_left//2][0],
                intervals[to_merge_right//2][1]])
    ret.extend(intervals[to_merge_right//2+1:])

    return ret


def insert_interval_roll(intervals, a, b):
    assert a < b
    if not intervals:
        return [[a, b]]

    ret = []
    # add all existing intervals to the left of [a, b]
    i = 0
    while i < len(intervals) and intervals[i][1] < a:
        ret.append(intervals[i])
        i += 1

    ret.append([a, b])
    
    # merge
    while i < len(intervals) and intervals[i][0] <= ret[-1][1]:
        ret[-1][0] = min(ret[-1][0], intervals[i][0])
        ret[-1][1] = max(ret[-1][1], intervals[i][1])
        i += 1

    # now merge is done, simple append the rest
    ret.extend(intervals[i:])
    
    return ret
            


if __name__ == "__main__":
    print(insert_interval_bisect(
        [[1, 2], [3, 5], [6, 7], [8, 10], [12, 16]], 4, 9))
    print(insert_interval_bisect([[1, 3], [6, 9]], 2, 5))
    print(insert_interval_bisect([[3, 4], [9, 11]], 1, 2))

    print('---------------')
    print(insert_interval_roll(
        [[1, 2], [3, 5], [6, 7], [8, 10], [12, 16]], 4, 9))
    print(insert_interval_roll([[1, 3], [6, 9]], 2, 5))
    print(insert_interval_roll([[3, 4], [9, 11]], 1, 2))

    
