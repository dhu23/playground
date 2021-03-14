# leetcode #3

# 'abcabcbb' -> abc
# 'bbbbb' -> b
# 'pwwkew' -> wke

# char map that contains full information of the string: 
# a -> (0, 3) b -> (1, 4, 6, 7) c -> (2, 5)

import random
import datetime
import math

def build_radix_map(s):
    rm = {}
    for i, c in enumerate(s):
        if c in rm:
            rm[c].append(i)
        else:
            rm[c] = [i]
    return rm


def find_longest_at(s, radix_map, i, char_curr_pos):
    # compute by using radix_map, the largest index j, 
    # so that [i, j] doesn't cover any character in radix_map twice
    for k, pos_list in radix_map.iteritems():
        # print k, pos_list, char_curr_pos[k]
        # update char_curr_pos to the pos right after or at i
        while pos_list[char_curr_pos[k]] < i:
            if char_curr_pos[k] < len(pos_list)-1:
                char_curr_pos[k] += 1
            else: 
                break
    pos_min = None    
    for k, pos_list_index in char_curr_pos.iteritems():
        pos_list = radix_map[k]
        pos_next = len(s)-1 \
                if pos_list_index+1 >= len(pos_list) \
                else pos_list[pos_list_index+1]-1
        if pos_min is None or pos_next < pos_min:
            pos_min = pos_next

    # print char_curr_pos
    return pos_min

# l is the length
def find_longest_unique_substring(s, radix_map):
    char_curr_pos = dict([(k, 0) for k in radix_map])
    longest = None
    for i in range(len(s)):
        j = find_longest_at(s, radix_map, i, char_curr_pos)
        if j-i+1 > longest or longest is None:
            longest = j-i+1
        # print 'i=%d,j=%d,substr=%s,len=%d' % (i, j, s[i:j+1], j-i+1)
    return longest

# dynamic programming method. 
# after calculating longest substring at i, when we calculate longest 
# substring at i+1, some of the work would be repeated. say longest string
# at i+1 ends at j, written as
# L(i+1) := j
# then L(i) := min(j, radix_map(next s[i]))
# build up L backwards from the end of the string s


def test1(s):
    start = datetime.datetime.now()
    radix_map = build_radix_map(s)
    # print find_longest_at(s, radix_map, 0, dict([(k, 0) for k in radix_map]))
    longest = find_longest_unique_substring(s, radix_map)
    cost = (datetime.datetime.now()-start).total_seconds()
    print cost, longest
    return cost

def complexity_test():
    
    alphabeta = 'abcdefghijklmnopqrstuvwxyz'

    def generate_array(N):
        random.seed(123)
        return ''.join([alphabeta[each%26] \
                for each in random.sample(range(100*N), N)])
    
    ns, costs = [], []
    for size in [10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000]:
        costs.append(test1(generate_array(size)))
        ns.append(size)

    print ns
    print costs
    import numpy
    from scipy import stats
    print stats.linregress(numpy.log(ns), numpy.log(costs))

if __name__ == "__main__":
    test1('abcabcbb')
    test1('bbbbb')
    test1('pwwkew')

    complexity_test()
