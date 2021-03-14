# given two singly linked lists, determine if the two lists intersect. Return
# the intersecting node. Note that the intersection is defined based on
# reference, not value. That is, if the kth node of the first linked ist is
# the exact same node (by reference) as the jth node of the second linked list,
# then they are intersecting

from linked_list import *
from return_kth_to_last import kth_to_last_runner

# intersecting singly linked lists end up with the same node

def check_intersect(l1, l2):
    return id(get_last_node(l1)) == id(get_last_node(l2))

def intersection_node(l1, l2):
    v1, v2 = [], []
    n1, n2 = l1, l2
    while n1 is not None:
        v1.append(id(n1))
        n1 = n1.next
    while n2 is not None:
        v2.append(id(n2))
        n2 = n2.next

    i = -1
    if v1[i] != v2[i]:
        return None # no intersection
    else:
        while (-i) <= len(v1) and (-1) <= len(v2) and v1[i] == v2[i]:
            i -= 1
        i += 1
        i = (-i)
        # return ith from the back
        return kth_to_last_runner(l1, i)

def intersection_node2(l1, l2):
    if not check_intersect(l1, l2):
        return None

    l1_len, l2_len = list_len(l1), list_len(l2)
    if l1_len > l2_len:
        longer, shorter = l1, l2
    else:
        longer, shorter = l2, l1

    visited = set() # this hash table might take up more memory than vectors
    while shorter is not None:
        visited.add(id(shorter))
        if id(longer) in visited:
            return longer
        longer, shorter = longer.next, shorter.next
        

if __name__ == "__main__":
    node1 = Node(1)
    node2 = Node(2)
    node3 = Node(3)
    node4 = Node(4)
    node5 = Node(5)
    node6 = Node(6)

    # 2 -> 3 -> 4 -> 5
    # 1 -> 6 ->            connecting to node 4
    node2.next = node3
    node3.next = node4
    node4.next = node5
    node1.next = node6
    node6.next = node4

    l1, l2 = node2, node1

    print(check_intersect(l1, l2))
    print(intersection_node(l1, l2))
    print(intersection_node2(l1, l2))
