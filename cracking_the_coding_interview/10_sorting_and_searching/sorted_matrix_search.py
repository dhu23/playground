# given an M x N matrix in which each row and each column is sorted in
# ascending order, write a method to find an element

# for any element in the middle of the matrix m(i, j) that partitions the
# matrix into 4 sub-matrices
# A1  |  A2
# A3  |  A4
# where m1 is the bottom-right of A1, m2 is the top left of A4
# A1 <= m1
# A4 >= m2
# A2 and A3 is not determined
import random

def get_matrix():
    m = [
        [1,   3,  5,  6,  7,  8],
        [2,   4, 10, 19, 22, 25],
        [12, 13, 17, 21, 23, 26]
        ]
    return m, 3, 6

def find(m, x, row_up, row_down, col_left, col_right):
    #print(row_up, row_down, col_left, col_right)
    if row_up > row_down or col_left > col_right: return (None, None)
    if m[row_up][col_left] > x or m[row_down][col_right] < x:
        return (None, None)
    if m[row_up][col_left] == x: return (row_up, col_left)
    if m[row_down][col_right] == x: return (row_down, col_right)

    row_mid1, col_mid1 = (row_up+row_down) // 2, (col_left+col_right) // 2
    row_mid2, col_mid2 = row_mid1+1, col_mid1+1

    if m[row_mid1][col_mid1] == x: return (row_mid1, col_mid1)
    if m[row_mid2][col_mid2] == x: return (row_mid2, col_mid2)
    elif m[row_mid1][col_mid1] > x:
        in_upper_left = find(m, x, row_up, row_mid1, col_left, col_mid1)
        if in_upper_left != (None, None):
            return in_upper_left
        else:
            in_upper_right = find(m, x, row_up, row_mid1, col_mid2, col_right)
            if in_upper_right != (None, None):
                return in_upper_right
            else:
                return find(m, x, row_mid2, row_down, col_left, col_mid1)
    elif m[row_mid2][col_mid2] < x:
        in_bottom_right = find(m, x, row_mid2, row_down, col_mid2, col_right)
        if in_bottom_right != (None, None):
            return in_bottom_right
        else:
            in_upper_right = find(m, x, row_up, row_mid1, col_mid2, col_right)
            if in_upper_right != (None, None):
                return in_upper_right
            else:
                return find(m, x, row_mid2, row_down, col_left, col_mid1)
    else:
        in_upper_right = find(m, x, row_up, row_mid1, col_mid2, col_right)
        if in_upper_right != (None, None):
            return in_upper_right
        else:
            return find(m, x, row_mid2, row_down, col_left, col_mid1)
            
    
if __name__ == "__main__":
    m, row_dim, col_dim = get_matrix()
    print(find(m, 17, 0, row_dim-1, 0, col_dim-1))
    print(find(m, 8, 0, row_dim-1, 0, col_dim-1))
