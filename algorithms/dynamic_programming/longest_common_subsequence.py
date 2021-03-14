# longest common subsequence

# recursive solution

def lcs1(astr, bstr):
    # boundary conditions: one of the string has len=1
    if len(astr) == 1:
        return 1 if astr[0] in bstr else 0

    if len(bstr) == 1:
        return lcs1(bstr, astr)
    
    if astr[-1] == bstr[-1]:
        return 1+lcs1(astr[:-1], bstr[:-1])
    else:
        # lcs won't have both last letter at the same time
        return max(lcs1(astr[:-1], bstr), lcs1(astr, bstr[:-1]))

# dynamic programming
def lcs2(astr, bstr):
    L = {}

    # L[(i, j)] = L(astr[0:i+1], bstr[0:j+1])
    for i in range(len(astr)):
        for j in range(len(bstr)):
            if i == 0 or j == 0:
                L[(i, j)] = 1
            elif astr[i] == bstr[j]: # i and j should be both greater than 0
                L[(i, j)] = L[(i-1, j-1)]+1
            else:
                L[(i, j)] = max(L[(i, j-1)], L[(i-1, j)])
                
    return L[(len(astr)-1, len(bstr)-1)]

if __name__ == "__main__":
    test1 = ('ABCDGH', 'AEDFHR')
    test2 = ('AGGTAB', 'GXTXAYB')

    print lcs1(*test1)
    print lcs1(*test2)
    print lcs2(*test1)
    print lcs2(*test2)
