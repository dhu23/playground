# this is a typical backtracking problem

def possible_moves(used_left, used_right, n):
    assert used_left <= n
    assert used_right <= n
    assert used_left >= used_right
    if used_right == n:
        return []
    if used_left == n:
        return [')']
    elif used_left == used_right:
        return ['(']
    else:
        return ['(', ')']
        
def parens(stack, used_left, used_right, n):
    assert n > 0

    moves = possible_moves(used_left, used_right, n)
    if moves:
        for move in moves:
            
            if move == '(': used_left += 1
            elif move == ')': used_right += 1
            stack.append(move)

            parens(stack, used_left, used_right, n)
            
            if move == '(': used_left -= 1
            elif move == ')': used_right -= 1
            stack.pop()
    else:
        print(''.join(stack))


if __name__ == "__main__":
    parens([], 0, 0, 4)
