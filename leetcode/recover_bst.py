# problem
# two elements of a binary search tree are swapped by mistake
# recover the tree without changing its structure

# a naive solution is to do an inorder traversal and then check which two
# elements are out of order, which will take O(n) space. However a smarter
# solution would go through the tree with inorder traversal without generating
# a second copy of the data

class Node(object):
    def __init__(self, val):
        self.val = val
        self.left = None
        self.right = None

def build_valid_tree1():
    root = Node(2)
    root.left = Node(1)
    root.right = Node(3)

    return root

def build_invalid_tree1():
    root = Node(1)
    root.left = Node(2)
    root.right = Node(3)

    return root

#         valid tree
#              20
#             /  \
#           10    30
#          /  \   / \
#         1  15  25  33
def build_valid_tree2():
    root = Node(20)
    root.left = Node(10)
    root.right = Node(30)
    root.left.left = Node(1)
    root.left.right = Node(15)
    root.right.left = Node(25)
    root.right.right = Node(33)

    return root

#          invalid tree2
#              25
#             /  \
#           10    30
#          /  \   / \
#         1  15  20  33
def build_invalid_tree2():
    root = Node(25)
    root.left = Node(10)
    root.right = Node(30)
    root.left.left = Node(1)
    root.left.right = Node(15)
    root.right.left = Node(20)
    root.right.right = Node(33)

    return root


#          invalid tree3
#              30
#             /  \
#           10    20
#          /  \   / \
#         1  15  25  33
def build_invalid_tree3():
    root = Node(30)
    root.left = Node(10)
    root.right = Node(20)
    root.left.left = Node(1)
    root.left.right = Node(15)
    root.right.left = Node(25)
    root.right.right = Node(33)

    return root

# this is an incorrect implementation and
# would return true for build_invalid_tree2() though it is not bst
def validate_bst_wrong(r):
    if r is None:
        return True

    is_valid_node = ((r.right is None or r.right.val > r.val) and
                     (r.left is None or r.left.val < r.val))

    return (is_valid_node and
            validate_bst_wrong(r.left) and validate_bst_wrong(r.right))


def validate_bst(r, lower, upper):
    if r is None:
        return True
    
    return ((lower is None or lower < r.val) and
            (upper is None or upper > r.val) and 
            (r.left is None or validate_bst(r.left, lower, r.val)) and
            (r.right is None or validate_bst(r.right, r.val, upper)))

# inorder traversal that generates a list of data output
def inorder(r):
    ret = []
    if r is None:
        return ret

    ret.extend(inorder(r.left))
    ret.append(r.val)
    ret.extend(inorder(r.right))

    return ret

# inorder traversal for recovering the bst with 2 swapped nodes
# however due to the use of recursion, it is not of O(1) space usage
first_node = None
second_node = None
prev_node = None

def traverse(r):
    if r is None:
        return

    global first_node
    global second_node
    global prev_node
    
    traverse(r.left)
    if (first_node is None and prev_node is not None and
        prev_node.val > r.val):
        first_node = prev_node

    if (first_node is not None and prev_node is not None and
        prev_node.val > r.val):
        second_node = r

    prev_node = r
    traverse(r.right)
    
def recover_bst_naive(r):
    traverse(r)
    
    global first_node
    global second_node

    first_node.val, second_node.val = second_node.val, first_node.val

# use Morris traversal to replace the regular inorder traversal to achieve
# O(1) space usage requirement
def recover_bst(r):
    raise Exception('not implemented')
    
if __name__ == "__main__":
    print(inorder(build_valid_tree2()))
    print(validate_bst_wrong(build_valid_tree2()))
    print(validate_bst(build_valid_tree2(), None, None))
    
    print(inorder(build_invalid_tree2()))
    print(validate_bst_wrong(build_invalid_tree2()))
    print(validate_bst(build_invalid_tree2(), None, None))

    bad_tree1 = build_invalid_tree3()
    print(inorder(bad_tree1))
    recover_bst_naive(bad_tree1) # use recursive inorder traversal
    print(inorder(bad_tree1))

    
