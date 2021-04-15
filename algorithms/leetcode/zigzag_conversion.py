# n is number of rows
# row 1                         2n-1                          4n-3
# row 2                   2n-2  2n                      4n-4  4n-2
# row 3             2n-3        2n+1              4n-5        4n-1
# ...           ...             ...          ...
# row n-1   n+1                 3n-3   3n-1
# row n                         3n-2


# string input length is n
# in each |/ shape, position pattern is:
# 1   delta = 2n-2         -->  2n-1   
# 2   delta = 2n-4         -->  2n-2   delta = 2         -->  2n
# 3   delta = 2n-6         -->  2n-3   delta = 4         -->  2n+1
# .................................................................
# n-1 delta = 2n-2(n-1)=2  -->  n+1    delta = 2n-4      -->  3n-3
# n   delta = 2n-2                                       -->  3n-2


def print_row(row_id, input_str, n): # row_id goes from 1 to n
    delta = 2*n-2

    size = len(input_str)
    pos = row_id-1
    if row_id in (1, n):
        while pos < size:
            print input_str[pos],
            pos += delta
    else:
        slope = True # print the slop. False meaning the vertical line
        while pos < size:
            print input_str[pos],
            if slope is True:
                pos += 2*n-2*row_id
                slope = False
            else:
                pos += 2*(row_id-1)
                slope = True

def print_zigzag(input_str, n):
    for i in range(1, n+1):
        print_row(i, input_str, n)

if __name__ == "__main__":
    print_zigzag("PAYPALISHIRING", 3)
