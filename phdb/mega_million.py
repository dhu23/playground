import random
import datetime
import multiprocessing as mp

white_ball_sample = range(1, 71) # 1 to 70
yellow_ball_sample = range(1, 26) # 1 to 25

prizes = {
    (5, 1): 1600000000,
    (5, 0): 1000000,
    (4, 1): 10000,
    (4, 0): 500,
    (3, 1): 200,
    (3, 0): 10,
    (2, 1): 10,
    (1, 1): 4,
    (0, 1): 2
    }

def generate_series():
    sample = random.sample
    return sample(white_ball_sample, 5), sample(yellow_ball_sample, 1)

def check_match(drawing_set, game):
    white_drawing, yellow_drawing = drawing_set
    white_game, yellow_game = game

    white_match = len([g for g in game if g in drawing_set])
    yellow_match = int(yellow_drawing[0] == yellow_game[0])
    return white_match, yellow_match


def calculate_reward(args):
    num_game, drawing_set = args
    reward = 0
    for _ in range(num_game):
        game = generate_series()
        # print(game)
        white_match, yellow_match = check_match(drawing_set, game)
        if (white_match, yellow_match) in prizes:
            prize = prizes[(white_match, yellow_match)]
            # print(game, prize)
            reward += prize
    return reward


def simulate_game(num_game):
    white_drawing, yellow_drawing = generate_series()
    
    drawing_set = (set(white_drawing), yellow_drawing)
    print('=== Drawing : ', drawing_set)

    cost = 2*num_game
    if num_game <= 1000:
        reward = calculate_reward((num_game, drawing_set))
    else:
        with mp.Pool(6) as p:
            rs = p.map(calculate_reward, [(num_game//100, drawing_set)]*100)
            reward = sum(rs)
        
    return "{:,}".format(cost), "{:,}".format(reward), reward/cost
            
if __name__ == "__main__":
    start = datetime.datetime.now()
    
    print(simulate_game(1))
    print(simulate_game(10))
    print(simulate_game(100))
    print(simulate_game(1000))
    print(simulate_game(10000))
    print(simulate_game(100000))
    print(simulate_game(1000000))
    print(simulate_game(10000000))
    # print(simulate_game(100000000))
    # print(simulate_game(1000000000))
    # 10B tickets
    # print(simulate_game(10000000000)) 

    end = datetime.datetime.now()
    print('total time=', end-start)
