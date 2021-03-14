#               1
#          2         3
#        1   2     2   4
#       5 6 2       2
# snake traversal : result should be
# 1 | 3 2 | 1 2 2 4 | 2 2 5 6


class Node(object):
    def __init__(self, val):
        self.val = val
        self.left, self.right = None, None

    def __repr__(self):
        return 'Node(%s), val=%s, left=%r, right=%r' % (
            hex(id(self)), self.val,
            None if self.left is None else repr(self.left),
            None if self.right is None else repr(self.right))


# add one [1]
# left to right traversal ====> 
# remove 1(pop front), add children(push back) [2, 3]
# right to left traversal ====>
# remove 3(pop back), add children(push front) [2, 4, 2]
# remote right 2(pop back), add children(push front) [1, 2, 2, 4]
# left to right traversal ====>
# remove 1(pop front), add children(push back) [2, 2, 4, 5, 6]
# remove 2, add children [2, 4, 5, 6, 2]
# remove 2, add children [4, 5, 6, 2, 2]
# remote 4, no children to add [5, 6, 2, 2]
# right to left traversal ====>
# ....
def traverse_snake(root):
    pass
    
if __name__ == "__main__":
    r = Node(1)
    rl = Node(2)
    r.left = rl
    rr = Node(3)
    r.right = rr

    print(repr(r))
