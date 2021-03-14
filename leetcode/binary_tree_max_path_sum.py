# given a binary tree, find the maximum path sum
# for this problem, a path is defined as any sequence of nodes from some
# starting node to any node in the tree along the parent child connections
# The path must contain at least one node that does not need to go through
# the root.

class Node(object):
    def __init__(self, val):
        self.val = val
        self.left = None
        self.right = None

        # will update the following two values along the way
        # self.max_leaf_sum, max sum to any leaf from this node(inclusive)
        # self.max_path_sum, max path sum going through this root(inclusive)

#            10
#           /   \
#          15    -2
#         /  \   /  \
#        -1  -2 29   4        
def build_binary_tree1():
    root = Node(10)

    root.left = Node(15)
    root.left.left = Node(-1)
    root.left.right = Node(-2)

    root.right = Node(-2)
    root.right.left = Node(29)
    root.right.right = Node(4)

    return root

def build_binary_tree2():
    root = Node(1)

    root.left = Node(2)
    root.right = Node(3)

    return root


def find_max_path_sum(tree):
    max_path_sum = None

    def traverse(n):
        nonlocal max_path_sum
        
        if n is None:
            if max_path_sum is None or max_path_sum < 0:
                max_path_sum = 0
            return (0, 0, 0) # node_val, leaf_sum, path_sum

        
        left_val, left_leaf_sum, left_path_sum = traverse(n.left)
        right_val, right_leaf_sum, right_path_sum = traverse(n.right)

        leaf_sum = max(left_leaf_sum+n.val, right_leaf_sum+n.val, n.val)
        path_sum = left_leaf_sum+n.val+right_leaf_sum

        if max_path_sum is None or max_path_sum < path_sum:
            max_path_sum = path_sum
        return (n.val, leaf_sum, path_sum)

    _, _, root_path_sum = traverse(tree)
    max_path_sum = max(max_path_sum, root_path_sum)
    
    return max_path_sum


if __name__ == "__main__":
    print(find_max_path_sum(build_binary_tree1()))
    print(find_max_path_sum(build_binary_tree2()))
    




