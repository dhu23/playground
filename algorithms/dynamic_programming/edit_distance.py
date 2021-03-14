
# convert 'str1' to 'str2' by minimum number of edits
# a. insert,
# b. remove,
# c. replace,


# morph s1 into s2
def calc_conversion(s1, s2):
    if s1 == s2:
        return 0
    
    if len(s1) == 0:
        return len(s2)
    elif len(s2) == 0:
        return len(s1)

    if s1[-1] == s2[-1]:
        return calc_conversion(s1[:-1], s2[:-1])
    elif s1[0] == s2[0]:
        return calc_conversion(s1[1:], s2[1:])

    # last characters of two strings are not the same
    # option 1: added s2[-1] to s1, then compare s1 with s2[:-1]
    # option 2: remove s1[-1], compare s2[:-1] and s2
    # option 3: replace s1[-1] with s2[-1], compare s1[:-1] and s2[:-1]

    return 1+min(
        calc_conversion(s1, s2[:-1]),
        calc_conversion(s1[:-1], s2),
        calc_conversion(s1[:-1], s2[:-1]))

# dynamic 
def calc_conversion_dp(s1, s2):
    D = {}
    D[(0, 0)] = 0
    for i in range(len(s1)):
        D[(i+1, 0)] = i+1

    for i in range(len(s2)):
        D[(0, i+1)] = i+1

    for i in range(1, len(s1)+1):
        for j in range(1, len(s2)+1):
            if s1[i-1] == s2[j-1]:
                D[(i, j)] = D[(i-1, j-1)]
            else:
                D[(i, j)] = 1+min(D[(i-1, j-1)], D[(i-1, j)], D[(i, j-1)])

    # print D
    return D[len(s1), len(s2)]

if __name__ == "__main__":
    s1 = 'sunday'
    s2 = 'saturday'
    print calc_conversion(s1, s2)
    print calc_conversion(s2, s1)
    print calc_conversion_dp(s1, s2)
    print calc_conversion_dp(s2, s1)
