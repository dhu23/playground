# write a method to compute all permutations of a string whose characters
# are not necessarily unique. The list of permutations should not have dups

import itertools
import pprint

def permutation(s, start, ret):

    if start == len(s)-1:
        print(s)
        ret.append(s)

    tried = set()
    for i in range(start, len(s)):
        #if start != i and s[start] == s[i]:
        #    continue

        s[start], s[i] = s[i], s[start]
        if s[start] not in tried:
            tried.add(s[start])
            permutation(s, start+1, ret)
        s[start], s[i] = s[i], s[start]
        
    

if __name__ == "__main__":
    ret = []
    permutation(list('abac'), 0, ret)
    print(ret)
    print('right answer=')
    pprint.pprint(set(itertools.permutations('abac')))

    print('extreme dup case')
    ret = []
    # inefficient algo would take forever
    # here it is instant
    permutation(list('aaaaaaaaaaaaaaaa'), 0, ret) 
    print(ret)


    print('=====')
    permutation(list('()()'), 0, [])
