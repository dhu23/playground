# implement an algorithm to delete a node in the middle
# (i.e. any node but the first and the last node, not necessarily the
# exact middle) of a singly linked list, given only access to that node

# example:
# input: the node c from the list: a->b->c->d->e->f
# result: nothing is returned, but new linked list looks like a->b->d->e->f

from linked_list import *

def build_inputs():
    head = convert_list(range(10))
    return head, head.next.next.next # return head and node 3

# delete handle node
def delete_middle_node(handle):
    assert handle is not None # requirement from problem 
    next = handle.next # this exists
    rest = next.next
    handle.val = next.val
    handle.next = rest 

if __name__ == "__main__":
    ll_head, node3 = build_inputs()
    print('-----------------')
    print_list(ll_head)

    delete_middle_node(node3)
    print('-----------------')
    print_list(ll_head)
    
