# imagine a web server for a simplified search engine. This system has 100
# machines to respond to search queries, which may then call out using
# processSearch(string query) to another cluster of machines to actually get
# the result. The machine which responds to a given query is chosen at random.
# so you cannot guarantee that the same machine will always respond to the
# same request. The method processSearch is very expensive. Design a caching
# mechanism for the most recent queries. Be sure to explain how you woudl
# update the cache when data changes.


# 100 machines as user machines and processSearch(query) are sent out.
# Create a separate cache layer between the user cluster and core cluster,
# so that the cache cluster can well distribute the most recent, say 3 million
# search keys. And we can further partition the distribution so that certain
# cache machines contain key1 to key 100 and the next group contain key101 to
# key200. The cache machines make the actual processSearch(string query) to
# the core cluster on a cache miss. The machines should be configured in a way
# that cache machines and core machines are in a healthy busy status during
# normal use. Cache machines need to resend their cached keys to the core once
# every a few min(hour depending on use case) to get updated data. 

# the cache itself, will have to include the following feature:
# 1, fast look up (through hash table)
# 2, limited size to bump out old enough cache content (a queue)
# 3, query content gets updated and move to the most recent 

# one way to design this is to have a map(hash table more accurately) from
# query to location(in python reference node) in the queue(implemented by
# linked list)

def hex_id(x):
    return hex(id(x))

class Node(object):
    def __init__(self, val):
        self.val = val
        self.next = None
        self.prev = None
    def __repr__(self):
        return 'value={0},next={1},prev={2}'.format(
            self.val, hex_id(self.next), hex_id(self.prev))

class DoublyLinkedList(object):
    def __init__(self):
        self.head = Node(None) # use sentinal nodes for easier manipulation
        self.tail = Node(None)

        self.head.next = self.tail
        self.tail.prev = self.head

        self.size = 0

    def __len__(self):
        return self.size
        
    def __repr__(self):
        n = self.head.next
        builder = ["----- linked list content ----- "]
        while n is not self.tail:
            builder.append(repr(n))
            n = n.next
        return '\n'.join(builder)

    def insert(self, n, node): # insert after node
        #print(node)
        n.next = node.next
        #print(node.next)
        node.next.prev = n

        n.prev = node
        node.next = n

        # how can we guarantee that this node is in the list?!
        self.size += 1

    def remove(self, node): # remove node
        node.prev.next = node.next
        node.next.prev = node.prev
        node.prev, node.next = None, None

        # how can we guarante that this node is in the list?!
        self.size -= 1
        
        
class Cache(object):
    def __init__(self, limit):
        self.content = {}
        self.q = DoublyLinkedList()
        self.limit = max(limit, 2)

    def lookup(self, k):
        return self.content.get(k, None)

    def add(self, k, v):
        if k in self.content:
            q_node = self.content[k]
            q_node.val = (k, v)
            # move it to the top
            self.q.remove(q_node)
            print(q_node)
            self.q.insert(q_node, self.q.head)
            
        else:
            q_node = Node((k, v))
            self.q.insert(q_node, self.q.head)
            self.content[k] = q_node
            
            # bump of the stalest one if off limit
            if len(self.q) > self.limit:
                node_to_remove = self.q.tail.prev
                del self.content[node_to_remove.val[0]]
                self.q.remove(node_to_remove)

    def __repr__(self):
        builder = ["--- showing cache ---"]
        builder.append(repr(self.q))
        builder.append("hash table content:")
        for k, v in self.content.items():
            builder.append('key={0},val={1}'.format(k, v))
        return '\n'.join(builder)

def test_dll():
    dll = DoublyLinkedList()
    dll.insert(Node(1), dll.head)
    dll.insert(Node(2), dll.head)
    dll.insert(Node(3), dll.head)
    print(dll)

def test_cache():
    print('----- test cache ------')
    cache = Cache(4)
    cache.add(6, "result for 6")
    cache.add(5, "result for 5")
    cache.add(4, "result for 4")
    cache.add(6, "result for new 6")
    cache.add(3, "result for 3")
    cache.add(2, "result for 2")
    cache.add(1, "result for 1")
    print(cache)

if __name__ == "__main__":
    test_dll()
    test_cache()
