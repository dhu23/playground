# design an algorithm to figure out if someone has won a game of tic-tac-toe

# there is almost inefficient algorithms or brute force algorithms
# try to find the best one




def find_winner(game):        
    # try something as follows:
    # as we read in the 2-d array, track the following variables:
    # 1) is the current line a mix of both x and o
    # 2) track 3 scores for each col, x = +1, o = -1
    # 3) track diag scores too.
    #    left diag (0, 0), (1, 1), (2, 2). right diag (0, 2), (1, 1), (2, 0)
    col_scores = [0, 0, 0]
    left_diag, right_diag = 0, 0
    
    for i in range(3):
        row_score = 0
        for j in range(3):
            score = 1 if game[i][j] == 'x' else -1
            row_score += score
            col_scores[j] += score

            if i == j:
                left_diag += score
            if i+j == 2:
                right_diag += score

            if i == 2: # at the last row
                if col_scores[j] == 3:
                    return 'x'
                if col_scores[j] == -3:
                    return 'o'
                if right_diag == 3:
                    return 'x'
                if right_diag == -3:
                    return 'o'
                if left_diag == 3:
                    return 'x'
                if left_diag == -3:
                    return 'o'
                
        if row_score == 3:
            return 'x'
        if row_score == -3:
            return 'o'
    return None
    
if __name__ == "__main__":
    # a game without winner
    game1 = [['x', 'o', 'x'],
            ['x', 'x', 'o'],
            ['o', 'x', 'o']]

    print(find_winner(game1))

    game2 = [['x', 'x', 'o'],
             [' ', 'x', ' '],
             ['o', 'x', 'o']]

    print(find_winner(game2))
