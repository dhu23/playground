## rotate N by N matrix by 90 degree

def make_matrix(n=6):
    m = []
    for start in range(1, 1+n*(n-1)+1, n):
        m.append(range(start, start+n, 1))
    return m

def print_matrix(m):
    n = len(m)
    for row in m:
        print ''.join(['{%d:5d}' % i for i in range(n)]).format(*row)

    print '\n\n'

# (i, j), (j, n-i), (n-i, n-j), (n-j, i)        
def rotate_matrix(m, i, j, counter_clockwise=True):
    n = len(m)-1
    if counter_clockwise:
        m[i][j], m[j][n-i], m[n-i][n-j], m[n-j][i] = m[j][n-i], m[n-i][n-j], m[n-j][i], m[i][j]
    else:
        m[i][j], m[j][n-i], m[n-i][n-j], m[n-j][i] = m[n-j][i], m[i][j], m[j][n-i], m[n-i][n-j]

if __name__ == "__main__":
    m = make_matrix()
    print_matrix(m)
     
    n = len(m)
    for i in range(0, n/2 if n%2 == 0 else n/2+1):
        for j in range(0, n/2 if n%2 == 0 else n/2+1):
            rotate_matrix(m, i, j, False)

            print_matrix(m)
            
    print_matrix(m)
