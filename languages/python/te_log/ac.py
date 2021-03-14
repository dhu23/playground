#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Sep  6 12:14:20 2019

@author: daowenhu
"""

import json
import datetime
import pprint

# ac book looks like the following conceptually
'''
"asks":
102.00000000       1
              NONE=S2,Key=QB|ZNZ9|CME,px=102.00000000[S],freed[1]
101.00000000       1
              NONE=S1,Key=QB|ZNZ9|CME,RplBy=RPL_S1,px=101.00000000[S],freed[1]
96.00000000       1
              RPL=RPL_S1,Key=QB|ZNZ9|CME,RefOrdId=S1,px=96.00000000[S],freed[0]

"bids":
96.00000000       1
              NONE=B1,Key=QB|ZNZ9|CME,CxlBy=CXL_B1,px=96.00000000[B],freed[1]
95.00000000       1
              NONE=B2,Key=QB|ZNZ9|CME,px=95.00000000[B],freed[1]
 '''
 
def str2time(s):
    return datetime.datetime.strptime(s, "%Y.%m.%dT%H:%M:%S.%f")

def extract_substr(line, token1, token2=None):
    idx1 = line.index(token1)
    if idx1 < 0:
        raise Exception('token1=%s not found' % token1)
    
    substr_start = idx1+len(token1)
    
    if token2 is None:
        return line[substr_start:].strip()
    
    idx2 = line.index(token2, idx1+1)
    if idx2 < 0:
        raise Exception('token2=%s not found' % token2)
    
    substr_end = idx2
    return line[substr_start:substr_end].strip()

# checking no crossing at exchange for an AC book
def verify_acbook(acbook):
    assert "asks" in acbook
    assert "bids" in acbook
    
    def has_freed_ord(level):
        return any(o['isFreed'] for o in level)
    
    def get_prices_at_exchange(book_side):
        return [float(price) for price, level in book_side.items() 
                if has_freed_ord(level)]

    min_ask = min(get_prices_at_exchange(acbook['asks']))
    max_bid = max(get_prices_at_exchange(acbook['bids']))
    
    return min_ask > max_bid

# ac is a map of anticross key to acbook
def verify_ac(ac):
    for k, acbook in ac.items():
        if not verify_acbook(acbook):
            print('key=%s' % k)
            pprint.pprint(acbook)

# build parent - child relation
            
# collect all blocking orders and blocked orders
            
# check time diff between terminal state from exchange and remove from ac book

def get_log_time(l): 
    return str2time(l[1:24])

class ACAnalyzer(object):
    def __init__(self):
        self.acbook_list, self.ac_list = [], []
        self.child_ord_store = {} # only contain original child orders
        
        self.child_map = {} # map to original order id, not the ref
        
    def process_line(self, line):
        if "ACBook in json:" in line:
            acbook = json.loads(extract_substr(line, 'in json:'))
            self.acbook_list.append(get_log_time(line), acbook)

        elif "AntiCross in json:" in line:
            ac = json.loads(extract_substr(line, 'in json:'))
            self.ac_list.append(get_log_time(line), ac)
            
        elif 'checkBookCrossing' in line and 'checking incoming order:' in line:
            action = extract_substr(line, 'checking incoming order:', '=')
        
            child_ord = extract_substr(line, action+'=', ',')
            self.child_ord_store[child_ord] = {
                    'ordid': child_ord,
                    'is_orig': action == "NEW",
                    'attempt_t': get_log_time(line), 
                    'inserted': False,
                    }
            
        elif "ChildOrder::sendNewOrd():" in line and 'NewOrd:' in line:            
            self._update_sent_ord(line)
        
        elif "ChildOrder::sendRplOrd():" in line and 'RplOrd' in line:
            child_ord, parent_ord = self._update_sent_ord(line)
            ref_ord = extract_substr(line, 'refOrdId=', ' ')
            
            orig_ord = self.child_map.get(ref_ord, ref_ord)
            self.child_map[child_ord] = orig_ord
            
            assert orig_ord in self.child_ord_store
            assert parent_ord == self.child_ord_store[orig_ord]['parent']
            self.child_ord_store[orig_ord].setdefault('rpl', []).append(child_ord)
                
        elif "ACBook::insertToOrderBook():" in line:
            child_ord = extract_substr(line, 'into ACBook:')
            self.child_ord_store[child_ord].update({'inserted': True})
            
        elif "ACBook::checkInsertOrder():" in line:
            if 'New ord rejected by ac:' in line:
                self._update_block(line)
            elif 'Rpl ord rejected by ac:' in line:
                self._update_block(line)
                
    def _update_sent_ord(self, line):
        child_ord = extract_substr(line, 'ordId=', ' ')
        parent_ord = extract_substr(line, 'parentOrdId=', ' ')
        self.child_ord_store[child_ord].update({
                'parent': parent_ord,
                'sent_t': get_log_time(line),
                })
        return child_ord, parent_ord
    
    def _update_block(self, line):
        blocked_ord = extract_substr(line, 'rejected by ac:', '=')
        blocking_ord = extract_substr(line, 'blocked by:', '=')
        
        self.blocks[blocked_ord] = {
                'by': blocking_ord, 
                't': get_log_time(line)
                }
        
    def __repr__(self):
        pass
        
def analyzie_ac(file_name):
        
    tool = ACAnalyzer()
    
    with open(file_name, 'r') as f:
        for line in f:
           tool.process_line(line) 


if __name__ == "__main___":
    pass