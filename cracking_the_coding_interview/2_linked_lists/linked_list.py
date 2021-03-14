import functools

class Node(object):
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

def find_node(head, val, terminal=None):
    n = head
    while n is not terminal and n.val != val:
        n = n.next
    return n

def append_list(ll_head, n):
    head = n
    head.next = ll_head
    return head

def convert_list(l):
    return functools.reduce(append_list, map(Node, reversed(l)), None)

def list_len(ll, terminal=None):
    count = 0
    n = ll
    while n is not terminal:
        count += 1
        n = n.next
    return count

def find_kth(ll, k):
    count = 1
    n = ll
    while n is not None and count < k:
        count += 1
        n = n.next
    return n
        
def reverse_list(ll):
    # empty or single node cases handled by the following generic code
    #if ll is None or ll.next is None: return ll

    ret = None
    n = ll
    while n is not None:
        temp = n.next
        ret = append_list(ret, n)
        n = temp
    return ret

def get_last_node(ll):
    n = ll
    while n.next is not None:
        n = n.next
    return n

if __name__ == "__main__":
    # test convert list
    print_list(convert_list(range(5, 0, -1))) # build 5->4->3->2->1
    print(list_len(convert_list(range(10)))) # len = 10
    print(find_kth(convert_list(range(10)), 5)) # node 4
    print('-----------------------------')
    print_list(reverse_list(convert_list(range(10))))
