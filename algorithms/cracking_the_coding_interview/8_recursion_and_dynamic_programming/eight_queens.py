# write an algorithm to print all ays of arranging eight queens on an 8X8
# chess board so that none of them share the same row, column or diagonal.
# In this case, diagonal means all diagonals, not just the two that bisect
# the board

def solve(board, n):
    def is_leaf(b):
        #return len([c for c in row for row in b if c == 'Q']) == n
        return len(b) == n
    
    def available_slots(b):
        taken_xs = set(each[0] for each in b)
        taken_ys = set(each[1] for each in b)
        taken_left_diag = set(each[0]-each[1] for each in b)
        taken_right_diag = set(each[0]+each[1] for each in b)
        ret = []
        for i in range(n):
            for j in range(n):
                if i in taken_xs or j in taken_ys: continue
                if i-j in taken_left_diag or i+j in taken_right_diag: continue
                ret.append((i, j))
        #print('board', b)
        #print('available', ret, len(ret))
        return ret
    
    if is_leaf(board):
        print('solved board=', board)
        return True
    else:
        for each in available_slots(board):
            board.append(each)
            solved = solve(board, n)
            board.pop()
            #if solved: return True
            #else: continue # meaning each is not good. try next
        

def n_queens(n):
    # board = [[' ']*n for i in range(n)]
    board = []
    print(solve(board, 8))
    print(board)


if __name__ == "__main__":
    n_queens(8)
