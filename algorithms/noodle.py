# -*- coding: utf-8 -*-
"""
Created on Wed Feb  1 22:48:00 2017

@author: daowen
"""

import math
import random
import datetime

# the system evolves from state 0 to 1, 2, 3, ... N. Each state there are 
# N, N-1, N-2, ... strings in the bowl, regardless the number of loops.
# at state 0, count of loops = 0 and the probability of generating a new 
# loop from state n-1 to n, prob(n-1) depends on the number of strings at 
# state n-1, which is N-n+1. prob(L, n-1) = 1/(2*(N-n+1)-1)


N = 20
prob = [1.0/(2*(N-i+1)-1) for i in xrange(1, N+1)]

prob_rec = [1.0/p for p in prob]

print prob, len(prob)
print prob_rec, math.factorial(N)

# at state n, x is the count of loops
# p(x=n, n) = p(x=n-1, n-1)*prob(L, n-1)
# p(x=0, n) = p(x=0, n-1)*(1-prob(L, n-1))
# all else p(x, n) = p(x, n-1)*(1-prob(L, n-1))+p(x-1, n-1)*prob(L, n-1)

def flip(p):
    return 1 if random.random() < p else 0


start = 0
num_path = 20000 #  math.factorial(N) # 5000 paths
total_loops = 0
start_time = datetime.datetime.now()
for pth in xrange(num_path): 
    # path = [start]
    path = start
    for i in xrange(N):
        # path.append(path[-1]+flip(prob[i]))
        path += flip(prob[i])
    # print path
    total_loops += path
e_loops = total_loops*1.0/num_path
print e_loops, datetime.datetime.now()-start_time