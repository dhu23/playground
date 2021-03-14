# -*- coding: utf-8 -*-
"""
Created on Sun Mar 12 16:05:56 2017

@author: daowen
"""

import numpy as np
import pandas as pd

def create_data_set(num=1):
    
    def make_random_list(sample_list, count):
        return [sample_list[np.random.randint(low=0, high=len(sample_list))] \
                for i in range(count)]
    
    output = []
    
    for i in range(num):
        #create a weekly (mondays) date range
        rng = pd.date_range(start='1/1/2009', end='12/31/2012', freq='W-MON')
        
        #create random data
        data = np.random.randint(low=25, high=1000, size=len(rng))
        
        #status pool
        status = [1, 2, 3]
        
        #Make a random list of statuses
        random_status = make_random_list(status, len(rng))
        
        # state pool
        states = ['GA', 'FL', 'fl', 'NY', 'NJ', 'TX']
        
        #make a random list of states
        random_states = make_random_list(states, len(rng))
        
        output.extend(zip(random_states, random_status, data, rng))
        
    return output
    
if __name__ == "__main__":
    print create_data_set(4)