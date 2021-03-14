# a magic index in an array A[0...n-1] is defined to be an index such that
# A[i] = i. Given a sorted array of distinct integers, write a method to find
# a magic index, if one exists, in array A

# Follow up: what if the values are not distinct

# since a is sorted,
# if A[i] = i, found it
# if A[i] > i, move it idx >= A[i]
# if A[i] < i, next
def find_magic_number(a, i=0): # this is basically a sub-linear search
    if i >= len(a):
        return []
    if i == a[i]:
        return [i] + find_magic_number(a, i+1)
    elif i < a[i]:
        return find_magic_number(a, a[i])
    else:
        return find_magic_number(a, i+1)

# using a binary search technique but the considerations are as follow:
# 1) if A[j] = i where j > i, search between A[0] and A[i] on j's left side
#    search everything on the right side
# 2) if A[j] = i where j < i, search everything on the left side, but
#                             search between A[i] and A[len-1] on the right
# 3) if A[j] = i where j == i, found it
def find_magic_number_dups(a):
    def find(a, left, right):
        if left > right: return None

        mid = (left+right) // 2

        # the following could be simplified to shorter code
        if mid > a[mid]: # case 1)
            found_left = find(a, left, a[mid]-1)
            if found_left: return found_left
            else: return find(a, mid+1, right)
        elif mid < a[mid]: # case 2)
            found_right = find(a, a[mid]+1, right)
            if found_right: return found_right
            else: return find(a, left, mid-1)
        else:
            return mid

    return find(a, 0, len(a)-1)

# the aforementioned two algorithms are very similar, except that the 2nd
# one might approach the result faster, as it is binary search


# when all the integers are distinct,
# the array can be divided to at most 3 regions:
# 1) A[j] < j, for [0, p1)
# 2) A[j] == j, for [p1, p2]
# 3) A[j] > j, for (p2, +inf)

def find_magic_number_distinct(a):
    if not a: return None
    left, right = 0, len(a)-1
    if a[left] > left or a[right] < right: return None

    while left <= right:
        mid = (left+right) // 2
        if a[mid] == mid: return mid # could be just one of many
        elif a[mid] < mid:
            left = mid+1
        else:
            right = mid-1
    return None
    
    
if __name__ == "__main__":
    print(find_magic_number([0, 1, 1, 2, 3, 3, 5, 6, 7, 7, 10]))
    print(find_magic_number_dups([0, 1, 1, 2, 3, 3, 5, 6, 7, 7, 10]))
    print(find_magic_number_distinct([-1, 0, 1, 2, 4, 5, 9, 10, 20]))
