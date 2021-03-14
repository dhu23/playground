# implement a function to check if a binary tree is balanced. For the
# purposes of this question, a balanced tree is defined to be a tree such that
# the heights of the two subtrees of any node never differ by more than one

from graphs import *


def check_balance(root):
    if root is None:
        return True, 0

    left_balanced, left_height = check_balance(root.left)
    right_balanced, right_height = check_balance(root.right)

    height = max(left_height, right_height)+1
    if left_balanced and right_balanced and abs(left_height-right_height) <= 1:
        return True, height
    else:
        return False, height

def build1():
    root = BinaryTreeNode(1)
    root.left = BinaryTreeNode(2)
    root.right = BinaryTreeNode(3)
    root.left.left = BinaryTreeNode(4)
    root.left.left.left = BinaryTreeNode(5)
    return root


def build2():
    root = BinaryTreeNode(1)
    root.left = BinaryTreeNode(2)
    root.right = BinaryTreeNode(3)
    return root
    
if __name__ == "__main__":
    print(check_balance(build1()))
    print(check_balance(build2()))

    
