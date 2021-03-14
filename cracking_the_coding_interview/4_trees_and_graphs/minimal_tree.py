# given a sorted increasing order array with unique integer elements, write
# an algorithm to create a binary search tree with minimal height

from graphs import *

def build_minimal_height_bst(a):
    if not a:
        return None
    a_len = len(a)
    '''
    if a_len == 1:
        return BinaryTreeNode(a[0])
    if a_len == 2:
        root = BinaryTreeNode(a[1])
        root.left = BinaryTreeNode(a[0])
        return root
    if a_len == 3:
        root = BinaryTreeNode(a[1])
        root.left = BinaryTreeNode(a[0])
        root.right = BinaryTreeNode(a[2])
        return root
    '''
    mid = a_len//2
    root = BinaryTreeNode(a[mid])
    root.left = build_minimal_height_bst(a[:mid])
    root.right = build_minimal_height_bst(a[mid+1:])
    return root

    


if __name__ == "__main__":
    build_minimal_height_bst([1, 2, 3, 4, 5, 6])
