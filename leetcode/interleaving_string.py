# given s1, s2, s3 find whether s3 is formed by the interleaving of s1 and s2
# for example:
# s1 = "aabcc"
# s2 = "dbbca"

# when s3 = "aadbbcbcac", return true
# when s3 = "aadbbbaccc", return false

# backtracking
def check_interleaving_bt(s1, s2, s3):
    s1_len, s2_len, s3_len = len(s1), len(s2), len(s3)
    assert s1_len+s2_len == s3_len
    stack = [] # storing 0 and 1, like [0, 0, 1, 1, 1]
    # 0 means that position is filled by a character from s1, otherwise s2
    # prefer s1 over s2, meaning during backtracking, if the position is
    # already 1, then it would trigger more backtracking
    s1_idx, s2_idx = 0, 0
    
    while True:
        ''' 
        if s1_idx >= s1_len and s2_idx < s2_len:
            # s1 is exhausted
            if s2[s2_idx:] == s3[s1_idx+s2_idx:]:
                return True
        elif s2_idx >= s2_len and s1_idx < s1_len:
            # s2 is exhausted
            if s1[s1_idx:] == s3[s1_idx+s2_idx:]:
                return True
        elif s2_idx >= s2_len and s1_idx >= s1_len:
            raise Exception('should not happen')
        '''

        if s1_idx < s1_len and s1[s1_idx] == s3[s1_idx+s2_idx]:
            s1_idx += 1
            stack.append(0)

            if s1_idx == s1_len and s2_idx == s2_len:
                return True
            
        elif s2_idx < s2_len and s2[s2_idx] == s3[s1_idx+s2_idx]:
            s2_idx += 1
            stack.append(1)

            if s1_idx == s1_len and s2_idx == s2_len:
                return True
        else:
            # backtracking
            while stack and stack[-1] == 1:
                stack.pop()
                s2_idx -= 1

            # after backtracking, there is no feasible solutions left
            if not stack:
                return False
            else:
                assert stack[-1] == 0
                stack[-1] = 1
                s1_idx -= 1
                s2_idx += 1


# dynamic programing solution, and build a table which represents
# T(i, j) = for s1[:i], s2[:j], check if s3[:i+j] is an interleaving string
def check_interleaving_dp(s1, s2, s3):
    s1_len, s2_len, s3_len = len(s1), len(s2), len(s3)
    if s1_len+s2_len != s3_len:
        return False
    # each row represents all possible i's, from 0 to s1_len
    # each column represents all possible j's, from 0 to s2_len
    # if s1_len is zero, check s2[:j] == s3[:j]
    # if s2_len is zero, check s1[:i] == s3[:i]
    # if both zero, return True
    # T(i, j) = if s1[i-1] matches s3[i+j-1], return T(i-1, j)
    #           if s2[j-1] matches s3[i+j-1], return T(i, j-1)
    #           otherwise False
    t = []
    for i in range(0, s1_len+1):
        t.append([False for j in range(0, s2_len+1)])
    #print(t)
    for i in range(0, s1_len+1):
        for j in range(0, s2_len+1):
            if i == 0 and j == 0:
                t[i][j] = True
            elif i == 0:
                t[i][j] = s2[:j] == s3[:j]
            elif j == 0:
                t[i][j] = s1[:i] == s3[:i]
            else:
                t[i][j] = ((s1[i-1] == s3[i+j-1] and t[i-1][j]) or
                           (s2[j-1] == s3[i+j-1] and t[i][j-1]))
    #print(t)
    return t[s1_len][s2_len]


if __name__ == "__main__":
    s1 = 'aabcc'
    s2 = 'dbbca'

    print(check_interleaving_bt(s1, s2, 'aadbbcbcac'))
    print(check_interleaving_bt(s1, s2, 'aadbbbaccc'))

    print(check_interleaving_dp(s1, s2, 'aadbbcbcac'))
    print(check_interleaving_dp(s1, s2, 'aadbbbaccc'))
