import random
import datetime
import math

import numpy
import scipy
from scipy import stats

def verify_sorted_data(data_list):
    for i, d in enumerate(data_list[:-1]):
        if d > data_list[i+1]:
            raise Exception("data.input.NOT.sorted")

def get_cost(start):
    return (datetime.datetime.now()-start).total_seconds()


def brute_force_2_sum(data_list, target):
    results = []
    start = datetime.datetime.now()
    for i, d1 in enumerate(data_list):
        for j, d2 in enumerate(data_list[i+1:]):
            if d1 + d2 == target:
                results.append((d1, d2))
    return results, get_cost(start)


def brute_force_3_sum(data_list, target):
    results = []
    start = datetime.datetime.now()
    for i, d in enumerate(data_list):
        two_sum, _ = brute_force_2_sum(data_list[i+1:], target-d)
        for n1, n2 in two_sum:
            results.append((d, n1, n2))
    return results, get_cost(start)


def print_func(data_list, lo, hi):
    if(data_list[lo] < 70):
        print lo, hi, data_list[lo], data_list[hi]


def find_2_sum(data_list, target, is_sorted=False):
    start = datetime.datetime.now()

    # sort data
    if not is_sorted:
        data_list.sort()

    lo, hi = 0, len(data_list)-1
    results = []
    while lo < hi:

        while data_list[lo] + data_list[hi] < target and lo < hi:
            lo += 1

        while data_list[lo] + data_list[hi] > target and lo < hi:
            hi -= 1

        # now the sum is less or equal to the target
        if lo >= hi:
            break

        if data_list[lo] + data_list[hi] == target:
            results.append((data_list[lo], data_list[hi]))
        
        lo += 1

    return results, get_cost(start)


def find_3_sum(data_list, target):
    results = []
    start = datetime.datetime.now()
    data_list.sort()

    for i, d in enumerate(data_list):
        if d*3 > target:
            break
        two_sum, _ = find_2_sum(data_list[i+1:], target-d, True)
        for n1, n2 in two_sum:
            results.append((d, n1, n2))
    return results, get_cost(start)


def find_2_sum_pair(data_list): # a_i + a_j = a_k + a_l
    # use hash table or radix sort idea
    pass
    

def analyze_complexity(input_data, sum_func):
    n_list = []
    cost_list = []
    for input_list, target in input_data:
        _, cost = sum_func(input_list, target) 
        print len(input_list), cost, math.log(cost)
        n_list.append(len(input_list))
        cost_list.append(cost)

    ns = numpy.array(n_list)
    costs = numpy.array(cost_list)
    return stats.linregress(numpy.log(ns), numpy.log(costs))


def generate_data(seed_list, repeat):
    '''seed input is the size of the generated list. The list is generated
    by randomly sampling size data points from 1 to 10*size. Target is set
    at 5*size'''
    data = []
    for seed in seed_list:
        for i in xrange(repeat):
            random.seed(seed)
            data.append(
                    (random.sample(xrange(seed*10), seed), seed*5))
    return data

def compare_2_sum_algo():

    test_data = generate_data(
            [10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000], 5)
    lope1, intercept1, r_value1, p_value1, std_err1 = analyze_complexity(
            test_data, brute_force_2_sum)
    lope2, intercept2, r_value2, p_value2, std_err2 = analyze_complexity(
            test_data, find_2_sum)
    print slope1, slope2


def compare_3_sum_algo():

    test_data = generate_data(
            [10, 20, 50, 100, 200, 400, 800, 1000, 1200], 1)
    slope1, _, _, _, _ = analyze_complexity(test_data, brute_force_3_sum)
    test_data = generate_data(
            [10, 50, 100, 500, 1000, 2000, 5000, 10000, 20000], 2)
    slope2, _, _, _, _ = analyze_complexity(test_data, find_3_sum)
    print slope1, slope2

def verify_algo():
    random.seed(100)
    test_data = random.sample(xrange(1500), 250)
    target = 750

    print brute_force_2_sum(test_data, target)
    print brute_force_3_sum(test_data, target)
    
    print find_2_sum(test_data, target)
    print find_3_sum(test_data, target)



if __name__ == "__main__":
    
    verify_algo()
    # compare_2_sum_algo()
    # compare_3_sum_algo()
