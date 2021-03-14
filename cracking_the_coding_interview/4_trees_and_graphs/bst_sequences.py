# a binary search tree was create by traversing through an array from left
# to right and insert each element. Given a binary search tree, with distinct
# elements, print all possible arrays that could have led to this tree

# example:
# input           2
#               /   \
#              1     3
# output (2, 1, 3), (2, 3, 1)

# given a tree structure
# data Tree a = Leaf | Node a (Tree a) (Tree a)
# due to the fact it is a binary search tree, the list order of two elements
# belonging to left and right sides is irrelevant because the tree structure
# invariant can guarantee them to go to the right place
# say there are N(left) nodes on the left side and N(right) nodes on the right
# each left order

from graphs import *
import itertools

def equivalent(a1, a2):
    return isomorphic(build_bst(a1), build_bst(a2))

def bst_sequences(bst):
    if bst is None:
        return [[]]

    if bst.left is None and bst.right is None:
        return [[bst.val]]
    elif bst.left is None:
        ret = []
        for each in bst_sequences(bst.right):
            ret.append([bst.val]+each)
        return ret
    elif bst.right is None:
        ret = []
        for each in bst_sequences(bst.left):
            ret.append([bst.val]+each)
        return ret
    else:
        ret = []
        left_ret, right_ret = bst_sequences(bst.left), bst_sequences(bst.right)
        for left_seq, right_seq in itertools.product(left_ret, right_ret):
            # with two seqences, make a group of new ones that
            # combine the two without changing any of the relative orders
            # this is interleaving strings
            interleaves = interleave_sequences(left_seq, right_seq)
            for i in interleaves:
                i.insert(0, bst.val)
            ret.extend(interleaves)
        return ret
        

# return all possible solutions by backtracking
def interleave_sequences(s1, s2):
    ret = []
    s1_len, s2_len = len(s1), len(s2)
    stack = [] # storing list of (data, source)
    i, j = 0, 0
    end_of_proc = False
    while True:
        if len(stack) == s1_len+s2_len:
            ret.append([s[0] for s in stack])
            # trigger backtracking
            while not (stack[-1][1] == 0 and j < s2_len):
                if stack[-1][1] == 0:
                    i -= 1
                if stack[-1][1] == 1:
                    j -= 1
                stack.pop()
                if not stack:
                    end_of_proc = True
                    break
            if end_of_proc:
                break
            
            assert stack[-1][1] == 0
            stack.pop()
            i -= 1
            stack.append((s2[j], 1))
            j += 1
        if i < s1_len:
            stack.append((s1[i], 0))
            i += 1
        elif j < s2_len:
            stack.append((s2[j], 1))
            j += 1
    return ret

def interleave_sequences_recursion(s1, s2):
    if not s1:
        return [s2]
    if not s2:
        return [s1]
    ret = []
    for i in interleave_sequences_recursion(s1[1:], s2):
        ret.append([s1[0]]+i)
    for i in interleave_sequences_recursion(s1, s2[1:]):
        ret.append([s2[0]]+i)
    return ret

if __name__ == "__main__":
    print(equivalent([2, 1, 3], [2, 3, 1]))
    print(interleave_sequences([1, 2, 3], [4, 5, 6]))
    tree_source = [4, 2, 1, 3, 6, 5, 7]
    all_possible_ways = bst_sequences(build_bst(tree_source))
    print(all_possible_ways)
    
    for each_possibility in all_possible_ways:
        assert equivalent(each_possibility, tree_source)

    good_ones, bad_ones = [], []
    for each_permutation in itertools.permutations(tree_source,
                                                   len(tree_source)):
        if equivalent(each_permutation, tree_source):
            good_ones.append(each_permutation)
        else:
            bad_ones.append(each_permutation)

    print(len(good_ones), len(bad_ones), len(all_possible_ways))

    interleaves0 = list(interleave_sequences_recursion([1, 2, 3], [4, 5, 6]))
    interleaves1 = list(interleave_sequences([1, 2, 3], [4, 5, 6]))
    print(len(interleaves0), len(interleaves1))
