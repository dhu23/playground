# design an algorithm and write code to find the first common ancestor of
# two nodes in a binary tree. Avoid storing additional nodes in a data
# structure. NOTE: this is not necessarily a binary search tree

from graphs import *

def build():
    N = BinaryTreeNode
    r = N(1)
    r.left = N(2)
    r.right = N(3)
    r.left.left = N(4)
    r.left.right = N(5)
    r.right.left = N(6)
    r.right.left.left = N(7)
    r.right.left.right = N(8)
    r.left.right.left = N(9)
    r.left.right.right = N(10)
    return r


# storing nodes in extra data structure
def stack_to_vals(stack):
    return [s.val for s in stack]        

def get_path(root, val, stack, path):
    if path or root is None:
        return

    stack.append(root)
    # print(stack_to_vals(stack))
    if stack[-1].val == val:
        path.extend(stack_to_vals(stack))
        return

    get_path(root.left, val, stack, path)
    if path:
        return
    get_path(root.right, val, stack, path)
    if path:
        return
    stack.pop()
    
def find_ancestors(root, val):
    stack, path = [], []
    get_path(root, val, stack, path)
    # print(path)
    return path

def find_first_common_ancestor(root, n1, n2):
    ancestors1, ancestors2 = find_ancestors(root, n1), find_ancestors(root, n2)
    print(ancestors1, ancestors2)
    commons = [x for (x, y) in zip(ancestors1[:-1], ancestors2[:-1]) if x == y]
    if commons:
        return commons[-1]
    else:
        return None


def get_path0(root, val):
    if root is None:
        print('get_path0:searching none for', val)
        return None
    print('get_path0:searching %d for %d' % (root.val, val))
    if root.val == val:
        return root

    found = get_path0(root.left, val)
    if found:
        return found
    found = get_path0(root.right, val)
    if found:
        return found

    return found

def search_val2(root, val2, found_val2):

    if found_val2[0]: # if both are found
        return
    else:
        # DFS to search val2 from current root
        found = get_path0(root, val2)
        if found:
            found_val2[0] = True
            print("found common ancestor", root.val)

# this prints out the common ancestor
def get_path1(root, val1, val2, found_val1, found_val2):
    if root is None:
        print('searching at None root', val1, val2, found_val1, found_val2)
    else:
        print('searching at', root.val, val1, val2, found_val1, found_val2)
    if root is None or (found_val1[0] and found_val2[0]):
        return

    if (not found_val1[0] and
        ((root.left is not None and root.left.val == val1) or
         (root.right is not None and root.right.val == val1))):
        print('found va1', root.val, val1)
        found_val1[0] = True
        
    if found_val1[0]:
        search_val2(root, val2, found_val2)
    else:
        # if val1 still found at current level, go further down
        get_path1(root.left, val1, val2, found_val1, found_val2)
        # found val1 from root.left
        if found_val1[0]:
            search_val2(root, val2, found_val2)
        else:
            # only if val1 is not found from left side, go right side
            get_path1(root.right, val1, val2, found_val1, found_val2)

# method 3            
# return null if one of n1/n2 is not in root's subtree
# return the common ancestor if both of them are
def find_first_common_ancestor2(root, n1, n2):
    if root is None:
        return None, False, False # common ancestor, n1 in root, n2 in root
    if root.val == n1 and root.val == n2:
        return root, True, True
        
    c_left, n1_on_left, n2_on_left = find_first_common_ancestor2(
        root.left, n1, n2)

    if root.val == n1:
        # check n2 under root
        if n2_on_left is True:
            return root, True, True
    elif root.val == n2:
        # check n1 under root
        if n1_on_left is True:
            return root, True, True
    else:
        if n1_on_left and n2_on_left:
            assert c_left is not None
            return c_left, True, True

    c_right, n1_on_right, n2_on_right = find_first_common_ancestor2(
        root.right, n1, n2)

    if root.val == n1:
        if n2_on_right:
            return root, True, True
    elif root.val == n2:
        if n1_on_right:
            return root, True, True
    else:
        if n1_on_right and n2_on_right:
            assert c_right is not None
            return c_right, True, True

    # cases when n1 and n2 are under two different subtrees
    if n1_on_left and n2_on_right:
        assert not n1_on_right and not n2_on_left
        return root, True, True
    elif n1_on_right and n2_on_left:
        assert not n1_on_left and not n2_on_right
        return root, True, True

    # all other cases
    return (
        None,
        root.val == n1 or n1_on_left or n1_on_right,
        root.val == n2 or n2_on_left or n2_on_right)


if __name__ == "__main__":
    root = build()
    print(find_first_common_ancestor(root, 4, 8))
    print(find_first_common_ancestor(root, 5, 9))
    print(find_first_common_ancestor(root, 4, 5))


    print('======================')
    f1, f2 = [False], [False]
    get_path1(root, 4, 8, f1, f2)


    print('=======================')
    def find_first_common_ancestor2_helper(root, n1, n2):
        r, n1_in_tree, n2_in_tree = find_first_common_ancestor2(root, n1, n2)
        print(r.val, n1_in_tree, n2_in_tree)

    find_first_common_ancestor2_helper(root, 4, 8)
    find_first_common_ancestor2_helper(root, 5, 9)
    find_first_common_ancestor2_helper(root, 4, 5)
