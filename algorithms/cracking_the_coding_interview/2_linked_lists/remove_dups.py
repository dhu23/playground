# write code to remove duplicates from an unsorted linked list
# follow up:
# how would you solve this problem if a temporary buffer is not allowed
from linked_list import *

def build_list():
    head = Node(5)
    head.next = Node(6)
    head.next.next = Node(5)
    head.next.next.next = Node(4)
    head.next.next.next.next = Node(6)
    return head

def remove_dup1(head):
    n = head # n is the tail of the new linked list without duplicates
    unique_vals = {n.val}
    while n.next is not None:
        if n.next.val not in unique_vals:
            unique_vals.add(n.next.val)
            n = n.next # move n
        else:
            n.next = n.next.next
    return head

# without extra temporary buffer
def remove_dup2(head):
    n = head
    while n is not None:
        checker = n
        while checker.next is not None:
            if checker.next.val == n.val:
                # remove checker.next
                checker.next = checker.next.next
            else:
                checker = checker.next
        n = n.next
    return head


if __name__ == "__main__":
    print_list(build_list())
    print(find_node(build_list(), 8))
    print('------------------------')
    print_list(remove_dup1(build_list()))
    print('------------------------')
    print_list(remove_dup2(build_list()))
