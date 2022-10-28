from threading import Event, Thread
import pandas as pd
from queue import Queue
import time
import datetime
from collections import namedtuple


Result = namedtuple('Result', 'arg ans start end')
#FIB_INPUTS = [10, 11, 12, 13, 14, 15, 16, 17]
#FIB_INPUTS = [20, 21, 22, 23, 24, 25, 26, 27]
FIB_INPUTS = [30, 31, 32, 33]
#BATCH_SLEEP_SECONDS = 0.002
BATCH_SLEEP_SECONDS = None


def _now():
    return datetime.datetime.now().strftime("%Y/%m/%d %H:%M:%S.%f")


def _print(s):
    print(f'{_now()}: {s}')


def fib(n):
    if n < 2:
        return n
    return fib(n-1) + fib(n-2)


def backout_fib(fibval):
    # use trial-and-error to back solve fibonacci
    n = 0
    while fib(n) < fibval:
        n += 1
    return n


producer_task = fib
consumer_task = backout_fib


def run_task(task, arg):
    start_t = time.time()
    ret = task(arg)
    end_t = time.time()
    return Result(arg, ret, start_t, end_t)


def run_producer_task(arg):
    return run_task(producer_task, arg)


def run_consumer_task(arg):
    return run_task(consumer_task, arg)


# data producer
def produce_data(evt: Event, q: Queue, repeat_limit: int, results: list):
    repeated = 0
    while True:
        for n in FIB_INPUTS:
            ret = run_producer_task(n)
            q.put(ret.ans)
            results.append(ret)
        repeated += 1
        if repeated < repeat_limit:
            if BATCH_SLEEP_SECONDS:
                time.sleep(BATCH_SLEEP_SECONDS)
        else:
            break
    evt.set()


# data consumer
def consume_data(evt: Event, q: Queue, results: list):
    while True:
        try:
            x = q.get(timeout=1.5)
            results.append(run_consumer_task(x))
        except:
            _print('got a q.get() exception')
            if not evt.is_set():
                continue
            else:
                break



# 2022/10/26 22:43:17.903164: x=0, fib=0, cost=1.430511474609375e-06
# 2022/10/26 22:43:17.903217: x=1, fib=1, cost=9.5367431640625e-07
# 2022/10/26 22:43:17.903239: x=2, fib=1, cost=1.430511474609375e-06
# 2022/10/26 22:43:17.903261: x=3, fib=2, cost=2.1457672119140625e-06
# 2022/10/26 22:43:17.903290: x=4, fib=3, cost=5.0067901611328125e-06
# 2022/10/26 22:43:17.903319: x=5, fib=5, cost=2.86102294921875e-06
# 2022/10/26 22:43:17.903338: x=6, fib=8, cost=3.5762786865234375e-06
# 2022/10/26 22:43:17.903355: x=7, fib=13, cost=5.245208740234375e-06
# 2022/10/26 22:43:17.903376: x=8, fib=21, cost=8.344650268554688e-06
# 2022/10/26 22:43:17.903402: x=9, fib=34, cost=1.3113021850585938e-05
# 2022/10/26 22:43:17.903436: x=10, fib=55, cost=2.1219253540039062e-05
# 2022/10/26 22:43:17.903489: x=11, fib=89, cost=3.910064697265625e-05
# 2022/10/26 22:43:17.903566: x=12, fib=144, cost=5.984306335449219e-05
# 2022/10/26 22:43:17.903689: x=13, fib=233, cost=9.632110595703125e-05
# 2022/10/26 22:43:17.903923: x=14, fib=377, cost=0.0002079010009765625
# 2022/10/26 22:43:17.904208: x=15, fib=610, cost=0.0002560615539550781
# 2022/10/26 22:43:17.904623: x=16, fib=987, cost=0.0003917217254638672
# 2022/10/26 22:43:17.905245: x=17, fib=1597, cost=0.0006000995635986328
# 2022/10/26 22:43:17.906564: x=18, fib=2584, cost=0.0012929439544677734
# 2022/10/26 22:43:17.908217: x=19, fib=4181, cost=0.0015990734100341797
# 2022/10/26 22:43:17.910848: x=20, fib=6765, cost=0.002596139907836914
# 2022/10/26 22:43:17.915020: x=21, fib=10946, cost=0.004134654998779297
# 2022/10/26 22:43:17.921960: x=22, fib=17711, cost=0.006910085678100586
# 2022/10/26 22:43:17.931747: x=23, fib=28657, cost=0.009731054306030273
# 2022/10/26 22:43:17.945128: x=24, fib=46368, cost=0.01332855224609375
# 2022/10/26 22:43:17.966277: x=25, fib=75025, cost=0.021093368530273438
# 2022/10/26 22:43:17.999325: x=26, fib=121393, cost=0.03299975395202637
# 2022/10/26 22:43:18.054691: x=27, fib=196418, cost=0.05531620979309082
# 2022/10/26 22:43:18.139770: x=28, fib=317811, cost=0.08502864837646484
# 2022/10/26 22:43:18.280001: x=29, fib=514229, cost=0.14017987251281738

