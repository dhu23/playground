# write code to partition a list around a value x
# such that all nodes less than x come before all nodes gerater than or equal
# to x. If x is contained in the list, the values of x only need to be after
# elements less than x. (see below).

# the partition element x can appear anywhere in the right partition, it does
# not need to appear between the left and right partitions

# input: 3->5->8->5->10->2->1  (partition=5)
# output: 3->1->2->10->5->5->8

from linked_list import *

def partition_list(ll_head, p):
    left, left_tail, right = None, None, None
    n = ll_head
    while n is not None:
        temp = n.next
        if n.val < p:
            left = append_list(left, n)
            if left_tail is None:
                left_tail = left
        else:
            right = append_list(right, n)
        n = temp
        
    # append right after left_tail
    if left_tail is None:
        assert left is None
        return right
    else:
        left_tail.next = right
        return left

def partition_list2(ll_head, p):
    node = ll_head
    head, tail = node, node # the head and tail of the new list
    while node is not None:
        next = node.next
        if node.val < p:
            node.next = head
            head = node
        else:
            tail.next = node
            tail = node
        node = next
    tail.next = None
    return head
            
if __name__ == "__main__":
    print_list(partition_list(convert_list([3, 5, 8, 5, 10, 2, 1]), 5))
    print('=======================================')
    print_list(partition_list2(convert_list([3, 5, 8, 5, 10, 2, 1]), 5))
    
