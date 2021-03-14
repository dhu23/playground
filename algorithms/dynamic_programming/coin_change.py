# S = {1, 2, 3} N = 4,
# solutions {1, 1, 1, 1}, {1, 1, 2}, {2, 2}, {1, 3}

# S = {2, 5, 3, 6} N = 10,
# solutions {2, 2, 2, 2, 2}, {2, 2, 3, 3}, {2, 2, 6}, {2, 3, 5}, {5, 5}

# it is in fact solving
# n1*s1 + n2*s2 + ... nm*sm = N

# then this can be done by backtracking
import itertools
import datetime

def time_run(func):
    def inner(*args, **kwargs):
        start = datetime.datetime.now()
        ret = func(*args, **kwargs)
        print func.__name__, 'cost=', datetime.datetime.now()-start
        return ret
    return inner

# S = [2, 5, 3, 6] N = 10
# [5, 0, 0, 0], [2, 0, 2, 0], [2, 0, 0, 1], [1, 1, 1, 0], [0, 2, 0, 0]

def is_feasible_partial_solution(solutions, S, N):
    total = sum ([coin*count for coin, count in itertools.izip(S, solutions)])
    if total == N:
        return True, True
    elif total < N:
        return True, False
    else:
        return False, False

def solve_coin_change_bt(solutions, fixate_index, S, N, count):
    solutions[fixate_index] = 0 # initialize variable
    feasible, complete = is_feasible_partial_solution(solutions, S, N)
    while feasible:
        if fixate_index == len(S)-1:
            if complete:
                print solutions
                count[0] += 1
        else: 
            solve_coin_change_bt(solutions, fixate_index+1, S, N, count)
            
        solutions[fixate_index] += 1
        feasible, complete = is_feasible_partial_solution(solutions, S, N)

    solutions[fixate_index] = 0 # back-tracking

# dynamic programming, for solving total number of ways
# Count(S[:], N) = Count(S[:-1], N)+Count(S, N-S[-1])
def count_coin_change_recursive(S, N):
    if N == 0: return 1
    if N < 0: return 0

    # now N > 0
    if not S: return 0
    return count_coin_change_recursive(
        S[:-1], N)+count_coin_change_recursive(S, N-S[-1])

@time_run
def count_coin_change_tail(S, N):
    return count_coin_change_recursive(S, N)

def count_coin_change_dp(S, N):
    pass

@time_run
def solve_coin_change(S, N):
    solutions = [0]*len(S)
    count = [0]
    solve_coin_change_bt(solutions, 0, S, N, count)
    print count
        
if __name__ == "__main__":
    # recursive calls depth will be 4. Each stack controls one variable
    solve_coin_change([2, 5, 3, 6], 10)
    # solve_coin_change([1, 2, 5, 10, 20, 50, 100], 326)
    solve_coin_change([100, 50, 20, 10, 5, 2, 1], 326)
    print count_coin_change_recursive([2, 5, 3, 6], 10)

    print count_coin_change_tail([1, 2, 5, 10, 20, 50, 100], 326)
