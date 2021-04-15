# given a string S and a string T, count the number of distinct subsequences
# of S which equals T

# A subsequence of a string is a new string which is formed from the original
# string by deleting some (can be none) of the characters without disturbing
# the relative positions of the remaining characters. (i.e. 'ACE' is a
# subsequence of 'ABCDE'while 'AEC' is not)

# example S = 'rabbbit', T = 'rabbit', return 3

# this can be solved by backtracking
# for each character in T, find its position in S without violating their
# relative order.

# 0
# 0 1
# 0 1 2
# 0 1 2 3
# 0 1 2 3 5
# 0 1 2 3 5 6 => output
# 0 1 2 4
# 0 1 2 4 5
# 0 1 2 4 5 6 => output
# 0 1 3
# 0 1 3 4
# 0 1 3 4 5
# 0 1 3 4 5 6 => output
# done

def find_distinct_subsequences(s, t):
    stack, ret, t_len = [], [], len(t)
    i = 0 # index of character from string t that's being looking at
    while i >= 0 and i < t_len:
        growing_stack, swapping_after_bt = len(stack) == i, len(stack) == i+1
        assert growing_stack or swapping_after_bt
        found = s.find(t[i], 0 if not stack else stack[-1]+1)

        if found == -1:
            # backtracking, there are two possibilities to come here
            # either on a growing fail, or on a swapping fail
            while found == -1:            
                i -= 1
                # maintain a swaping_after_bt state
                if not stack or i < 0:
                    return ret
                if swapping_after_bt:
                    stack.pop()
                if not stack:
                    return ret
                found = s.find(t[i], stack[-1]+1)
            
        else:
            if growing_stack:
                stack.append(found)
            elif swapping_after_bt:
                stack[-1] = found
            else:
                raise Exception('should not happen')
            if len(stack) == t_len:
                print('found one', stack)
                ret.append(list(stack))
            else:
                i += 1
        

def find_indices(source, start, c):
    ret = []
    found = source.find(c, start)
    while found != -1:
        ret.append(found)
        found = source.find(c, found+1)
    return ret

def find_distinct_subsequence_recursive(s, t, s_idx, t_idx):
    if s_idx >= len(s) or t_idx >= len(t):
        return []
    
    idxs = find_indices(s, s_idx, t[t_idx])
    print(idxs, s_idx, t[t_idx])
    if t_idx == len(t)-1:
        return [[idx] for idx in idxs]
    
    ret = []
    # can be a problem if size of t is large which causes stack overflow
    for idx in idxs:
        ans = find_distinct_subsequence_recursive(s, t, idx+1, t_idx+1)
        
        for each in ans:
            ret.append([idx]+each)
    return ret


# the original question is to find the number of distinct subsequences
# not all the solutions. Use dynamic programming to build up the solution map
# m(i, j) means the # of solutions for s[:i], t[:j]
# then for incremental inputs {s[:i], s[i]}, {t[:j], t[j]}
# m(i+1, j+1) = m(i, j)+m(i, j+1) if s[i] == t[j]
#               m(i, j+1) otherwise
def find_distinct_subsequence_dp(s, t):
    m, s_len, t_len = [], len(s), len(t)
    # for first row when i = 0, meaning s[:i] aka '', it will be 1 only
    # when sub t string is also ''
    m.append([1 if i == 0 else 0 for i in range(t_len+1)])
    # for each empty t string, there is one solution
    # by deleting all characters in s
    for i in range(1, s_len+1): # i from 1 to s_len
        m.append([1 for j in range(t_len+1)]) # only the first 1 is meaningful

    for i in range(1, s_len+1):
        for j in range(1, t_len+1):
            m[i][j] = m[i-1][j]
            if s[i-1] == t[j-1]:
                m[i][j] += m[i-1][j-1]

    return m[s_len][t_len]
    

if __name__ == "__main__":
    print(find_distinct_subsequences('rabbbit', 'rabbit'))

    print(find_indices('abbccaaddacb', 4, 'a'))
    print(find_distinct_subsequence_recursive('rabbbit', 'rabbit', 0, 0))

    print(find_distinct_subsequence_dp('rabbbit', 'rabbit'))
    print(find_distinct_subsequence_dp('acdabefbc', 'ab'))
