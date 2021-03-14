import time

X, O, NONE = ("X", "O", ".")


class Board():
    def __init__(self):
        self.board_length = 9;
        self.squares = [NONE for idx in range(self.board_length)]
        self.winning_combinations = [
            [0,1,2], [3,4,5], [6,7,8],
            [0,3,6], [1,4,7], [2,5,8],
            [0,4,8], [2,4,6]]
        self.col_score = [0, 0, 0]
        self.row_score = [0, 0, 0]
        self.left_diag, self.right_diag = 0, 0

    def __str__(self):
        #return  ("-------------\n| {} | {} | {} |\n" * 3 + "-------------").format(*self.squares)
        return ''.join(self.squares)

    def update_score(self, location, make_move=True):
        if self.squares[location] == X:
            score = 1
        elif self.squares[location] == O:
            score = -1
        else:
            return
        score *= (1 if make_move else -1)
        i, j = divmod(location, 3)
        self.row_score[i] += score
        self.col_score[j] += score
        if i == j: self.left_diag += score
        if i+j == 2: self.right_diag += score
            
    def make_move(self, location, player):
        if self.is_valid_move(location):
            self.squares[location] = player
            self.update_score(location, True)
            return True
        return False

    def undo_move(self, location):
        if self.is_valid_location(location):
            self.update_score(location, False)
            self.squares[location] = NONE
            return True
        return False

    def is_valid_move(self, location):
        return self.is_valid_location(location) and self.squares[location] == NONE

    def is_valid_location(self, location):
        return location >= 0 and location < self.board_length

    def get_winner(self):
        '''
        col_scores = [0, 0, 0]
        left_diag, right_diag = 0, 0
        for i in range(3):
            row_score = 0
            for j in range(3):
                
                score = 1 if self.squares[3*i+j] == X else -1 if self.squares[3*i+j] == O else 0
                row_score += score
                col_scores[j] += score

                if i == j: left_diag += score
                if i+j == 2: right_diag += score
                if i == 2:
                    if col_scores[j] == 3: return X
                    elif col_scores[j] == -3: return O
                    if right_diag == 3: return X
                    elif right_diag == -3: return O
                    if left_diag == 3: return X
                    elif left_diag == -3: return O
            if row_score == 3: return X
            elif row_score == -3: return O
        return NONE
        '''

        
        if 3 in self.col_score: return X
        elif -3 in self.col_score: return O
        if 3 in self.row_score: return X
        elif -3 in self.row_score: return O
        if self.left_diag == 3: return X
        elif self.left_diag == -3: return O
        if self.right_diag == 3: return X
        elif self.right_diag == -3: return O
        return NONE
        
        
        '''
        for player in (X, O):
            for combo in self.winning_combinations:
                if self.squares[combo[0]] == player and self.squares[combo[1]] == player and self.squares[combo[2]] == player:
                    # print(combo)
                    return player
        return NONE
        '''

    def get_moves(self):
        return filter(self.is_valid_move, range(9))

iters = 0
def find_winners(board, depth=0, player=X):
    global iters
    iters += 1
    for move in board.get_moves():
        board.make_move(move, player)
        if board.get_winner() == player:
            #print(board)
            _ = 1
        else:
            find_winners(board, depth + 1, O if player == X else X)
        board.undo_move(move)

if __name__ == "__main__":
    start = time.time()
    find_winners(Board())
    print("Took: {time:.4f} seconds to execute {iters} times.".format(time=(time.time() - start), iters=iters))
    #b = Board()
    #b.squares = list('X.XOXOXO.')
    
    #print(b.get_winner())
