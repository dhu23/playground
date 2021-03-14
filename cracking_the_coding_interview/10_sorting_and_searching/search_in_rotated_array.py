# given a sorted array of n integers that has been rotated an unknown number
# of times, write code to find an element in the array. You may assume the
# array is originally sorted in increasing order.

def binary_search(r, k, left, right):
    if left > right: return None
    mid = (left+right) // 2
    if r[mid] == k:
        return mid
    elif r[mid] < k:
        return binary_search(r, k, mid+1, right)
    else:
        return binary_search(r, k, left, mid-1)
    
# this is wrong because in certain cases both sides will have to be searched
# but it is not in the following logic
def search_rotated_wrong(r, k, left, right):
    if left > right: return None

    if r[left] == k: return left
    if r[right] == k: return right

    if r[left] < r[right]: # this is a normal array
        return binary_search(r, k, left, right)

    # 1) r[left] and r[right] are the same
    # 2) r[left] > r[right], this is a rotated array
    mid = (left+right) // 2
    if r[mid] == k: return mid
    
    if r[mid] >= r[left]: # r[mid] on the larger end
        if k > r[mid] or k < r[left]:
            return search_rotated(r, k, mid+1, right)
        else: # k between r[mid] and r[left]
            return binary_search(r, k, left, mid-1)
    else: # r[mid] on the smaller end
        if k < r[mid] or k > r[right]:
            return search_rotated(r, k, mid+1, right)
        else: # k between r[mid] and r[right]
            return binary_search(r, k, mid+1, right)

def search_rotated(r, k, left, right):
    if left > right: return None

    if r[left] == k: return left
    if r[right] == k: return right

    if r[left] < r[right]: # this is a normal array
        return binary_search(r, k, left, right)
    # optimization done

    mid = (left+right) // 2
    if r[mid] == k: return mid

    if r[mid] > r[left]: # left side is a normal array
        if k > r[left] and k < r[mid]:
            return binary_search(r, k, left+1, mid-1)
        else: # k is in the right side
            return search_rotated(r, k, mid+1, right-1)
    elif r[mid] < r[left]: # left side is a rotated array
        if k > r[mid] and k < r[right]:
            return binary_search(r, k, mid+1, right-1)
        else:
            return search_rotated(r, k, left+1, mid-1)
    else: # r[mid] == r[left]
        # now you don't know which side is rotated.
        # and x != r[mid]
        if r[right] != r[mid]: # x in right side
            search_rotated(r, k, mid+1, right-1)
        else:
            # search both
            found_in_left = search_rotated(r, k, left+1, mid-1)
            if found_in_left is not None:
                return found_in_left
            else:
                return search_rotated(r, k, mid+1, right-1)
    raise Exception("should not reach this line")
    
        
# for testing purpose
def naive(rotated, k):
    ret = []
    for i, x in enumerate(rotated):
        if x == k:
            ret.append(i)
    return ret


if __name__ == "__main__":
    original = sorted(set([1, 2, 3, 5, 6, 7, 13, 8, 14, 7, 9, 10, 15, 19, 23]))
    print(original)

    n = 8
    rotated = original[n:]+original[:n]
    print(rotated)
    print(search_rotated(rotated, 8, 0, len(rotated)-1))
    print(search_rotated(rotated, 1, 0, len(rotated)-1))

    import random
    def generate_rotate_arr(n):
        orig = sorted([random.randint(1, 20) for i in range(n)])
        k = random.randint(0, n-1)
        return orig[k:]+orig[:k]

    def verify(rotated_arr, k):
        found = search_rotated(rotated_arr, k, 0, len(rotated_arr)-1)
        naive_found = naive(rotated_arr, k)
        print('in rotated arr for k=', k, ',found=', found, naive_found)
        if found is None:
            if naive_found:
                print('rotated', rotated_arr, ',k=', k)
            assert not naive_found
        else:
            if found not in naive_found:
                print('rotated', rotated_arr, ',k=', k)
            assert found in naive_found
        

    verify(generate_rotate_arr(30), 15)
    verify(generate_rotate_arr(20), 15)
    verify(generate_rotate_arr(10), 15)
    verify(generate_rotate_arr(40), 15)
    verify(generate_rotate_arr(50), 5)
    
