#!/usr/bin/env python3
# -*- coding: utf-8 -*-

'''
grep validateNewOrd /home/qbsim/te/logs/qbstrats1.20190806.log | grep transactTime > /mnt/hc/qbsim/te/te1_neword.20190806
grep "ChildOrder::on" /home/qbsim/te/logs/qbstrats1.20190806.log | grep transactTime > /mnt/hc/qbsim/te/te1_onmsg.20190806
grep "Dispatcher" /home/qbsim/te/logs/qbstrats1.20190806.log | grep "TE Received" > /mnt/hc/qbsim/te/te1_dispatcher.20190806
grep "QB::TE::TradingEngine::postEvent()" qbstrats2.20190807.log > /mnt/hc/qbsim/te/te2_push.20190807
grep "QB::TE::TradingEngine::run()" qbstrats2.20190807.log > /mnt/hc/qbsim/te/te2_pop.20190807
'''

from datetime import datetime
import re
# import pprint
import matplotlib.pyplot as plt
import numpy as np
import sys
import pandas as pd

def cleanTime(t):
    return t.split('.')[0]

PATTERN1 = '%Y-%m-%d %H:%M:%S'
PATTERN2 = '%Y.%m.%dT%H:%M:%S'
PATTERN3 = '%Y-%m-%d %H:%M:%S.%f'
PATTERN4 = '%Y.%m.%dT%H:%M:%S.%f'

def str2time(s, pattern):
    try:
        return datetime.strptime(s.strip(), pattern)
    except:
        print(s, pattern)
        sys.exit(0)

'''
def parse_transact_line(line):
    m = re.match('Received:(.*) - Sent:(.*) - type: (.*) Diff.*', line)
    recv_str, send_str, msg_type = m.groups()
    recv_t = str2time(cleanTime(recv_str), PATTERN1)
    send_t = str2time(cleanTime(send_str), PATTERN1)
    
    return recv_t, send_t, msg_type.strip()
'''

def parse_onmsg_line(line):
    recv_t = str2time(line[1:20], PATTERN2)
    msg_type = line.split('cho :')[1].lstrip().split(':')[0]
    send_t = str2time(cleanTime(line.split('transactTime=')[1][:23]), PATTERN3)
    lag_in_sec = (recv_t-send_t).total_seconds()
    return recv_t, send_t, lag_in_sec, msg_type

def parse_dispatch_line(line):
    recv_t = str2time(line[1:20], PATTERN2)
    # print('recv_t %s' % recv_t)
    msg_type = line.split('TE Received')[1].split(':')[1].split()[-1].strip()
    # print(cleanTime(line.split('time=')[1][:19]))
    sym = line.split('TE Received symbol:')[1].lstrip().split()[0]
    send_t = str2time(cleanTime(line.split('time=')[1][:23]), PATTERN3) 
    
    # print(recv_t, send_t, msg_type)
    return recv_t, send_t, msg_type, sym

def extract_event_data(s):
    src = s.split(',')[0].split('src=')[1].strip()
    data_str = s.split('`data=')[1]
    data_type = data_str.split(':')[0].strip()
    txn_t = str2time(data_str.split('ime=')[1][:23], PATTERN3)
    return {'src': src, 'data_type': data_type, 'txn_t': txn_t}

def calc_lag(d):
    recv_lag = (d['recv_t']-d['txn_t']).total_seconds()
    push_lag = (d['done_t']-d['recv_t']).total_seconds()
    total_lag = (d['done_t']-d['txn_t']).total_seconds()
    return {'recv_lag': recv_lag, 'push_lag': push_lag, 'total_lag': total_lag}

class PushParser(object):
    def __init__(self):
        self.data, self.rec = [], []
        
    def __call__(self, line):
        if 'trying to post event to eventQ' in line and 'ime=' in line:
            rec = {}
            rec['recv_t'] = str2time(line[1:24], PATTERN4)
            event_q_part = line.split('trying to post event to eventQ:')[1]
            rec.update(extract_event_data(event_q_part))
            self.rec.append(rec)
            
        elif 'event pushed successfully' in line:
            if self.rec:
                self.rec[0]['done_t'] = str2time(line[1:24], PATTERN4)
                self.rec[0].update(calc_lag(self.rec[0]))
                self.data.append(self.rec.pop(0))


class PopParser(object):
    def __init__(self):
        self.data, self.rec = [], []
    
    def __call__(self, line):
        if 'obtained an event from eventQueue:' in line and 'ime=' in line:
            rec = {}
            rec['recv_t'] = str2time(line[1:24], PATTERN4)
            event_q_part = line.split('obtained an event from eventQueue:')[1]
            rec.update(extract_event_data(event_q_part))
            self.rec.append(rec)
        
        elif 'finished dispatching event' in line:
            if self.rec:
                self.rec[0]['done_t'] = str2time(line[1:24], PATTERN4)
                self.rec[0].update(calc_lag(self.rec[0]))
                self.data.append(self.rec.pop(0))

