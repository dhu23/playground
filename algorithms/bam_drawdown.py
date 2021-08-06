#!/bin/python3

import math
import os
import random
import re
import sys



#
# Complete the 'drawdowncalc' function below.
#
# The function is expected to return a FLOAT.
# The function accepts following parameters:
#  1. FLOAT_ARRAY rets
#  2. INTEGER N
#

def drawdowncalc(rets, N):
    # Write your code here
    #print(rets)
    #print(N)
    
    #vals = list(map(lambda x : x+1, rets))
    #print(vals)
    
    drawdowns = [] # a list of drawdowns
    current_drawdown = 1.0
    current_drawdown_min = current_drawdown

    # current_drawdown_min tracks the mininum drawdown that
    # happened within this run (before the reset of current_drawdown)
    # this is before subtracting it by 1.0
    
    for idx, x in enumerate(rets):
        #print("at idx=", idx, ",val=", x, ",dd=", current_drawdown)
        new_drawdown = current_drawdown*(1.0+x)
        
        if new_drawdown >= 1.0:
            # back to profit
            if current_drawdown < 1.0:
                #print('adding dd=', current_drawdown_min)
                drawdowns.append(current_drawdown_min-1.0)

                current_drawdown = 1.0
                current_drawdown_min = 1.0
        else:
            if new_drawdown < current_drawdown:
                current_drawdown_min = new_drawdown
                #print('new dd min:', current_drawdown_min)
            current_drawdown = new_drawdown
            #print('new drawdown:', current_drawdown)
        
    # to count for the new update due to the last one
    if current_drawdown_min < 1.0:
        #print('adding dd at the end:', current_drawdown_min)
        drawdowns.append(current_drawdown_min-1.0)
            
    #print(drawdowns)
    
    nidx = N-1
    if nidx >= len(drawdowns):
        return len(drawdowns)
    else:
        # sort from lowest val(large drawdown) to highest val
        drawdowns.sort()
        #print(drawdowns)
        return round(drawdowns[nidx], 4)
        
# I realized that I need to merge drawdowns in the algorithm, basically
# using the idea of my linear algorithm, by the time of 1/15, using the 
# example plot, there should be two smaller drawdowns from 1/13 to 1/14 and
# another from 1/14 to 1/15
# due to the fact that both high points and low points of the following 
# drawdowns are lower than the previous one, they should be merged into 
# one bigger drawdown. That's why my algorithm would miss out the last
# drawdown (2nd largest drawdown) in the example.

# the second attempt is down here...just in case I can't get all done...

import numpy as np
def draw_down(rets, n):
    performance = np.cumprod(np.array([1.0] + rets)+1.0)
    
    dds = [] # indices for beginning and ending of a drawdown
    # as of the timepoint
    
    def _append_dds(start, end):
        if dds:
            # find the last dd
            last_start, last_end = dds[-1]
            if (
                performance[last_start] > performance[start_idx] and
                performance[last_end] > performance[end_idx]):
                dds.pop()
                dds.append((last_start, end_idx))
                return    
        dds.append((start_idx, end_idx))
            
    start_idx, end_idx = 0, 0
    for idx, x in enumerate(rets, start=1):
        if x < 0:
            end_idx = idx
        if x >= 0:
            if start_idx < end_idx:
                _append_dds(start_idx, end_idx)    
                    
            start_idx, end_idx = idx, idx
    
    if start_idx < end_idx:
        _append_dds(start_idx, end_idx)
            
    dd_vals = []
    for _start, _end in dds:
        dd_vals.append(performance[_end]/performance[_start]-1.0)
        
    print(dd_vals)
    
    nidx = N-1
    if nidx >= len(dd_vals):
        return len(dd_vals)
    else:
        # sort from lowest val(large drawdown) to highest val
        dd_vals.sort()
        return round(dd_vals[nidx], 4)

# use the new algorithm
drawdowncalc = draw_down

## for streaming data processing
class DDTracker(object):
    def __init__(self):
        self.dds = []
        self.start_idx, self.end_idx = 0, 0
        self.perf = [1.0]
        self.cumprod = 1.0
        self.idx = 0

    def _append_dds(self, dds):
        if dds:
            last_start, last_end = dds[-1]
            if (
                self.perf[last_start] > self.perf[self.start_idx] and 
                self.perf[last_end] > self.perf[self.end_idx]
            ):
                dds.pop()
                dds.append((last_start, self.end_idx))
                return

        dds.append((self.start_idx, self.end_idx))

    def add(self, x):
        self.idx += 1
        self.cumprod *= 1+x
        self.perf.append(self.cumprod)

        if x < 0: 
            self.end_idx = idx
        else:
            if self.start_idx < self.end_idx:
                _append_dds(self.dds)

            self.start_idx, self.end_idx = self.idx, self.idx

        
if __name__ == '__main__':
    fptr = open(os.environ['OUTPUT_PATH'], 'w')

    rets_count = int(input().strip())

    rets = []

    for _ in range(rets_count):
        rets_item = float(input().strip())
        rets.append(rets_item)

    N = int(input().strip())

    result = drawdowncalc(rets, N)
        
    fptr.write(str(result) + '\n')

    fptr.close()
