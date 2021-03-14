# implement a function to check if a linked list is a palindrome

from linked_list import *

# recursive stack trace for 1->2->3->2->1
# level 1           1->2->3->2->1    1 
# level 2              2->3->2->1    2
# level 3                 3->2->1    3
# level 4                    2->1    2
# level 5                       1    1

def check_palindrome_list(ll):
    n = ll
    stack = []
    while n is not None:
        stack.append(n.val)
        n = n.next
    lo, hi = 0, len(stack)-1
    while lo < hi:
        if stack[lo] != stack[hi]:
            return False
        lo, hi = lo+1, hi-1
    return True

# for fun, with recursion
def check_palindrome_list_recursion(ll, forward_ll):
    if ll.next is None: # at the last node
        if ll.val == forward_ll.val:
            return True, forward_ll.next
        else:
            return False, forward_ll.next

    flag, forward_ll = check_palindrome_list_recursion(ll.next, forward_ll)
    if flag and forward_ll.val == ll.val:
        return True, forward_ll.next
    else:
        return False, forward_ll.next

def check_palindrome_list2(ll):
    return check_palindrome_list_recursion(ll, ll)[0]

# use two runners, one at +1 speed and one at +2 speed
def check_palindrome_runner(ll):
    if ll.next is None:
        return True
    elif ll.next.next is None:
        return ll.val == ll.next.val

    # now ll.next.next is valid
    
    stack = []
    faster, slower = ll, ll

    # detecting the last
    while faster.next is not None and faster.next.next is not None:
        stack.append(slower.val)
        faster = faster.next.next
        slower = slower.next

    if faster.next is None:
        # linked list has odd nodes
        pass
    else:
        # even nodes
        stack.append(slower.val)
    
    p2 = slower.next # regardless even or odd nodes
    
    while p2 is not None:
        if p2.val != stack[-1]:
            return False
        else:
            p2 = p2.next
            stack.pop()
    return True
        

if __name__ == "__main__":
    print('--------------------')
    print(check_palindrome_list(convert_list([1, 2, 3, 2, 1])))
    print(check_palindrome_list(convert_list([1, 2, 3, 4, 4, 3, 2, 1])))
    print(check_palindrome_list(convert_list(range(5))))

    print('--------------------')
    print(check_palindrome_list2(convert_list([1, 2, 3, 2, 1])))
    print(check_palindrome_list2(convert_list([1, 2, 3, 4, 4, 3, 2, 1])))
    print(check_palindrome_list2(convert_list(range(5))))

    print('--------------------')
    print(check_palindrome_runner(convert_list([1, 2, 3, 2, 1])))
    print(check_palindrome_runner(convert_list([1, 2, 3, 4, 4, 3, 2, 1])))
    print(check_palindrome_runner(convert_list(range(5))))

    
    
