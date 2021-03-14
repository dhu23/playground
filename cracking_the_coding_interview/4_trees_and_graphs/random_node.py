# you are implementing a binary search tree from scratch which in addition
# to insert, find, and delete, has a method getRandomNode() which returns
# a random node from the tree. All nodes should be equality likely to be
# chosen. Design and implement an algorithm for getRandomNode. And explain
# how you would implement the rest of the methods.

from graphs import *
import random

N = BinaryTreeNode

def insert(root, val):
    # print('inserting')
    if root is None:
        root = N(val)
        return root, root

    if root.val > val: # go left
        root.left, added = insert(root.left, val)
    elif root.val < val: # go right
        root.right, added = insert(root.right, val)
    else:
        pass # do nothing
    return root, added

def find(root, val):
    if root is None:
        return None

    if root.val < val: # go right
        return find(root.right, val)
    elif root.val > val: # go left
        return find(root.left, val)
    else:
        print('found val=', val)
        return root

def find_max(root):
    if root is None:
        return None
    n = root
    while n.right is not None:
        n = n.right
    return n

def find_min(root):
    if root is None:
        return None
    n = root
    while n.left is not None:
        n = n.left
    return n
    
def delete(root, val):
    if root is None:
        return root, None # nothing to delete
    if root.val < val: # go right
        root.right, deleted = delete(root.right, val)
        return root, deleted
    elif root.val > val: # go left
        root.left, deleted = delete(root.left, val)
        return root, deleted
    else:
        if root.left is None and root.right is None:
            # delete this leaf node
            return None, root

        if root.left is not None:
            left_max = find_max(root.left)
            assert left_max is not None            
            left_max.val, root.val = root.val, left_max.val
            root.left, deleted = delete(root.left, left_max.val)
            return root, deleted
        else:
            assert root.right is not None
            right_min = find_min(root.right)
            assert right_min is not None
            right_min.val, root.val = root.val, right_min.val
            root.right, deleted = delete(root.right, right_min.val)
            return root, deleted
        
# not the best the method 1) when (min, max) range is huge and nodes are few
# 2) when nodes value are not numerical
def get_random_node_naive(root):
    if root is None:
        raise Exception('Cannot get random node from an empty tree')
    min_node, max_node = find_min(root), find_max(root)
    if min_node.val == max_node.val:
        return root
    while True:
        guess = random.randint(min_node.val, max_node.val)
        guess_node = find(root, guess)
        if guess_node is not None:
            return guess_node

# a binary search tree that supports random node
class RandomBinarySearchTree(object):
    def __init__(self):
        self.root = None
        self.nodes = set()

    def insert(self, val):
        self.root, added = insert(self.root, val)
        self.nodes.add(added)

    def find(self, val):
        return find(self.root, val)

    def delete(self, val):
        self.root, deleted = delete(self.root, val)
        if deleted is not None:
            assert deleted in self.nodes
            self.nodes.remove(deleted)

    def random_node(self):
        return random.sample(self.nodes, 1)[0]
            
    def inorder(self):
        bt_inorder(self.root)
        
if __name__ == "__main__":
    r = N(30)

    print(insert(r, 20))
    print(insert(r, 45))

    print(find(r, 25))
    print(find(r, 45))

    insert(r, 15)
    insert(r, 44)

    bt_inorder(r)
    
    print(find(r, 44))
    r, node44 = delete(r, 44)
    assert node44.val == 44

    bt_inorder(r)
    
    insert(r, 33)
    print('44 is already deleted')
    print(find(r, 44))
    print(find(r, 33))

    bt_inorder(r) # 15, 20, 30, 33, 45
    r, _ = delete(r, 30)
    print('30 is already deleted')
    bt_inorder(r) # 15, 20, 33, 45
    r, _ = delete(r, 20)
    r, _ = delete(r, 45)
    r, _ = delete(r, 15)
    assert r.val == 33 and r.left is None and r.right is None

    bt_inorder(r) # 33
    r, _ = delete(r, 33)

    assert r is None

    print('testing RandomBinarySearchTree')
    rbst = RandomBinarySearchTree()
    rbst.insert(30)
    rbst.insert(25)
    rbst.insert(45)
    rbst.inorder()
    rbst.delete(25)
    print('---')
    rbst.inorder()

    rbst2 = RandomBinarySearchTree()
    for i in range(20):
        rbst2.insert(i)

    rbst2.inorder()

    import collections
    m = collections.Counter(rbst2.random_node().val for i in range(5000))
    print(m)
