# you have an array with all the numbers from 1 to N, where N is at most 32000
# The array may have duplicate entries and you do not know what N is. With
# only 4 kilobytes of memory available, how would you print all duplicate
# element in the array.


# 4 kB -> 4096 Bytes. Use char

def find_dup(arr):
    marks = bytearray(4096*8) # bytes is an immutable bytearray

    for x in arr:
        # find the location
        byte_pos, bit_pos = divmod(x, 8)
        x_byte = marks[byte_pos]
        mask = 1 << bit_pos
        x_bit = (x_byte & mask) != 0
        if x_bit is False: # we havne't seen number x
            marks[byte_pos] |= mask
        else:
            print(x)
            
            


if __name__ == "__main__":
    find_dup([1, 2, 3, 2, 3023, 230, 10000, 10000, 10000, 2, 3])
