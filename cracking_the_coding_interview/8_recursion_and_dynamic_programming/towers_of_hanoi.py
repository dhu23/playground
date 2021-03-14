# in the classic problem of the towers of hanoi, you have 3 towers and N disks
# of different sizes which can slide onto any tower. The puzzle starts with
# disks sorted in ascending order of size from top to bottom.
# i.e. each disk sits on top of an even larger one. You have the following
# constraints:
# 1) only one disk can be moved at a time
# 2) a disk is slid off the top of one tower onto another tower
# 3) a disk cannot be placed on top of a smaller disk.

# write a program to move the disks from the first tower to the last
# using stacks

towers = {'t1': [5, 4, 3, 2, 1], 't2': [], 't3': []}

def print_stacks(towers):
    print('--------')
    print(towers['t1'])
    print(towers['t2'])
    print(towers['t3'])


# move the top n places of t1 to t2
# then place the new top of t1 on t3, if flag is true
def move_top(t1, t2, t3, n, flag=True):
    assert n > 0
    if n == 1:
        t2.append(t1.pop())
        print_stacks(towers)
        if flag:
            t3.append(t1.pop())
            print_stacks(towers)
    else:
        move_top(t1, t3, t2, n-1, True)
        move_top(t3, t2, t1, n-1, False)
        if flag:
            t3.append(t1.pop())
            print_stacks(towers)
        
def hanoi(t1, t2, t3):
    assert all(t1[i-1] >= t1[i] for i in range(1, len(t1)))
    assert not t2 and not t3

    move_top(t1, t3, t2, len(t1), False)

if __name__ == "__main__":
    hanoi(towers['t1'], towers['t2'], towers['t3'])
