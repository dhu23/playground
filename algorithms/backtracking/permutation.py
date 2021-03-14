
def generate_all_permutations(s):
    if len(s) in (0, 1):
        return s
    elif len(s) == 2:
        return [s, s[::-1]]

    # when len(s) >= 3
    ret = []
    for i, c in enumerate(s):
        if i == 0: 
            rest = s[1:]
        elif i == len(s)-1:
            rest = s[:-1]
        else:
            rest = s[:i]+s[i+1:]
        
        #print 'rest=', rest
        ret.extend([c+x for x in generate_all_permutations(rest)])

    return ret


def verify(c_list, pos):
    assert 0 <= pos < len(c_list)

def swap(c_list, i, j):
    
    verify(c_list, i)
    verify(c_list, j)

    if i == j: 
        return

    c_list[i], c_list[j] = c_list[j], c_list[i]


def permute(c_list, pos):
    verify(c_list, pos)
    
    if pos == len(c_list)-1:
        print ''.join(c_list)
        return
    
    for i in range(pos, len(c_list), 1):
        swap(c_list, pos, i)
        permute(c_list, pos+1)
        swap(c_list, pos, i) # swap back for back-tracking


def print_all_permutations(s):
    c_list = list(s)
    permute(c_list, 0)


if __name__ == "__main__":
    # print generate_all_permutations("abc")
    print_all_permutations('abcdefghijk')
