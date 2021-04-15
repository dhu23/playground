class GraphNode(object):
    def __init__(self, val):
        self.val = val
        self.children = []

class Graph(object):
    def __init__(self):
        self.nodes = []

class BinaryTreeNode(object):
    def __init__(self, val):
        self.val = val
        self.left = None
        self.right = None
        

# g is a map of nodes to a list of nodes
def dfs(g, r, visited):
    visited.add(r)
    if r in g:
        for n in g[r]:
            if n not in visited:
                dfs(g, n, visited)
          
def isomorphic(tree1, tree2):
    if tree1 is None and tree2 is None:
        #print('both root none: isomorphic')
        return True
    elif tree1 is None or tree2 is None:
        #print('one is none: not isomorphic')
        return False
    elif tree1.val != tree2.val:
        #print('root not equal, %d!=%d' % (tree1.val, tree2.val))
        return False
    else:
        return (isomorphic(tree1.left, tree2.left) and
                isomorphic(tree1.right, tree2.right))


def add_node_bst(root, val):
    if root is None:
        return BinaryTreeNode(val)
    if root.val == val:
        return root
    elif root.val < val:
        root.right = add_node_bst(root.right, val)
    else:
        root.left = add_node_bst(root.left, val)
    return root
        
def build_bst(a):
    if not a:
        return None

    N = BinaryTreeNode
    root = N(a[0])

    for i in a[1:]:
        add_node_bst(root, i)

    return root

def bt_inorder(root):
    if root is None:
        return

    bt_inorder(root.left)
    print(root.val)
    bt_inorder(root.right)
