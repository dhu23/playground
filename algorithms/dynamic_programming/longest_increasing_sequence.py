# L is a table data to tack the longest increasing sequence that ends with
# letter at index i. Misunderstanding of this fact would get the whole
# algorithm wrong

def lis1(l):
    L = []
    for i in range(len(l)):
        L.append(1) # L(i)

    for i in range(1, len(l), 1):
        for j in range(i):
            if l[j] < l[i] and L[j]+1 > L[i]: 
                L[i] = L[j]+1
    return L



if __name__ == "__main__":
    seq = [1, 2, 3, 0, 11, 23, 34, 56, 5, 6, 7, 24, -5]
    # seq = [10, 22, 9, 33, 21, 50, 41, 60]
    seq = [10, 22, 9, 33, 21, 23, 24, 25, 26]
    print lis1(seq)
