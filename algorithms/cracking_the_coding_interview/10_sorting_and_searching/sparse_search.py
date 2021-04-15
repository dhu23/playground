# given a sorted array of strings that is interspersed with empty strings.
# write a method to find the location of a given string

# input: ball {"at", "", "", "", "ball", "", "", "car", "", "", "dad", "", ""}
# output: 4

def sparse_search_r(target, data, left, right):
    if left > right: return None

    while not data[left]:
        left += 1
    if left >= len(data) or data[left] > target: return None

    while not data[right]:
        right -= 1
    if right < 0 or data[right] < target: return None

    if left > right: return None
    mid1 = (left+right) // 2
    while not data[mid1]:
        mid1 -= 1
    if mid1 >= 0 and data[mid1] == target: return mid1
    if mid1 >= 0 and data[mid1] > target:
        return sparse_search_r(target, data, left, mid1-1)

    return sparse_search_r(target, data, (left+right)//2, right)
    

def sparse_search(target, data):
    return sparse_search_r(target, data, 0, len(data)-1)


if __name__ == "__main__":
    print(sparse_search(
        "ball",
        ["at", "", "", "", "ball", "", "", "car", "", "", "dad", "", ""]))
