# given an image represented by an NxN matrix, where each pixel in the image
# is 4 bytes, write a method to rotate the image by 90 degrees, in place

#       (i, j)
#    
#                         (j, n-i)
#
#
#
# (n-j, i)
#
#                    (n-i, n-j)
# for even N,
# ----------- 0 -----------
#   --------- 1 ---------
#      ---------------
#         ---------
#            (n/2-1)
# for odd N,
# ---------- 0 ---------------
#   -------- 1 -------------
#     ------ -----------
#        ----------- (n-1)/2-1
#          (n-1)/2

def rotate_matrix(m):
    dim = len(m)
    for i in range(0, dim // 2): # outer index go from 0 to floor(n/2)-1
        for j in range(i, dim-1-i):
            #print(i, j, dim)
            m[i][j], m[j][dim-1-i], m[dim-1-i][dim-1-j], m[dim-1-j][i] = (
                m[dim-1-j][i], m[i][j], m[j][dim-1-i], m[dim-1-i][dim-1-j])
    return m

def print_matrix(m):
    dim = len(m)
    for i in range(dim):
        assert len(m[i]) == dim
        format = ''.join(['{:3}'] * dim)
        print(format.format(*m[i]))

def build_matrix(dim):
    m = []
    for i in range(dim):
        m.append(list(range(dim)))
    return m

if __name__ == "__main__":
    print_matrix(rotate_matrix(build_matrix(5)))
    print_matrix(rotate_matrix(build_matrix(6)))
