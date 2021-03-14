# write an algorithm such that if an element in an MxN matrix is 0
# its entire row and column are set to 0

def zero_matrix(matrix):
    m, n = len(matrix), None
    zrows, zcols = set(), set()
    
    for i in range(m):
        assert n is None or len(matrix[i]) == n
        if n is None:
            n = len(matrix[i])
        for j in range(n):
            if matrix[i][j] == 0:
                zrows.add(i)
                zcols.add(j)
    print(zrows, zcols)
    for i in range(m):
        if i in zrows:
            matrix[i] = [0]*n
        else:
            for j in range(n):
                if j in zcols:
                    matrix[i][j] = 0
        # print out row
        format = ''.join(['{:3}']*n)
        print(format.format(*matrix[i]))

def build_matrix():
    return [
        [5, 1, 18, 4, -5, 0],
        [6, 0, 1, 2, 6, -15],
        [2, 3, 5, 6, -7, 92],
        [1, 2, 3, 4, 100, 1],
        [0, 1, 2, -3, 0, 11]]
        
if __name__ == "__main__":
    zero_matrix(build_matrix())
