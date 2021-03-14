#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Nov 17 23:05:55 2017

@author: daowen
"""

# coroutines

from collections import namedtuple
Result = namedtuple('Result', 'count average')
# the subgenerator
def averager():
    total = 0.0
    count = 0
    average = None
    while True:
        print('yielding', count)
        term = yield count
        print('received term', term)
        if term is None:
            break
        total += term
        count += 1
        average = total/count
    print('returning', count, average)
    return Result(count, average)

# the delegating generator
def grouper(results, key):
    print('entered grouper(%s)' % key)
    if True:#while True:
        print('in grouper while with key=', key)
        results[key] = yield from averager()
        print('results[key] assigned', key, results[key])
    print('leaving grouper %%%%%%%%%%%%%%%')

# the client code, a.k.a. the caller
def main(data):
    results = {}
    for key, values in data.items():
        print('-------------------\nprocessing key=', key)
        group = grouper(results, key)
        ret = next(group)
        print('ret of priming', ret)
        for value in values:
            print('sending', value)
            ret = group.send(value)
            print('received', ret)
        group.send(None) # important!
    # print(results)
    report(results)

# uncomment to debug
# output report

def report(results):
    for key, result in sorted(results.items()):
        group, unit = key.split(';')
        print('{:2} {:5} averaging {:.2f}{}'.format(
                result.count, group, result.average, unit))
        
data = {
        'girls;kg':
            [40.9, 38.5, 44.3, 42.2, 45.2, 41.7, 44.5, 38.0, 40.6, 44.5],
        'girls;m':
            [1.6, 1.51, 1.4, 1.3, 1.41, 1.39, 1.33, 1.46, 1.45, 1.43],
        'boys;kg':
            [39.0, 40.8, 43.2, 40.8, 43.1, 38.6, 41.4, 40.6, 36.3],
        'boys;m':
            [1.38, 1.5, 1.32, 1.25, 1.37, 1.48, 1.25, 1.49, 1.46],
        }

if __name__ == '__main__':
    main(data)