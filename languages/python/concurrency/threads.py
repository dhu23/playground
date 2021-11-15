import threading
import queue
import os
import time
from datetime import datetime

def task1():
    print('Task 1 assigned to thread: {}'.format(
        threading.current_thread().name
    ))
    time.sleep(10)
    print('ID of process running task 1: {}'.format(os.getpid()))


def task2():
    print('Task 2 assigned to thread: {}'.format(
        threading.current_thread().name
    ))
    time.sleep(20)
    print('ID of process running task 2: {}'.format(os.getpid()))


def report():
    print('# of threads: {}'.format(threading.active_count()))


def test_thread_name():
    print('ID of process running main program: {}'.format(os.getpid()))

    print('main thread name: {}'.format(threading.current_thread().name))

    t1 = threading.Thread(target=task1) # name='t1')
    t2 = threading.Thread(target=task2) # name='t2')

    report()
    time.sleep(10) # htop at this point would show only the MainThread

    t1.start()
    t2.start()

    report()
    # since task1 sleeps 10 seconds less than task2, htop would show the
    # main thread and thread 2 for about 10 seconds
    t1.join()
    report()

    t2.join()
    report()

    time.sleep(10) # shows only the main thread


x = 0
xlock = threading.Lock()
def task3():
    global x
    for i in range(1000000):
        if i % 10000 == 0:
            # even with this 50ms break,
            # it is very hard to replicate the case where numbers get messed up
            time.sleep(0.01) 
            # pass
        #xlock.acquire()
        x += 1
        #xlock.release()


def test_threaded_sum():
    t1 = threading.Thread(target=task3)
    t2 = threading.Thread(target=task3)

    t1.start()
    t2.start()
    t1.join()
    t2.join()

    global x
    print('x={}'.format(x))


def do_work(evt, q):
    while not evt.is_set():
        try:
            print('trying to get an item from the queue')
            job = q.get(timeout=5)
        except:
            print('get exception. continue...')
            continue
        if job == 1:
            print('processing job == 1')
            q.task_done()
        elif job == 2:
            print('processing job == 2')
            q.task_done()
        else:
            print(f'bad job {job}')
            q.task_done()


def test_event():
    evt = threading.Event()
    que = queue.Queue()
    work_thread = threading.Thread(target=do_work, args=(evt, que))
    work_thread.start()

    while True:
        try:
            time.sleep(2)
            que.put(1)
        except KeyboardInterrupt:
            print('user terminating...')
            break

    evt.set()
    work_thread.join()
    que.join()


def process_job(evt, q, cond):
    while not evt.is_set():
        try:
            print(f'{datetime.now()} worker: getting a job from the queue')
            job = q.get(timeout=2)
        except:
            print(f'{datetime.now()} worker: q.get timed-out. continue...')
            continue

        if job == 1:
            print(f'{datetime.now()} worker: processing job == 1')
            q.task_done()
        elif job == 2: # job 2 is snapshot
            print(f'{datetime.now()} worker: processing job == 2')
            q.task_done()
        else:
            print(f'{datetime.now()} worker: bad job {job}')
            q.task_done()


def get_snapshot_long():
    print(f'{datetime.now()} get_snapshot: start...')
    time.sleep(10)
    print(f'{datetime.now()} get_snapshot: returning snapshot')
    return 2


def fetch_snapshot(evt, q, cond, flag):
    while not evt.is_set():
        with cond:
            print(f'{datetime.now()} fetcher: waiting for cond')
            ret = cond.wait_for(lambda : flag[0], timeout=5)
            if not ret: # timed-out
                print(f'{datetime.now()} fetcher: wait_for timed-out. continue...')
                continue

            ss = get_snapshot_long()
            q.put(ss)
            flag[0] = False


def test_snapshot_with_event():
    stop_worker_thread_evt = threading.Event()

    stop_snapshot_thread_evt = threading.Event()
    flag_lock = threading.Lock()
    cond = threading.Condition(flag_lock)
    run_snapshot = [False] # just so I can modify in other threads

    q = queue.Queue()

    # -------------------------------------------------------
    work_thread = threading.Thread(
        target=process_job,
        args=(stop_worker_thread_evt, q, cond))

    snapshot_thread = threading.Thread(
        target=fetch_snapshot,
        args=(stop_snapshot_thread_evt, q, cond, run_snapshot))

    work_thread.start()
    snapshot_thread.start()

    # --------------------------------------------------------
    count = 0 # every 5 count, put in a request for snapshot
    print(f'{datetime.now()} main: start...')
    while True:
        try:
            time.sleep(5)
            q.put(1)
            count += 1
            if count % 5 == 0:
                print(f'{datetime.now()} main: trying to notify snapshot thread...')
                with cond:
                    run_snapshot[0] = True
                    cond.notify()
                print(f'{datetime.now()} main: snapshot notified')
        except KeyboardInterrupt:
            print(f'{datetime.now()} main: user terminating...')
            break

    # -------------------------------------------------------
    stop_snapshot_thread_evt.set()
    print(f'{datetime.now()} main: marked events set')

    snapshot_thread.join()
    print(f'{datetime.now()} main: snapshot thread joined')

    stop_worker_thread_evt.set()
    work_thread.join()
    print(f'{datetime.now()} main: work thread joined')

    print(f'{datetime.now()} main: q size={q.qsize()}')
    #q.join() I don't think we can join the q here.
    #because the stop of threads are controlled by event variables
    #instead of sentinel job in the queue. You just can't guarantee that
    #the queue size is 0 at this point
    print(f'{datetime.now()} main: q joined. DONE')


if __name__ == '__main__':
    #test_threaded_sum()
    #test_event()
    test_snapshot_with_event()