def get_x_y(tuple_list):
    x = np.array([i[0] for i in tuple_list]).astype("datetime64")
    y = np.array([i[1] for i in tuple_list])
    return (x, y)

'''
def parse_log(log_file, line_parser):
    data = {}
    with open(log_file, 'r') as f:
        for line in f:
            recv_t, send_t, key = line_parser(line)
            lag_in_sec = (recv_t - send_t).total_seconds()
            data.setdefault(key, []).append((recv_t, lag_in_sec))
    print('parsed %s' % log_file)
    
    pdata = dict((key, get_x_y(d)) for key, d in data.items())
    return pdata
'''
def make_df(log_file, cols, line_parser):
    data = {}
    col_num = len(cols)
    print('started parsing %s' % log_file)
    with open(log_file, 'r') as f:
        for line in f:
            rec = line_parser(line)
            assert col_num == len(rec)
            for col, d in zip(cols, rec):
                data.setdefault(col, []).append(d)
    print('parsed %s' % log_file)
    return pd.DataFrame(data)

def make_df_dict(log_file, stateful_parser):
    with open(log_file, 'r') as f:
        #count = 0
        for line in f:
            #if count > 2000: break
            stateful_parser(line)
            #count += 1
            
    ret = {}
    for d in stateful_parser.data:
        #if 'recv_lag' not in d:
        #    print(d)
        for col_name, val in d.items():
            ret.setdefault(col_name, []).append(val)
    #print(ret)
    #for cn, d in ret.items():
    #    print(cn, len(d))
    return pd.DataFrame(ret)

def by_inst(inst):
    def inner(key):
        return key[1].startswith(inst)
    return inner

def by_sym(sym):
    def inner(key):
        return key[1] == sym
    return inner

def by_all():
    def inner(key):
        return True
    return inner

def select(data, key_predicate):
    ret = [d[0] for key, d in data.items() if key_predicate(key)]
    return np.hstack(ret)

def read_onmsg(log_file):
    return make_df(log_file, 
                   ["recv_t", "send_t", "lag", "msg_type"], 
                   parse_onmsg_line)

def read_dispatcher(log_file):
    return make_df(log_file, 
                   ["recv_t", "send_t", "msg_type", "sym"],
                   parse_dispatch_line)
    
def read_push(log_file):
    return make_df_dict(log_file, PushParser())

def read_pop(log_file):
    return make_df_dict(log_file, PopParser())

def process_files(tagged_files, proc):
    return dict((t, proc(file_name)) for t, file_name in tagged_files.items())

if __name__ == "__main__":
    
    #transact_data = parse_log('te3_transact_lag2.txt', parse_transact_line)
    #dispatch_data = parse_log('te3_dispatch_lag.txt', parse_dispatch_line)

    #onmsg_data1 = read_onmsg('te1_onmsg.20190806')
    #dispatch_data1 = read_dispatcher('te1_dispatcher.20190806')
    
    #onmsg_data2 = read_onmsg('te2_onmsg.20190806')
    #dispatcher_data2 = read_dispatcher('te2_dispatcher.20190806')
    
    onmsg_data = dict(zip(
            ['te1', 'te2', 'te3', 'te4'],
            map(read_onmsg, ['te1_onmsg.20190806', 
                             'te2_onmsg.20190806', 
                             'te3_onmsg.20190806',
                             'te4_onmsg.20190806'])))
    dispatcher_data = dict(zip(
            ['te1', 'te2', 'te3', 'te4'],
            map(read_dispatcher, ['te1_dispatcher.20190806',
                                  'te2_dispatcher.20190806',
                                  'te3_dispatcher.20190806',
                                  'te4_dispatcher.20190806'])))
    
    #onmsg_data7 = read_onmsg('te1_onmsg.20190807')
    #dispatcher_data7 = read_dispatcher('te1_dispatcher.20190807')
    
    #push_data = read_push('te2_push.20190807')
    #eventerr_data = read_eventerr('te2_eventerr.20190807')
    #pop_data = read_pop('te2_pop.20190807')
    
    '''
    onmsg_prod7 = process_files(
            {'te1': 'prod_te1_onmsg.20190807', 'te2': 'prod_te2_onmsg.20190807'},
            read_onmsg)
    
    dispatcher_prod7 = process_files(
            {'te1': 'prod_te1_dispatcher.20190807', 'te2': 'prod_te2_dispatcher.20190807'},
            read_dispatcher)
    '''
    pd.set_option('display.max_rows', 50)
    pd.set_option('display.max_columns', 40)
    pd.set_option('display.width', 500)
    plt.rcParams.update({'figure.figsize': (12, 8), 'figure.dpi': 100})