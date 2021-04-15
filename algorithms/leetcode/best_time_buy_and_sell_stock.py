# Problem I
# Sa you have an array for which the ith element is the price of a given
# stock on day i. If you were only permitted to complete at most one transcation
# (ie buy one and sell one share of the stock), design an algorithm to find the
# maximum profit

def profit1(prices):
    # go backwards, find local max, and update this local max only when
    # you encounter a larger local max, each possible max profit can happen
    # with an entry point in between these local maxs
    p_len = len(prices)
    local_max, profit = None, 0

    def check_local_max(l, i):
        assert 0 <= i < len(l) and len(l) > 2
        if i == len(l)-1: # last one
            return l[i] > l[i-1]
        elif i == 0:
            return l[0] > l[1]
        else:
            return l[i] > l[i-1] and l[i] > l[i+1]
    
    for i in reversed(range(p_len)):
        is_local_max = check_local_max(prices, i)
        if is_local_max and (local_max is None or local_max < prices[i]):
            local_max = prices[i]
        else:
            # not a local max, compute profit
            # if buy here and sold at current local_max price
            if local_max is not None:
                current_profit = local_max - prices[i]
                if profit < current_profit:
                    profit = current_profit
                    
    return profit


# Kadane's algorithm, or maximum subarray problem
def profit1_kadane(prices):
    p_len = len(prices)
    max_profit, max_so_far = 0, 0

    for i in range(1, p_len):
        max_so_far += prices[i]-prices[i-1]
        max_so_far = max(0, max_so_far)
        max_profit = max(max_profit, max_so_far)
    return max_profit

if __name__ == "__main__":
    title = 'best time to buy and sell stock III'
    print(title)
    print(profit1([3, 6, 7, 4, 3, 4, 6, 7, 8, 9, 10, 7]))
    print(profit1([7, 1, 5, 3, 6, 4]))
    print(profit1([7, 6, 4, 3, 1]))
    
    print(profit1_kadane([3, 6, 7, 4, 3, 4, 6, 7, 8, 9, 10, 7]))
    print(profit1_kadane([7, 1, 5, 3, 6, 4]))
    print(profit1_kadane([7, 6, 4, 3, 1]))

# Problem II
# Say you have an array for which the ith element is the price of a given
# stock on day i. Design an algorithm to find the max profit. You may
# complete as many transactions as you like(ie, buy one and sell one share
# of the stock multiple times), However you many not engage in multiple
# transactions at the same time(ie, you must sell the stock before you buy
# again)

# this is an extended Kadane algorithm, the difference is that you are allowed
# to include multiple non-overlapping subarray
def profit2(prices):
    p_len = len(prices)
    max_profit, max_so_far = 0, 0 # the final and max by the end of ith

    for i in range(1, p_len):
        max_so_far += max(0, prices[i]-prices[i-1])
        max_profit = max(max_profit, max_so_far)
    return max_profit

if __name__ == "__main__":
    title = 'Best time to buy and sell stock II'
    print(title)
    print(profit2([3, 6, 7, 4, 3, 4, 6, 7, 8, 9, 10, 7]))
    print(profit2([7, 1, 5, 3, 6, 4]))
    print(profit2([7, 6, 4, 3, 1]))

# Problem III
# say you have an array for which the ith element is the price of a given
# stock on day i. Design an algorithm to find the maximum profit. You may
# complete at most two transactions. You may not engage in multiple transactions
# at the same time (ie you must sell the stock before you buy again)

def profit3(prices):
    p_len = len(prices)

    max_profit, max_so_far = [0, 0], 0
    for i in range(1, p_len):
        max_so_far += prices[i]-prices[i-1]
        max_so_far = max(0, max_so_far)

        assert len(max_profit) == 2 and max_profit[0] >= max_profit[1]
        if max_so_far <= max_profit[1] or max_so_far == max_profit[0]:
            pass
        elif max_so_far > max_profit[0]:
            max_profit = [max_so_far, max_profit[0]]
        else:
            max_profit = [max_profit[0], max_so_far]
    return sum(max_profit), max_profit

if __name__ == "__main__":
    title = 'Best time to buy and sell stock III'
    print(title)
    print(profit3([3, 6, 7, 4, 3, 4, 6, 7, 8, 9, 10, 7]))
    print(profit3([7, 1, 5, 3, 6, 4]))
    print(profit3([7, 6, 4, 3, 1]))
