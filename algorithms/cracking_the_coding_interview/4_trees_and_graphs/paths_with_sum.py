# You are given a binary tree in which each node contains an integer value
# which might be positive or negative. Design an algorithm to count the number
# of paths that sum to a given value. The path does not need to start or
# end at the root or a leaf, but it much go downwards (travelling only from
# parent nodes to child nodes)

# each path n(i) -> n(j) is a subset of root -> n(i) -> n(j)
# therefore S(i, j) = S(r, j) - S(r, i)
# do a DFS, and every node should be visited
# during a visit to node j, there is a definite path (r -> j) and
# we can build an array that each element i corresponds to S(r, i)

from graphs import *
import random
import itertools

class BinaryTreeNodeWithSum(object):
    def __init__(self, val):
        self.val = val
        self.root_sum = None # sum from root to this node
        self.left = None
        self.right = None
        self.visited = False
        self.root_sum = None

class BinaryTreeWithSum(object):

    def __init__(self):
        self.root = None
        # leaf node has at least one null child
        # 1 - left, 2 - right, 0 - both
        self.leaves = set() 
        self.seed = 0

    def random_insert(self, val):
        N = BinaryTreeNodeWithSum
        if self.root is None:
            self.root = N(val)
            self.leaves.add((self.root, 0))
        else:
            random.seed(self.seed)
            self.seed += 10
            # randomness still exists as the id of the object is different
            # from runs to runs
            node_to_append, node_indicator = random.sample(self.leaves, 1)[0]
            if node_indicator == 1: # left side is empty
                node_to_append.left = N(val)
                #print('adding %d to %d left' % (val, node_to_append.val))
                self.leaves.remove((node_to_append, node_indicator))
                self.leaves.add((node_to_append.left, 0))
            elif node_indicator == 2: # right side is empty
                node_to_append.right = N(val)
                #print('adding %d to %d right' % (val, node_to_append.val))
                self.leaves.remove((node_to_append, node_indicator))
                self.leaves.add((node_to_append.right, 0))
            else: # case 0, both sides are empty
                random.seed(self.seed)
                self.seed += 15
                side = random.randint(1, 2) # pick a side
                if side == 1:
                    node_to_append.left = N(val)
                    #print('adding %d to %d left' % (val, node_to_append.val))
                    self.leaves.remove((node_to_append, node_indicator))
                    self.leaves.add((node_to_append, 2))
                    self.leaves.add((node_to_append.left, 0))
                else: # side is 2
                    node_to_append.right = N(val)
                    #print('adding %d to %d right' % (val, node_to_append.val))
                    self.leaves.remove((node_to_append, node_indicator))
                    self.leaves.add((node_to_append, 1))
                    self.leaves.add((node_to_append.right, 0))
                
    def inorder(self):
        bt_inorder(self.root)

# this function only looks at sub-path that ends at the last node
def find_sum1(path, target):
    ans = 0
    if not path:
        return ans
    # path = [9, -8, 8, -6, 5, 3, -2] 
    acc_path = list(itertools.accumulate(path))
    # print('acc', acc_path)
    if acc_path[-1] == target:
        ans += 1

    for i in ((acc_path[-1]-s) for s in acc_path[:-1]):
        if i == target:
            ans += 1
    return ans
    
def find_sum2(node_path, target):
    ans = 0
    if not node_path:
        return ans

    if node_path[-1].root_sum == target:
        ans += 1
        print('valid path', [nd.val for nd in node_path])
        
    for idx, i in enumerate((node_path[-1].root_sum-s.root_sum)
                            for s in node_path[:-1]):
        if i == target:
            ans += 1
            print('valid path', [nd.val for nd in node_path[idx+1:]], i)
    return ans
        
# back tracking way of traversing the tree
def path_sum(root, target):
    ans1, ans2 = 0, 0 # same quantity calculated by 2 different methods
    if root is None:
        return ans1, ans2
    
    path_stack = []
    n = root
    while True: # visiting n right now.
        assert n is not None
        if not n.visited:
            n.visited = True
            if not path_stack:
                n.root_sum = n.val
            else:
                n.root_sum = path_stack[-1].root_sum + n.val
            path_stack.append(n)
            print(list(nd.val for nd in path_stack))
            ans1 += find_sum1((nd.val for nd in path_stack), target)
            ans2 += find_sum2(path_stack, target)
        
        if n.left is not None and not n.left.visited:
            n = n.left
        elif n.right is not None and not n.right.visited:
            n = n.right
        else: # both left and right are None. reaching the bottom
            path_stack.pop()
            if not path_stack:
                break
            n = path_stack[-1]
    return ans1, ans2
    
if __name__ == "__main__":
    btws = BinaryTreeWithSum()

    random.seed(22)
    input_data = random.sample(range(-10, 10), 20)
    print('input', input_data)
    for i in input_data:
        btws.random_insert(i)
    #btws.inorder()
    print('path with sum=10')
    print(path_sum(btws.root, 10))
    find_sum1([], 8)
