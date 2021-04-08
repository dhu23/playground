def largest_rect(hs): 
    stack, done_rect = [], [] # a rectangle = height, start, end
    
    for idx, h in enumerate(hs):
        if h == 0:
            done_rect.extend(stack)
            stack = []
            continue

        h_start = None
        while stack and stack[-1][0] >= h:
            last = stack.pop(-1)
            h_start = last[1]
            if last[0] > h:
                done_rect.append(last)

        for i in range(len(stack)):
            hi, si, _ = stack[i]
            stack[i] = (hi, si, idx)
    
        stack.append((h, idx if h_start is None else h_start, idx))

    done_rect.extend(stack)
    return done_rect

if __name__ == "__main__":
    print(largest_rect([1, 2, 3, 4]))
    print(largest_rect([2, 7, 5, 5, 1, 3, 3, 3, 3, 3, 3, 0, 12, 13, 10]))