def _show_fib_costs():
    for x in range(40):
        start_t = time.time()
        ret = fib(x)
        end_t = time.time()
        _print(f'x={x}, fib={ret}, cost={end_t-start_t}')


# roughly speaking 190 micros to calculate fib(15)
# and roughly 560 micros to back solve it
def _show_task_avg(task):
    stats = []
    for _ in range(30):
        start_t = time.time()
        ret = task()
        end_t = time.time()
        stats.append(end_t-start_t)
    avg = sum(stats) / 30.0
    _print(f'stats: {stats}, avg={avg}')


def single_threaded_run(repeat_limit):
    start_t = time.time()
    repeated = 0
    producer_results = []
    consumer_results = []
    while True:
        for n in FIB_INPUTS:
            producer_ret = run_producer_task(n)
            consumer_ret = run_consumer_task(producer_ret.ans)
            producer_results.append(producer_ret)
            consumer_results.append(consumer_ret)

        repeated += 1
        if repeated < repeat_limit:
            if BATCH_SLEEP_SECONDS:
                time.sleep(BATCH_SLEEP_SECONDS)
        else:
            break
    end_t = time.time()
    return make_df(producer_results), make_df(consumer_results), end_t-start_t


def multi_threaded_run(repeat_limit):
    start_t = time.time()
    ctrl_evt = Event()

    producer_results = []
    consumer_results = []

    data_queue = Queue()

    producer_thread = Thread(
        target=produce_data,
        args=(ctrl_evt, data_queue, repeat_limit, producer_results)
    )
    consumer_thread = Thread(
        target=consume_data,
        args=(ctrl_evt, data_queue, consumer_results)
    )

    producer_thread.start()
    consumer_thread.start()

    producer_thread.join()
    consumer_thread.join()

    end_t = time.time()
    return make_df(producer_results), make_df(consumer_results), end_t-start_t

def make_df(results):
    data = {
        'arg': [each.arg for each in results],
        'ans': [each.ans for each in results],
        'start': [str(each.start) for each in results],
        'end': [str(each.end) for each in results],
        'cost': [each.end-each.start for each in results],
    }
    return pd.DataFrame(data=data)

if __name__ == '__main__':
    repeat_limit = 15
    spdf, scdf, scost_t = single_threaded_run(repeat_limit) # single threaded p/c dfs
    mpdf, mcdf, mcost_t = multi_threaded_run(repeat_limit) # multi threaded p/c dfs


    sum_spdf = spdf.groupby("arg").agg({'ans': 'first', 'cost': ['mean', 'std']})
    sum_scdf = scdf.groupby("ans").agg({'arg': 'first', 'cost': ['mean', 'std']})
    print(f'\nSingle threaded producer cost:\n {sum_spdf}')
    print(f'\nSingle threaded consumer cost:\n {sum_scdf}')
    # thru-put 
    single_cost = float(scdf.iloc[-1]['end']) - float(spdf.iloc[0]['start'])
    single_len = len(scdf)
    print(f'single thread cost: {single_cost}, len: {single_len}')
    print(f'avg: {single_cost/single_len}')

    sum_mpdf = mpdf.groupby("arg").agg({'ans': 'first', 'cost': ['mean', 'std']})
    sum_mcdf = mcdf.groupby("ans").agg({'arg': 'first', 'cost': ['mean', 'std']})
    print(f'\nMulti threaded producer cost:\n {sum_mpdf}')
    print(f'\nMulti threaded consumer cost:\n {sum_mcdf}')
    # thru-put 
    multi_cost = float(mcdf.iloc[-1]['end']) - float(mpdf.iloc[0]['start'])
    multi_len = len(mcdf)
    print(f'multi thread cost: {multi_cost}|{mcost_t}, len: {multi_len}')
    print(f'avg: {multi_cost/multi_len}|{mcost_t/multi_len}')
    print(f'threading start/stop overhead: {mcost_t - multi_cost}')
