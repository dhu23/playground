# you have two numbers represented by a linked list, where each node
# contains a single digit. The digits are stored in reverse order, such
# that the 1's digit is at the head of the list. Write a function that
# adds the two numbers and returns the sum as a linked list


# example
# input (7->1->6) + (5->9->2), that is 617+295
# output 2->1->9. That is 912

# follow up
# suppose the digits are stored in forward order, repeat the problem
# input (6->1->7) + (2->9->5), that is 617+295
# output 9->1->2. That is 912

from linked_list import *

def add_list_backwards(l1, l2):
    n1, n2, carry = l1, l2, 0
    ret_head, ret = None, None
    while (n1 is not None or n2 is not None) or carry != 0:
        v1 = 0 if n1 is None else n1.val
        v2 = 0 if n2 is None else n2.val
        carry, v = divmod(v1 + v2 + carry, 10)
        print(v1, v2, v, carry)
        if ret_head is None:
            ret_head = Node(v)
            ret = ret_head
        else:
            ret.next = Node(v)
            ret = ret.next

        if n1 is not None:
            n1 = n1.next
        if n2 is not None:
            n2 = n2.next
        
    return ret_head


def add_list_forwards(l1, l2):
    return reverse_list(
        add_list_backwards(reverse_list(l1),
                           reverse_list(l2)))

def int_to_list(n, backwards=True):
    ret = [int(c) for c in str(n)]
    if backwards:
        return list(reversed(ret))
    else:
        return ret

def add(n1, n2, backwards=True):
    add_list = add_list_backwards if backwards else add_list_forwards
    print_list(
        add_list(convert_list(int_to_list(n1, backwards)),
                 convert_list(int_to_list(n2, backwards))))

if __name__ == "__main__":
    print('----------------------------------------------')
    add(617, 595, backwards=True)
    print('----------------------------------------------')
    add(617, 295, backwards=False)
