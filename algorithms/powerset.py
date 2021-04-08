def print_power_set(ns):
    stack = list(ns)
    n_len = len(ns)

    while True:
        assert len(stack) == n_len
        print(list(filter(lambda x: x is not None, stack)))

        if stack[-1] is not None:
            stack[-1] = None
        else:
            # backtracking
            while stack and stack[-1] is None:
                stack.pop()
            if not stack:
                break
            else:
                stack[-1] = None
                stack.extend(ns[len(stack):])
        

if __name__ == "__main__":
    print_power_set([1, 2, 3, 4])
