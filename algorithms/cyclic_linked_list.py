

class Node(object):
    def __init__(self, value):
        self.value = value
        self.next = None


class TwoPointNode(Node):
    def __init__(self, value):
        Node.__init__(self, value)
        self.random = None

    def output(self):
        print 'id=%s, val=%s, next=%s, random=%s' % (
                id(self), self.value,
                id(self.next) if self.next is not None else None, 
                id(self.random) if self.random is not None else None)


def build_linked_list():
    n1 = TwoPointNode(1)
    n2 = TwoPointNode(2)
    n3 = TwoPointNode(3)
    n4 = TwoPointNode(4)

    n1.next = n2
    n2.next = n3
    n3.next = n4
    # n4.next is still None

    n1.random = n3
    n2.random = n2
    n3.random = n1
    n4.random = n2

    return n1

def print_linked_list(ll):
    first = ll
    while first is not None:
        first.output()
        first = first.next


def clone_linked_list(ll):
    first = ll
    head = ll

    # inserted copy after original nodes
    while first is not None:
        first_clone = TwoPointNode(first.value)
        first_clone.next = first.next
        first.next = first_clone

        first = first_clone.next
    
    print_linked_list(head)
    
    first = head
    # arrange the random pointers for clones
    while first is not None:
        # first_clone's next is first's random's clone
        first.next.random = first.random.next
        first = first.next.next
    
    print_linked_list(head)

    clone_head = head.next
    first_clone = head.next
    first = head

    while first is not None:
        first_clone = first.next
        first.next = first_clone.next # first.next.next
        if first.next is None:
            first_clone.next = None
        else:
            first_clone.next = first.next.next
        first = first.next

    print ''.join(['-']*20)
    print_linked_list(head)
    print_linked_list(clone_head)

if __name__ == "__main__":
    ll = build_linked_list()
    print_linked_list(ll)

    clone_linked_list(ll)
