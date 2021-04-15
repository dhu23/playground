# given an unsorted integer array, find the first missing positive integer

# given [1, 2, 0], return 3
# given [3, 4, -1, 1], return 2

# run in O(n) time and use constant space

# swap each positve element to its right position
# n(n>0) -> position n-1
def get_first_missing_positive(data_array):

    size = len(data_array)
    
    for i in range(size):
        while (data_array[i] > 0 and data_array[i] <= size and
               data_array[i] != data_array[data_array[i]-1]):
            # swap data_array[i] with what's at data_array[i]-1
            a = data_array[i]
            data_array[i], data_array[a-1] = data_array[a-1], a
            
            # the following swap doesn't work in python
            #data_array[i], data_array[data_array[i]-1] = data_array[data_array[i]-1], data_array[i]

    for i in range(size):
        if data_array[i] != i+1:
            break

    return i+1
    

if __name__ == "__main__":
    print(get_first_missing_positive([3, 4, -1, 1]))
    print(get_first_missing_positive([1, 2, 0]))
    print(get_first_missing_positive([3, 7, -1, 1, 2, 4, 4, 2, 8]))
