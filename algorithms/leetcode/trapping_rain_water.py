# given n non negative intergers representing an elevation map where the width of each bar is 1, compute how much water it is able to trap after raining

# a type of folding problem with stack

def fill_traps(bars):
    stack = [] # this stack tracks a sequence of decreasing bars

    collected = 0
    
    for i, height in enumerate(bars):

        if not stack:
            stack.append([i, height])
            continue

        if len(stack) == 1:
            if stack[-1][1] <= height:
                stack.pop()    
            stack.append([i, height])
            continue

        # stack size is at least 2 now
        if stack[-1][1] < height:
            tentative = 0
            # this is going up, collaspe the stack to secure water
            # until there are only 2 elements in the stack or the the last
            # one in the stack is higher than the current one
            while len(stack) >= 2 and stack[-1][1] <= height:
                tentative += (height-stack[-1][1])*(stack[-1][0]-stack[-2][0])
                stack.pop()

            # there should be at least one item in stack
            if stack[-1][1] < height:
                tentative -= (i-stack[-1][0]-1)*(height-stack[-1][1])
                stack.pop()
            collected += tentative
            
        stack.append([i, height])
        
    return collected


if __name__ == "__main__":
    print(fill_traps([1, 2, 3, 4])) # 0
    print(fill_traps([4, 3, 2, 1])) # 0
    print(fill_traps([0, 1, 0, 2, 0, 3, 0, 4, 0, 5])) # 10
    print(fill_traps([3, 2, 1, 2, 3])) # 4
    print(fill_traps([1, 2, 3, 4, 5, 4, 3, 2, 3, 4, 5, 6, 7, 6, 5, 4, 5])) # 10
    print(fill_traps([0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1])) # 6
