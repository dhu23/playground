import multiprocessing as mp
import time

NUMS = list(range(10000000))

def sum_func(idx, name):
    ret = 0
    for x in NUMS:
        if x % 100000 == 0:
            print('sleeping for idx=%d, name=%s, x=%d' % (idx, name, x))
            time.sleep(5)
        ret += x
    print('idx=%d, name=%s, ret=%d' % (idx, name, ret))
    return ret


if __name__ == '__main__':
    mp.Pool(4).starmap(
        sum_func, 
        [
            (1, 'one'), (2, 'two'), 
            #(3, 'three'), (4, 'four')
        ])
