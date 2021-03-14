import random


class UnionFind(object):
    '''tracks number from 0 to N-1'''
    def __init__(self, size):
        '''describe a function from i to self.data[i]'''
        self.data = range(size)
        self.group_size = [1]*size
        self.group_max = list(self.data)

    def _verify(self, n):
        if n >= len(self.data):
            raise Exception("number %d not in range" % n)

    def _get_root(self, n):
        self._verify(n)

        while self.data[n] != n: # root has self.data[n] == n
            self.data[n] = self.data[self.data[n]]
            # group max and group size could be updated here            
            
            n = self.data[n] # move up 
    
        return n

    def union(self, n1, n2):
        self._verify(n1)
        self._verify(n2)
        print 'union(%d, %d)' % (n1, n2)

        if n1 == n2:
            return

        n1_root = self._get_root(n1)
        n2_root = self._get_root(n2)

        if n1_root == n2_root:
            return

        new_max = max(self.group_max[n1_root], self.group_max[n2_root])

        if self.group_size[n1_root] >= self.group_size[n2_root]:
            self.data[n2_root] = n1_root
            self.group_size[n1_root] += self.group_size[n2_root]
            self.group_max[n1_root] = new_max
        else:
            self.data[n1_root] = n2_root
            self.group_size[n2_root] += self.group_size[n1_root]
            self.group_max[n2_root] = new_max


    def find(self, n1, n2):
        self._verify(n1)
        self._verify(n2)
    
        print 'find(%d, %d)' % (n1, n2)
        if n1 == n2:
            return True
        
        return self._get_root(n1) == self._get_root(n2)

    def find_group_max(self, n):
        self._verify(n)

        return self.group_max[self._get_root(n)]

def test_union_find_algo(method):
    num_max = 2000
    uf = method(num_max+1)

    for i in xrange(1500):
        uf.union(random.randint(0, num_max), random.randint(0, num_max))
    
    for i in xrange(2500):
        print uf.find(random.randint(0, num_max), random.randint(0, num_max))

def test_group_max(method):
    num_max = 20
    uf = method(num_max+1)

    uf.union(1, 2)
    uf.union(2, 7)
    uf.union(13, 9)
    uf.union(8, 19)
    uf.union(3, 7)

    print uf.find_group_max(9) # group = 9, 13  max = 13
    print uf.find_group_max(1) # group = 1, 2, 3, 7 max = 7


def test_successor_delete(method):
    num_max = 15
    uf = method(num_max+1)

    uf.union(3, 3+1) # remove 3
    uf.union(4, 4+1)

    uf.union(10, 10+1)
    uf.union(12, 12+1)
    uf.union(11, 11+1)
  
    # now array is
    # 0, 1, 2, [3], [4], 5, 6, 7, 8, 9, [10], [11], [12], 13, 14, 15
    
    print uf.find_group_max(2+1) # find the successor for 2
    print uf.find_group_max(8+1) # find for 8 => 9
    print uf.find_group_max(9+1) # find for 9 => 13

    print uf.data
    print uf.group_max


if __name__ == "__main__":
    # test_union_find_algo(UnionFind)
    # test_group_max(UnionFind)
    test_successor_delete(UnionFind)
