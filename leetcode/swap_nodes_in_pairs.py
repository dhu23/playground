# refer to more generic solution in reverse_nodes_in_k_group


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
    print('----------   done   -------------')
        
def swap_in_pairs(linked_list):

    ret, last = None, None
    first = linked_list

    # ret ------ last --> first --> second --> rest -----
    
    while first:
        second = first._next

        if second is None:
            if ret is None:
                ret = first
            break
        
        # second is not None, swap with first
        rest = second._next
        second._next = first
        first._next = rest

        if ret is None:
            ret = second
            
        else:
            # keep ret the same
            last._next = second    

        last = first
        first = rest
                
    return ret


if __name__ == "__main__":
    linked_list = Node(1)
    linked_list._next = Node(2)
    linked_list._next._next = Node(3)
    linked_list._next._next._next = Node(4)

    print_linked_list(linked_list)
    head = swap_in_pairs(linked_list)
    print_linked_list(head)
    
