# write an algorithm to find the "next" node (ie, in-order successor) of a
# given node in a binary search tree. You many assume that each node has a
# link to its parent

class BSTNode2Way(object):
    def __init__(self, val, parent=None):
        self.val = val
        self.parent = parent
        self.left, self.right = None, None
    def __repr__(self):
        return 'id={},val={},left={},right={},parent={}'.format(
            hex(id(self)),
            self.val,
            hex(id(self.left)) if self.left is not None else 'None',
            hex(id(self.right)) if self.right is not None else 'None',
            hex(id(self.parent)) if self.parent is not None else 'None')

# in-order 1 3 5 9 10 (15) 20 25 (30) 35 40 (45) 50
def build():
    N = BSTNode2Way
    r = N(30)

    r.left = N(15, r)
    r.right = N(45, r)

    r.left.left = N(5, r.left)
    r.left.left.left = N(3, r.left.left)
    r.left.left.right = N(9, r.left.left)

    r.left.left.left.left = N(1, r.left.left.left)
    r.left.left.right.right = N(10, r.left.left.right)
    
    r.left.right = N(20, r.left)
    r.left.right.right = N(25, r.left.right)

    r.right.left = N(35, r.right)
    r.right.left.right = N(40, r.right.left)

    r.right.right = N(50, r.right)

    collection = [
        r.left.left.left.left, # 1
        r.left.left.left, # 3
        r.left.left, #5
        r.left.left.right, # 9
        r.left.left.right.right, # 10
        r.left, # 15
        r.left.right, # 20
        r.left.right.right, # 25
        r, # 30
        r.right.left, # 35
        r.right.left.right, # 40
        r.right, # 45
        r.right.right, # 50
        ]
    return r, collection

def find_min(root):
    n = root
    while n.left:
        n = n.left
    return n

def find_successor(n):
    if n.right:
        return find_min(n.right)

    if n.parent is None:
        return None
    
    # if n is a left child, n's successor is its parent
    # if n is a right child, go up until it becomes a left subtree of
    # a grandparent, then that grandparent is the successor
    # if there is no such grandparent, n is the max. no successor exists

    def is_right_child(n):
        parent = n.parent
        return parent is not None and id(parent.right) == id(n):
               
    curr = n
    while is_right_child(curr):
        curr = curr.parent

    # when this stops, curr might be a left child or the root
    if curr.parent is None: # curr is the root
        return None
    else:
        return curr.parent
    '''
    if id(n.parent.left) == id(n):
        # left child
        return n.parent
    else:
        # right child
        parent = n.parent
        while (parent.parent is not None and
               id(parent.parent.right) == id(parent)):
            parent = parent.parent
        if parent.parent is None:
            return None
        else:
            return find_min(parent.parent.right)
    '''

if __name__ == "__main__":
    root, collection = build()
    for each in collection:
        print('==================')
        print(each)
        print(find_successor(each))
