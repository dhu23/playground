

import random
import matplotlib.pyplot as plt

class Node(object):
    def __init__(self, value):
        self.value = value
        self.next = None

    def output(self):
        print "id=%s, val=%s, next=%s" % (
                id(self), self.value, id(self.next) if self.next else None)


def print_linked_list(head):
    first = head
    while first is not None:
        first.output()
        first = first.next


def append_from(new_list_head, new_list_end, old_list_head):
    '''remove the first item from the old_list and append to the new list'''
    if old_list_head is None:
        return new_list_head, new_list_end, old_list_head

    if new_list_end is not None:
        new_list_end.next = old_list_head
    else:
        new_list_head = old_list_head

    new_list_end = old_list_head
    old_list_head = old_list_head.next
    new_list_end.next = None
    return new_list_head, new_list_end, old_list_head


def random_merge(ll1, size1, ll2, size2):
    first1, first2 = ll1, ll2

    head = None
    end = None

    added1, added2 = 0, 0

    while added1 < size1 or added2 < size2:
        p = random.random()
        if (p < 0.5 and added1 < size1) or (added2 == size2):
            head, end, first1 = append_from(head, end, first1)
            added1 += 1
        elif (p >= 0.5 and added2 < size2) or (added1 == size1):
            head, end, first2 = append_from(head, end, first2)
            added2 += 1
        else:
            raise Exception("should not happen")

    return head
    
def merge_shuffle(ll1, size1, ll2, size2):
    '''mimic knuth shuffle algorithm'''
    pass

def knuth_shuffle(data_list):
    
    for i in xrange(len(data_list)):
        t = random.randint(0, i)
        data_list[t], data_list[i] = data_list[i], data_list[t]
        
    
def create_linked_list(val_list):
    # build linked list
    node_list = [Node(val_list[0])]
    head = node_list[0]
    for i in val_list[1:]:
        n = Node(i)
        node_list[-1].next = n
        node_list.append(n)

    # print_linked_list(head)
    return head, node_list


def repeat_knuth_shuffle(repeat):
    _, data1_as_vector = create_linked_list(range(0, 10))
    _, data2_as_vector = create_linked_list(range(100, 110))

    total_as_vector = []
    total_as_vector.extend(data1_as_vector)
    total_as_vector.extend(data2_as_vector)
         
    sample_data = []    
    
    for i in xrange(repeat):
        v = list(total_as_vector)
        knuth_shuffle(v)

        # find location of 100
        sample_data.append(
                next(j for j, d in enumerate(v) if d.value == 100))  
    return sample_data

def repeat_random_merge(repeat):
    sample_data = []    
    
    for i in xrange(repeat):
        data1, _ = create_linked_list(range(0, 10))
        data2, _ = create_linked_list(range(100, 110))
        
        head = random_merge(data1, 10, data2, 10)
        first, c = head, 0
        while first is not None:
            if first.value == 100:
                sample_data.append(c)
                break
            c += 1
            first = first.next
    return sample_data


if __name__ == "__main__":
    
    
    # map(lambda x: x.output(), data1_as_vector)
    
    locations_of_100 = repeat_knuth_shuffle(10000)
    plt.hist(locations_of_100)
    
    
    # print_linked_list(random_merge(data1, 10, data2, 10))
    locations_of_merge_100 = repeat_random_merge(10000)
    plt.hist(locations_of_merge_100)
