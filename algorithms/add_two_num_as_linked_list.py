# leetcode #2
class Node(object):
    def __init__(self, val):
        self.val = val
        self.next = None
    def output(self):
        return 'id=%s,val=%d' % (id(self), self.val)

def print_list(first):
    curr = first
    while curr is not None:
        print curr.output()
        curr = curr.next

def linked_list1():
    l = Node(1)
    l.next = Node(4)
    l.next.next = Node(3)
    l.next.next.next = None
    return l

def linked_list2():
    l = Node(8)
    l.next = Node(6)
    l.next.next = Node(4)
    l.next.next.next = None
    return l

def reverse_list_print_util(first):
    if first is None:
        return
    reverse_list_print_util(first.next)
    print first.output()

def reverse_list(first):
    if first.next is None:
        return first

    head = reverse_list(first.next)
    first.next.next = first
    first.next = None
    return head

# receive number backwards, return backwards
# meaning l1 = 143(as 1->4->3) and l2 = 864(as 8->6->4)
# the sum is 1007(as 1->0->0->7)
def add1(l1, l2): # l1 and l2 need to have the same number of digits
    if l1.next is None and l2.next is None:
        s = l1.val+l2.val
        next_curr = None
    else:
        next_curr, next_carry = add1(l1.next, l2.next)
        s = l1.val+l2.val+(next_carry.val if next_carry is not None else 0)
    
    carry = 1 if s > 9 else 0
    curr = s-carry*10
    curr_node = Node(curr)
    curr_node.next = next_curr
    if carry == 1:
        carry_node = Node(carry)
        carry_node.next = curr_node
        return curr_node, carry_node
    else:
        return curr_node, None
    
# receive number forwards, return forwards
# meaning l1 = 341(as 1->4->3) and l2 = 468(as 8->6->4)
# the sum is 809(9->0->8)
def add2(l1, l2, carry): # l1 and l2 size can differ
    
    curr1, curr2 = l1, l2
    
    s = curr1.val if curr1 is not None else 0
    s += curr2.val if curr2 is not None else 0
    s += carry

    if s == 0:
        return None

    carry = 1 if s > 9 else 0
    curr = s-carry*10

    curr_node = Node(curr)

    next_node = add2(
            curr1.next if curr1 is not None else None, 
            curr2.next if curr2 is not None else None, 
            carry)

    if next_node is not None:
        curr_node.next = next_node

    return curr_node

# same as add2, but with a non-recursive implementation
def add3(l1, l2):
    
    curr1, curr2 = l1, l2
    head, tail, carry = None, None, 0
    while curr1 is not None or curr2 is not None:
        s = curr1.val if curr1 is not None else 0
        s += curr2.val if curr2 is not None else 0
        s += carry

        carry = 1 if s > 9 else 0
        curr = s-carry*10

        curr_node = Node(curr)
        if head is None: # beginning of the result linked list
            head = curr_node
            tail = curr_node
        else: # head and tail is already created
            tail.next = curr_node
            tail = curr_node
            tail.next = None
        
        curr1 = curr1.next if curr1 is not None else None
        curr2 = curr2.next if curr2 is not None else None
    
    if head is None or curr is None:
        raise Exception("Something is wrong")

    # tail.next = None

    return head
    
if __name__ == "__main__":
    print print_list(linked_list1())
    print print_list(linked_list2())
    reverse_list_print_util(linked_list1())
    
    head = reverse_list(linked_list1())

    print_list(head)

    print '-------------------add1----------------------'
    curr, carry = add1(linked_list1(), linked_list2())
    if carry is None:
        print_list(curr)
    else:
        print_list(carry)

    print '---------------------add2------------------------'
    head = add2(linked_list1(), linked_list2(), 0)
    print_list(head)

    print '--------------------add3-----------------------'
    head = add3(linked_list1(), linked_list2())
    print_list(head)
