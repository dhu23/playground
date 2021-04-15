# given a circular linked list, implement an algorithm that returns the node
# at the beginning of the loop

# def
# circular linked list is a (corrupted) linked list which a node's next pointer
# points to an earlier node, so as to make a loop in the linked list

# example input A->B->C->D->E->C(same C as ealier)
# output: C

from linked_list import *

def detect_loop(ll):
    faster, slower = ll, ll

    if faster.next is None or faster.next.next is None:
        return False, False
    
    faster, slower = faster.next.next, slower.next
    
    # faster should stop at a position where +2 move will be out of the end
    while faster.next is not None and faster.next.next is not None:
        if id(faster) == id(slower):
            # there is a loop, head node in loop
            # print(faster)
            return True, id(faster) == id(ll) 
        faster = faster.next.next
        slower = slower.next
    return False, False

# assuming the linked list already as a loop
def detect_loop_begin_naive(ll):
    found = set()
    n = ll
    while n is not None: # it probably won't happen
        if id(n) not in found:
            found.add(id(n))
            n = n.next
        else:
            return n
        
def detect_loop_begin(ll):
    is_loop, head_in_loop = detect_loop(ll)
    assert is_loop and not head_in_loop

    n = ll.next
    while True:
        _, n_in_loop = detect_loop(n)
        if n_in_loop:
            return n
        else:
            n = n.next # can tune it to move faster

# return detected loop begin node
def detect_loop_begin2(ll):
    faster, slower = ll, ll
    if faster.next is None or faster.next.next is None:
        return None
    
    faster, slower = faster.next.next, slower.next
    # faster should stop at a position where +2 move will be out of the end
    
    while faster.next is not None and faster.next.next is not None:
        if id(faster) == id(slower):
            # there is a loop, head node in loop
            break
        faster = faster.next.next
        slower = slower.next
    else:
        # if while loop stops because of false condition, there is no loop
        return None

    from_head, from_loop = ll, faster
    while True:
        if id(from_head) == id(from_loop):
            return from_head
        from_head, from_loop = from_head.next, from_loop.next

# can make a nice utility function that returns
# the last node if there is no loop
# the starting of the loop if there is
        
if __name__ == "__main__":
    a, b, c, d, e = Node('A'), Node('B'), Node('C'), Node('D'), Node('E')
    a.next = b
    b.next = c
    c.next = d
    d.next = e
    e.next = c

    print(detect_loop(c))
    print(detect_loop_begin_naive(a))
    print(detect_loop_begin(a))
    print(detect_loop_begin2(a))
