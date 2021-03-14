# longest palindromic sub-string

# method 1
# a straight forward method that checks for each selected center point,
# (letter or gap), how long a palindrome can be built around it

def build_palindrome(s, c1, c2): # c1, c2 form the center pair
    assert c2 == c1+1 or c2 == c1+2
    if s[c1] != s[c2]:
        return 0, ""
    
    left, right, radius = c1, c2, 1
    while left > 0 or right < len(s)-1:
        if s[left-1] == s[right+1]:
            radius += 1
            left -= 1
            right += 1
        else:
            break

    if c2 == c1+1: # it is abba type
        if radius == 0:
            return 0, ""
        else:
            return radius*2, s[left:right+1]
    else: # it is abcba type
        if radius == 0:
            return 1, s[left+1:right+1]
        else:
            return radius*2+1, s[left:right+1]


    
def lps_method1(s):

    def update_ret(max_len, ret, new_len, new_sub):
        if new_len == max_len:
            ret.append(new_sub)
            return max_len, ret
        elif new_len > max_len:
            return new_len, [new_sub]
        else:
            return max_len, ret
        
    if len(s) <= 1:
        return len(s), s
    
    max_len, ret = 0, []
    for i in range(len(s)):
        if i == 0 or i == len(s)-1: # palindrome len = 1
            max_len, ret = update_ret(max_len, ret, 1, s[i:i+1])
                            
            if i == 0 and s[0] == s[1]: # aa.... type
                max_len, ret = update_ret(max_len, ret, 2, s[0:2])
        else:
            # for each i in the middle
            
            # check for c1=i, c2=i+1 type
            max_len, ret = update_ret(
                max_len, ret, *build_palindrome(s, i, i+1))

            # check for c1=i-1, c2=i+1 type            
            max_len, ret = update_ret(
                max_len, ret, *build_palindrome(s, i-1, i+1))

    return max_len, ret
                

# method 2
# build a table so that  L(cl, i) return an indicator, indicating whether
# this sub-string is a palindrome
# L(cl, i) = whether it is palindrome, whether it has only one letter
def lps_method2(s):
    if len(s) <= 1: return len(s), s

    L = {}
    max_len, max_i = 0, None
    for i in range(len(s)):
        L[(1, i)] = (True, True)
        max_len, max_i = 1, i

    for i in range(len(s)-1): # sub string from i to i+1
        if s[i] == s[i+1]:
            L[(2, i)] = (True, True)
            max_len, max_i = 2, i
        else:
            L[(2, i)] = (False, False)
            
    for cl in range(3, len(s)+1):
        for i in range(0, len(s)-cl+1): # max i can be len(s)-cl 
            # looking at substring from i to i+cl-1, len=cl
            if s[i] == s[i+cl-1]:
                palindrome, identical = L[(cl-2, i+1)]
                if palindrome:
                    L[(cl, i)] = (
                        True, False if not identical else s[i] == s[i+1])
                    if cl > max_len:
                        max_len, max_i = cl, i
                else:
                    L[(cl, i)] = (False, False)
            else:
                L[(cl, i)] = (False, False)
    #return L[(len(s), 0)]
    return max_len, s[max_i:max_i+max_len]
    
if __name__ == "__main__":
    
    s = 'abcdefgfedadexyz'
    print lps_method1(s)
    print lps_method2(s)
    
