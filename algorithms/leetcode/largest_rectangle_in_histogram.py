# given n non-negative integers, representing the histogram bar height
# where the width of each bar is 1, find the area of largest rectangle
# in the histogram

#           __
#        __|  |
#       |xx|xx|
#       |xx|xx|   __
#  __   |xx|xx|__|  |
# |  |__|xx|xx|  |  |
# |__|__|xx|xx|__|__|
#  2  1  5  6  2  3

# given heights = [2, 1, 5, 6, 2, 3], return 10


def find_largest_rectangle_naive(heights):
    trackers = [[] for i in range(max(heights))]
    # the 0th, store rectangle information for height 1
    # there might be more than 1 rectangele with height 1, and they
    # are stored as a list, which is the 0th element for height 1
    # each tracks, start, end
    
    for i in range(len(heights)):
        h = heights[i]
        # update all trackers with height less or equal to heights[i]
        for rh, tracker in enumerate(trackers, start=1):
            if rh <= h:
                # encounter a new bar that is a rectangle extension candidate
                if tracker and tracker[-1][1] == i-1:
                    tracker[-1][1] = i
                else:
                    tracker.append([])
                    tracker[-1].extend([i, i])   
    print(trackers)        
    # find the largest

    def get_area(t, h): return h*(t[1]-t[0]+1)
    
    return max(get_area(max(tracker, key=lambda t: t[1]-t[0]+1), h)
               for h, tracker in enumerate(trackers, start=1))


# stack based solution and it is very similar to the water trapper in some
# degree. The stack will maintain indices that form an increasing heights
def find_largest_rectangle_stack(heights):
    largest, stack = 0, []
    
    h_len = len(heights)
    for i in range(h_len):
        
        while stack and heights[stack[-1]] >= heights[i]:
            h = heights[stack[-1]]
            stack.pop()
            boundary = stack[-1] if stack else -1
            area = h * (i-boundary-1)
            # both i and boundary are exclusive
            # print('area', h, i, boundary)
            largest = max(largest, area)

        stack.append(i)

    return largest


if __name__ == "__main__":
    print(find_largest_rectangle_naive([2, 1, 5, 6, 2, 3]))
    print(find_largest_rectangle_stack([2, 1, 5, 6, 2, 3]))
