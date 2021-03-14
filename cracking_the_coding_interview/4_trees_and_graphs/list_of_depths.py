# given a binary tree, design a algorithm which creates a linked list of all
# the nodes at each depth(eg if you have a tree with depth D, you will have
# D linked lists)

from graphs import *

def build():
    root = BinaryTreeNode(10)
    root.left = BinaryTreeNode(5)
    root.right = BinaryTreeNode(15)
    root.left.left = BinaryTreeNode(20)
    root.left.right = BinaryTreeNode(16)
    #root.right.left = BinaryTreeNode(11)
    root.right.right = BinaryTreeNode(3)
    return root

class LinkedListNode(object):
    def __init__(self, val):
        self.val = val
        self.next = None
    def __repr__(self):
        return 'id={0},val={1},next={2}'.format(
            id(self), self.val, id(self.next))

def print_list(head):
    n = head
    while n is not None:
        print(n)
        n = n.next

        
# BFS   
def to_linked_list(root):
    q = [(root, 0)]
    ret = {} # depth mapped to linked list

    while q:
        n, depth = q.pop(0)
        head = LinkedListNode(n.val)
        if depth in ret:
            head.next = ret[depth]
        ret[depth] = head

        if n.right:
            q.append((n.right, depth+1))
        if n.left:
            q.append((n.left, depth+1))
            
    return ret

# tree level starts at 0 with root
# pre-order traversal
def to_linked_list2_helper(root, ret, level):
    if root is None:
        return

    head = LinkedListNode(root.val)
    if level in ret:
        head.next = ret[level]
    ret[level] = head

    # traverse right side first so that the result linked list starts from left
    to_linked_list2_helper(root.right, ret, level+1)
    to_linked_list2_helper(root.left, ret, level+1)
    
def to_linked_list2(root):
    ret = {}
    to_linked_list2_helper(root, ret, 0)
    return ret
    
            
if __name__ == "__main__":


    def print_results(results):
        for d, ll in results.items():
            print('---------- depth=%d ------------' % d)
            print_list(ll)

    results = to_linked_list(build())
    print_results(results)
    
    results = to_linked_list2(build())
    print_results(results)
