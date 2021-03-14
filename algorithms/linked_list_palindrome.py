class Node(object):
    def __init__(self, val):
        self.val = val
        self.next = None

    def __repr__(self):
        return 'val=%s,id=%s,next=%s' % (
            self.val, hex(id(self)), hex(id(self.next)))


def create_linked_list(vals):
    head = Node(vals[0])
    prev = head
    for val in vals[1:]:
        prev.next = Node(val)
        prev = prev.next

    return head

def print_linked_list(head):
    curr = head
    while curr is not None:
        print curr
        curr = curr.next
    

# verify palindrome
#                        forward                     backward
# stack 0     head       (next^0)                    (next^n-1)
# stack 1     head.next  (next^1)                    (next^n-2)
# stack 2                (next^2)
# stack 3                (next^3)
# ...........................................................
# stack i                (next^i)                    (next^n-i-1)
# ...........................................................
# stack n-2              (next^n-2)    head.next     (next^1)
# stack n-1              (next^n-1)    head          (next^0)

def check_palindrome(forward, head):
    if forward.next is None:
        return forward.val == head.val, head.next
    
    success, backward_node = check_palindrome(forward.next, head)
    # print forward, backward_node
    if not success:
        return success, backward_node.next
    else:
        return forward.val == backward_node.val, backward_node.next
    
    


if __name__ == "__main__":
    print_linked_list(create_linked_list(range(10)))
    
    palindrome = create_linked_list([1, 2, 3, 4, 5, 4, 3, 2, 1])
    print check_palindrome(palindrome, palindrome)


    non_palindrome = create_linked_list([1, 2, 3, 4, 5, 5, 3, 2, 1])
    print check_palindrome(non_palindrome, non_palindrome)
