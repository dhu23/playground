# imagine you are reading in a stream of integers. Periodically you wish to be
# able to look up the rank of a number x(the number of values less than or
# equal to x). Implement the data structures and algorithms to support these
# operations. That is, implement the method track(int x), which is called when
# each number is generated, and the method getRankOfNumber(int x), which
# returns the number of values less than or equal to x(not including itself)

# example
# stream (in order of appearence): 5, 1, 4, 4, 5, 9, 7, 13, 3
# getRankOfNumber(1) = 0
# getRankOfNumber(3) = 1
# getRankOfNumber(4) = 3

# can this tree rebalance itself AT ALL??
class Node(object):
    def __init__(self, val):
        self.val = val
        self.left = None
        self.right = None
        self.count = 1 # number of elements under the node including itself

    def insert(self, x):
        if x > self.val:
            if self.right is None:
                self.right = Node(x)
            else:
                self.right.insert(x)
        elif x < self.val:
            if self.left is None:
                self.left = Node(x)
            else:
                self.left.insert(x)
        else:
            pass
        self.count += 1
        
            
    def rank(self, x):
        rank = 0
        if x > self.val:
            rank += self.count
            if self.right is not None:
                rank -= self.right.count
                rank += self.right.rank(x)
            else:
                rank += 1
        elif x < self.val:
            if self.left is not None:
                rank += self.left.rank(x)
        else: # x == self.val
            rank = self.count
            if self.right is not None:
                rank -= self.right.count
        return rank

    def inorder(self):
        if self.left is not None:
            self.left.inorder()
        print('val=%s,count=%d' % (self.val, self.count))
        if self.right is not None:
            self.right.inorder()

class Tracker(object):
    def __init__(self):
        self.data = None

    def insert(self, x):
        if self.data is None:
            self.data = Node(x)
            return
        self.data.insert(x)

    def get_rank(self, x):
        if self.data is None:
            return 0
        return self.data.rank(x)-1

    def inorder(self):
        if self.data is None:
            print("Null tree")
        else:
            self.data.inorder()

if __name__ == "__main__":
    seq = [5, 1, 4, 4, 5, 9, 7, 13, 3]
    #  [1, 3, 4, 4, 5, 5, 7, 9, 13]
    #r  1  2  4  4  6  6  7  8   9
    t = Tracker()
    for x in seq:
        print('inserting ', x)
        t.insert(x)
        t.inorder()
    
    for x in seq:
        print(x, t.get_rank(x))
