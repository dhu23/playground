# implement an algorithm to print all valid (e.g. properly opened and closed)
# combinations of n pairs of parenthese

# example:
# input 3
# output ((())), (()()), (())(), ()(()), ()()()

import permutations_with_dups # actually this can't be reused

# this generates valid permuations but invalid parenthese combo
def parens_bad(a):
    if a == 1: return ["()"]
    
    ret = []
    permutations_with_dups.permutation(list('()'*(a-1)), 0, ret)
    print(ret)

def build_combos(s, a, opens, closes, ret):
    assert a >= opens and a >= closes
    assert opens >= closes

    if opens == a and closes == a:
        ret.append(s)
    elif opens == a: # closes < a
        build_combos(s+')', a, opens, closes+1, ret)
    else:
        build_combos(s+'(', a, opens+1, closes, ret)
        if opens > closes:
            build_combos(s+')', a, opens, closes+1, ret)
    
# you can not have more ) than ( out there
def parens(a):
    ret = []
    build_combos("", a, 0, 0, ret)
    return ret

if __name__ == "__main__":
    print(parens(1))
    print(parens(2))
    print(parens(3))
    print(parens(4))
