# given a boolean expression consisting of the symbols 0(false), 1(true),
# &(AND), |(or), ^(XOR) and a desired boolean result value result. implement
# a function to count the number of ways of parenthesizing the expression
# such that it evaluates to result

# countEval("1^0|0|1", false) -> 2
# countEval("0&0&0&1^1|0", true) -> 10

import pprint

def count_right(left, oprand, right, flag, val):
    if (eval(left+oprand+str(val)) == flag):
        return count_eval_recursive(right, val)
    else:
        return 0

def count_left(left, oprand, right, flag, val):
    if (eval(str(val)+oprand+right) == flag):
        return count_eval_recursive(left, val)
    else:
        return 0

def count_both(left, oprand, right, flag, val_left, val_right):
    if (eval(str(val_left)+oprand+str(val_right)) == flag):
        return (count_eval_recursive(left, val_left) *
                count_eval_recursive(right, val_right))
    else:
        return 0

def count_eval_recursive(s, flag):
    s_len = len(s)
    assert s_len >= 3
    if s_len == 3: return int(eval(s) == flag)
    # multiple ways of doing this, for example
    # generally, s[:i], s[i], s[i+1:] ==> flag
    count = 0
    for i in range(1, s_len, 2):
        left, oprand, right = s[:i], s[i], s[i+1:]
        assert oprand in ('|', '^', '&')
        if len(left) < 3:
            count += count_right(left, oprand, right, flag, True)
            count += count_right(left, oprand, right, flag, False)
        elif len(right) < 3:
            count += count_left(left, oprand, right, flag, True)
            count += count_left(left, oprand, right, flag, False)
        else:
            count += count_both(left, oprand, right, flag, True, True)
            count += count_both(left, oprand, right, flag, True, False)
            count += count_both(left, oprand, right, flag, False, False)
            count += count_both(left, oprand, right, flag, False, True)
                
    #print(('==== %s , %s, %d ====') % (s, flag, count))
    return count


# can use dynmaic programming to make it more efficient
# build a table table(i, j, flag) where j >= i where i, j are even
# it corresponds to the number of ways to "flag" using s[i:j+1]
# construction rule:
# table(i, j, flag) = for k = i+1, i+2, ... j-1
#                     if s[i:k], s[k], s[k+1:j+1] ==> flag
#                     table(i, k-1, flag1) * table(k+1, j, flag2)
def count_eval(s, flag):
    assert all(c in ('^', '&', '|') for i, c in enumerate(s) if i%2 != 0)
    table, s_len = {}, len(s)
    for l in range(1, s_len+1, 2): # length of the substring
        for i in range(0, s_len, 2):
            if i+l-1 > s_len-1: continue # we have already visited it
            j = i+l-1
            #print('evaling=', i, j, s[i:j+1])
            if l == 1:
                sub_s = s[i:j+1]
                table[(i, j, True)] = int(eval(sub_s) == True)
                table[(i, j, False)] = int(eval(sub_s) == False)
            else:
                count_true, count_false = 0, 0
                for k in range(i+1, j, 2):
                    tries = [
                        (True, True), (True, False),
                        (False, True), (False, False)]
                    for flag_left, flag_right in tries:
                        sub_count = (table[(i, k-1, flag_left)] *
                                     table[(k+1, j, flag_right)])
                        ss = (str(flag_left)+s[k]+str(flag_right))
                        if eval(str(flag_left)+s[k]+str(flag_right)) == True:
                            count_true += sub_count
                        else: # is false
                            count_false += sub_count
                table[(i, j, True)] = count_true
                table[(i, j, False)] = count_false
    #pprint.pprint(table)
    return table[(0, s_len-1, flag)]


if __name__ == "__main__":
    print(count_eval_recursive("0&1^1|0", True))
    print(count_eval_recursive("1^0|0|1", False))
    print(count_eval_recursive("0&0&0&1^1|0", True))

    print(count_eval("0&1^1|0", True))
    print(count_eval("1^0|0|1", False))
    print(count_eval("0&0&0&1^1|0", True))


    
