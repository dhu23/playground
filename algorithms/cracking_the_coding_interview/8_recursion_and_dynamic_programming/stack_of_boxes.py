# you have a stack of n boxes, with widths w_i, heights h_i, and depths d_i.
# the boxes cannot be rotated and can only be stacked on top of one another if
# each box in the stack is strictly larger than the box above it in width,
# height and depth. Implement a method to compute the height of the tallest
# possible stack. The height of a stack is the sum of the heights of each box.

# backtracking problem

import operator
width = operator.itemgetter(0)
depth = operator.itemgetter(1)
height = operator.itemgetter(2)

def total_height(bs):
    return sum(map(height, bs))

def solve(available, used, stacks):
    if not available:
        stacks.append(used)
        return
 
    children = [(i, each) for i, each in enumerate(available)
                if (not used or
                    (width(each) < width(used[-1]) and
                     depth(each) < depth(used[-1])))]

    if not children:
        stacks.append(used)
        return

    for (i, c) in children:
        new_available = list(available)
        use = new_available.pop(i)
        solve(new_available, used+[use], stacks)
        

def stack_boxes(bs):
    available = bs
    used = []
    stacks = []
    solve(available, used, stacks)
    print(stacks)

    print(list(map(total_height, stacks)))


if __name__ == "__main__":
    
    boxes = [
        (3, 5, 2), # w, d, h
        (5, 3, 2),
        (1, 2, 3),
        (6, 1, 2),
        (5, 6, 1)
        ]

    stack_boxes(boxes)
