
# for items from 1 to i, with budget b_i, the solution is F(i, b_i)
# this is very similar to coin change problem
# it can be solved by dividing the solution into two sets:
# 1) set one that doesn't use item i, therefore F(i-1, b_i)
# 2) set two that uses i, therefore F(i-1, b_i-c(i))

def solve_knapsack_recursive(items, budget):
    if len(items) == 0: return 0
    if budget <= 0: return 0

    if items[-1][1] > budget:
        return solve_knapsack_recursive(items[:-1], budget)

    return max(
        solve_knapsack_recursive(items[:-1], budget),
        items[-1][0]+solve_knapsack_recursive(items[:-1], budget-items[-1][1]))

################### dynamic programming #####################
# solve F(i, j), i-# of first items, j-budget
#  i/j     0    1    2    3    4    5 ..... N
#   0      0    0    0    0    0    0 ..... 0
#   1      0    
# ...
# budget   0
# F0 := 0; Fi := Fi-1 if x < ci  max(Fi-1(x), Fi-1(x-ci)) if x >= ci
def solve_knapsack_dp_int(items, budget):
    weight_table = []
    for i in range(budget+1):
        weight_table.append([0]*(len(items)+1))

    for x in range(1, budget+1):
        # for each weight budget x
        for i in range(1, len(items)+1):
            # for each items
            w = items[i-1][1]
            benefit = items[i-1][0]
            if w > x:
                weight_table[x][i] = weight_table[x][i-1]
            else:
                weight_table[x][i] = max(
                    weight_table[x][i-1], benefit+weight_table[x-w][i-1])

    for row in weight_table:
        print row

if __name__ == "__main__":
    # benefit and cost
    items = [
        (20, 2), (12, 15), (3, 1), (4, 24), (1, 2), (45, 54), (23, 17),
        (25, 40), (22, 2), (18, 9)]

    budget = 75

    print solve_knapsack_recursive(items, budget)
    solve_knapsack_dp_int(items, budget)
