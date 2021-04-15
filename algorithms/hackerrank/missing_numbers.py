
def find_missing_numbers(arr, brr):
    arr.sort()
    brr.sort()
    i, j, a_len, b_len = 0, 0, len(arr), len(brr)

    print(arr)
    print(brr)
    
    diff = 0
    while i < a_len and j < b_len:
        #print(i, j, arr[i], brr[j])
        if arr[i] > brr[j]:
            diff += 1
            j += 1
            while j < b_len or brr[j] == brr[j-1]:
                j += 1
        elif arr[i] == brr[j]:
            ii, jj = i+1, j+1
            while ii < a_len and arr[ii] == arr[i]:
                ii += 1
            while jj < b_len and brr[jj] == brr[j]:
                jj += 1
            #print(i, j, ii, jj)
                
            if ii-i != jj-j:
                diff += 1
            i, j = ii, jj
        else:
            raise Exception('should not happen')
        
                
    return diff



if __name__ == "__main__":
    print(find_missing_numbers(
        [203, 204, 205, 206, 207, 208, 203, 204, 205, 206],
        [203, 204, 204, 205, 206, 207, 205, 208, 203, 206, 205, 206, 204]))
