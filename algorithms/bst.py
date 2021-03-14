import random

class Node(object):

    def __init__(self, val):
        self.val = val
        self.left = None
        self.right = None


def build_tree():
    root = Node(100)

    root.left = Node(50)
    root.right = Node(200)

    root.left.left = Node(25)
    root.left.right = Node(75)

    root.right.left = Node(150)
    root.right.right = Node(250)

    return root

def inorder_traversal(root):
    if root is None:
        return
    inorder_traversal(root.left)
    print root.val
    inorder_traversal(root.right)

def postorder_traversal(root):
    if root is None:
        return
    postorder_traversal(root.left)
    postorder_traversal(root.right)
    print root.val

def preorder_traversal(root):
    if root is None:
        return
    print root.val
    preorder_traversal(root.left)
    preorder_traversal(root.right)

def postorder_traversal_stack(root):
    pass

def morris_inorder_traversal(root):

    if root is None:
        return

    current = root

    # this method relies on the fact that the right most node of a 
    # binary search tree has no right child.

    # remove all print statements below, what is left is the core 
    # algorithm of Morris traversal, which can be used for inorder, 
    # preorder, postorder traversal
    while current is not None:

        if current.left is None:
            print current.val
            current = current.right # this also goes to the successor
        else:
            right_most = current.left
            while right_most.right is not None \
                    and right_most.right is not current:
                right_most = right_most.right
            # right_most.right is None
            
            # for setting the predecessor->successor(current) connection
            if right_most.right is None:
                right_most.right = current
                current = current.left
            # for removing the predecessor->successor(current) connection
            else: # right_most.right is current
                right_most.right = None
                print current.val
                current = current.right

def morris_preorder_traversal(root):

    if root is None:
        return

    current = root

    while current is not None:

        if current.left is None:
            print current.val
            current = current.right
        else:
            right_most = current.left
            while right_most.right is not None \
                    and right_most.right is not current:
                right_most = right_most.right
            # right_most.right is None
            
            if right_most.right is None:
                right_most.right = current
                print current.val
                current = current.left
            else:
                right_most.right = None
                current = current.right


def morris_postorder_traversal(root):
    
    # this is the alternative method to print a linked list in reversed order
    def reverse_tree(f, t): # f = from, t = to
        if f is t:
            return
        x, y = f, f.right
        while True:
            z = y.right
            y.right = x
            x, y = y, z
            if x is t:
                break

    def print_reverse(f, t):
        reverse_tree(f, t)

        p = t
        while True:
            print p.val
            if p is f:
                break
            p = p.right

        reverse_tree(t, f)

    if root is None:
        return
    
    dump = Node(-999)
    dump.left = root
    current = dump

    while current is not None:

        if current.left is None:
            current = current.right
        else:
            right_most = current.left
            while right_most.right is not None \
                    and right_most.right is not current:
                right_most = right_most.right
            # right_most.right is None
            
            if right_most.right is None:
                right_most.right = current
                current = current.left
            else:
                print_reverse(current.left, right_most)
                right_most.right = None
                current = current.right


if __name__ == "__main__":
    t = build_tree()
    inorder_traversal(t)
    print '==='
    postorder_traversal(t)
    print '==='
    preorder_traversal(t)

    print '-------------'
    morris_inorder_traversal(t)
    print '==='
    morris_postorder_traversal(t)
    print '==='
    morris_preorder_traversal(t)
