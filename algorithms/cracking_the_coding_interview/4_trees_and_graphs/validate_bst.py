# implement a function to check if a binary tree is a binary search tree

from graphs import BinaryTreeNode as N

# valid 
def build1():
    r = N(8)
    r.left = N(4)
    r.right = N(10)

    r.left.left = N(2)
    r.left.right = N(6)

    r.right.right = N(20)
    return r
    

def build2():
    r = N(8)
    r.left = N(4)
    r.right = N(10)

    r.left.left = N(2)
    r.left.right = N(12)
    r.right.right = N(20)
    return r

def check_bst(root, lower=None, upper=None):
    if root is None:
        return True

    if (check_bst(root.left, lower, root.val) and
        check_bst(root.right, root.val, upper) and 
        (lower is None or root.val > lower) and
        (upper is None or root.val < upper)):
        return True
    else:
        return False

# use a global variable to track the last element that was visited
# in an in-order traversal
last_visited = None
def check_bst2(root):
    global last_visited
    if root is None:
        return True

    if not check_bst2(root.left):
        return False
    # check current node
    if last_visited is not None and last_visited >= root.val:
        return False
    last_visited = root.val
    return check_bst2(root.right)
    
    

if __name__ == "__main__":
    print(check_bst(build1()))
    print(check_bst(build2()))

    print(check_bst2(build1()))
    print(check_bst2(build2()))
