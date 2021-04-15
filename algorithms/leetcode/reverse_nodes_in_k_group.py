class Node(object):
    def __init__(self, val):
        self._val = val
        self._next = None

    def __repr__(self):
        return '{}({}->{})'.format(self._val, id(self), id(self._next))

def print_linked_list(head):
    print('---------------------------------')
    current = head
    while current:
        print(current)
        current = current._next
    print('None({})'.format(id(None)))
    print('----------   done   -------------')

# with recursion
def reverse_linked_list_recursive(head):
    if head is None:
        raise Exception('cannot reverse empty linked list')
    
    rest = head._next
    if rest is None:
        return head, head

    # if head is not the last one
    h, t = reverse_linked_list_recursive(rest)
    t._next = head
    head._next = None
    return h, head
    

# no recurisve. count is the total number of nodes being reversed
def reverse_linked_list(head, count=None):
    assert count is None or count >= 2
    
    if head is None:
        raise Exception('cannot reverse empty linked list')

    num_done = 0
    # node 1 -> node 2 -> ... -> node i-1 -> node i -> node i+1 -> ...
    # during reversing
    # (rev_tail)                 (rev_head)  (fwd_head) (rest)
    # node 1 <- node 2 <- ... <- node i-1    node i -> node i+1 -> ...
    # node 1 <- node 2 <- ... <- node i-1 <- node i    node i+1 -> ...
    # (rev_tail)                             (rev_head) (fwd_head) ...

    rev_tail, rev_head = None, None
    # rev_tail is only updated once when it is no longer None
    fwd_head = head
    
    while fwd_head is not None and (count is None or num_done < count):
        rest = fwd_head._next

        fwd_head._next = rev_head
        rev_head = fwd_head
        if rev_tail is None:
            rev_tail = fwd_head

        fwd_head = rest    
        num_done += 1
        
    return fwd_head, (rev_head, rev_tail)

def build_linked_list(n):
    head = Node(1)
    tail = head

    val = 2
    while val <= n:
        tail._next = Node(val)
        tail = tail._next
        tail._next = None
        val += 1

    return head
    
    
def test_recursion_solution():
    ll_head = build_linked_list(20)
    h, t = reverse_linked_list_recursive(ll_head)
    print_linked_list(h)

def test_non_recursion_solution():
    ll_head = build_linked_list(10)
    h, (rhead, rtail) = reverse_linked_list(ll_head, 11)
    print('forword')
    print_linked_list(h)
    print('reversed')
    print_linked_list(rhead)


def reverse_nodes_in_k_group(head, k):
    result_head = None
    ll_head = head
    while ll_head is not None:
        ll_head, (rhead, rtail) = reverse_linked_list(ll_head, k)
        rtail._next = result_head
        result_head = rhead

    return result_head

def test_reverse_k_group():
    ll_head = build_linked_list(10)
    print_linked_list(reverse_nodes_in_k_group(ll_head, 2))
            
if __name__ == "__main__":
    test_non_recursion_solution()
    test_reverse_k_group()
