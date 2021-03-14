# given an infinite number of quarters (25 cents), dimes (10 cents), nickel
# (5 cents) and pennies (1 cent), write code to calculate the number of ways
# of representing n cents

# solve a table as follows:
#
# value  coins[0:] coins[1:]  coins[2:]  ... coins[len-1:]
#    0      1        1          1                  1  
#    1                                             
#    2                          x
#   ...
#   100
def find_coin_2d(n, coins):
    coins.sort(reverse=True)
    coin_count = len(coins)
    table = [[None]*coin_count for i in range(n+1)]
    
    # first row sets to 1
    table[0] = [1]*coin_count
    #print(table)
    for i in range(1, n+1): # i looping from 1 to n
        for j in reversed(range(coin_count)): # j looping from #coin-1 to 0
            if j == coin_count-1: # using the smallest coin
                table[i][j] = 1 if i%coins[j] == 0 else 0
            else:
                if i < coins[j]: table[i][j] = table[i][j+1]
                else:
                    table[i][j] = table[i][j+1] + table[i-coins[j]][j]
    #print(table)
    return table[-1][0]

# this method can be optimized to use one row at a time instead of a 2d matrix


# another similar problem:
# given a list of coins with unlimited supply. make changes for #money = n
# with the minimum amount of coins
# [100, 25, 15, 10, 1] -> 30  (15+15, 2 coins)

def min_coin_change(coins, n):
    coins.sort()
    coin_len = len(coins)
    table = [[None]*coin_len for i in range(0, n+1)]
    table[0] = [0]*coin_len

    for r in range(1, n+1):
        for j in range(coin_len): # using coins coins[:j+1]
            if j == 0: # first column
                table[r][j] = r//coins[j] if r % coins[j] == 0 else None
            else: 
                # compare table[r][j-1] and table[r-coins[j]][j]+1
                if r >= coins[j] and table[r-coins[j]][j] is not None:
                    table[r][j] = table[r-coins[j]][j]+1
                if table[r][j-1] is not None:
                    if table[r][j] is None or table[r][j] > table[r][j-1]:
                        table[r][j] = table[r][j-1]
    print(table)
    return table[-1][-1]


if __name__ == "__main__":
    available_coins = [25, 10, 5, 1]
    print(find_coin_2d(44, available_coins))
    print(find_coin_2d(56, available_coins))
    print(find_coin_2d(100, available_coins))
    print(min_coin_change([100, 25, 15, 10, 1], 30))
