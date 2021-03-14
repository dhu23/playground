# T1 and T2 are two very large binary trees, with T1 much bigger than T2
# Create an algorithm to determine if T2 is a subtree of T1

# A tree T2 is a subtree of T1 if there exist a node n in T1 such that the
# subtree of n is identical to T2. That is if you cut off the tree at node
# n, the two trees would be identical

from graphs import *

# Even very large binary trees should still work with recursive function

def build_small_tree():
    N = BinaryTreeNode
    r = N(10)
    r.left = N(5)
    r.right = N(15)
    r.left.left = N(6)
    r.left.right = N(3)
    r.left.right.right = N(12)

    return r

def build_big_tree():
    N = BinaryTreeNode
    r = N(30)
    r.left = N(25)
    r.right = N(45)
    r.right.right = N(55)
    r.right.left = N(32)

    r.right.left.right = build_small_tree()
    return r

# doing quite some duplicated work
def check_subtree_naive(big_tree, small_tree):
    if big_tree is None:
        return False

    if isomorphic(big_tree, small_tree):
        return True

    return (check_subtree(big_tree.left, small_tree) or
            check_subtree(big_tree.right, small_tree))

# can we build a DFA to solve this just like KMP algorithm?


if __name__ == "__main__":
    print(check_subtree_naive(build_big_tree(), build_small_tree()))
