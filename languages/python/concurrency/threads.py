import threading
import queue
import os
import sys
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


def fetch_snapshot2(evt, q, evt2): #evt2 == True means snapshot is needed
    while not evt.is_set():
        print(f'{datetime.now()} fetcher2: start snapshot evt block')
        while not evt2.wait(10): # looping while evt2 is not set
            print(f'{datetime.now()} fetcher2: snapshot evt timed-out')
            continue

        if evt.is_set():
            print(f'{datetime.now()} fetcher2: ending fetcher2')
            break

        # we are here because evt2 is set to true
        ss = get_snapshot_long()
        q.put(ss)
        evt2.clear()



def test_snapshot_with_event():
    stop_worker_thread_evt = threading.Event()

    stop_snapshot_thread_evt = threading.Event()
    flag_lock = threading.Lock()
    cond = threading.Condition(flag_lock)
    run_snapshot = [False] # just so I can modify in other threads

    snapshot_evt = threading.Event()

    q = queue.Queue()

    # -------------------------------------------------------
    work_thread = threading.Thread(
        target=process_job,
        args=(stop_worker_thread_evt, q, cond))

    #snapshot_thread = threading.Thread(
    #    target=fetch_snapshot,
    #    args=(stop_snapshot_thread_evt, q, cond, run_snapshot))

    snapshot_thread = threading.Thread(
        target=fetch_snapshot2,
        args=(stop_snapshot_thread_evt, q, snapshot_evt))

    work_thread.start()
    snapshot_thread.start()

    # --------------------------------------------------------
    count = 0 # every 5 count, put in a request for snapshot
    print(f'{datetime.now()} main: start...')
    while True:
        try:
            time.sleep(5)
            q.put(1)
            if count % 5 == 0:
                print(f'{datetime.now()} main: trying to notify snapshot thread...')
                #with cond:
                #    run_snapshot[0] = True
                #    cond.notify()
                snapshot_evt.set()
                print(f'{datetime.now()} main: snapshot notified')
            count += 1
        except KeyboardInterrupt:
            print(f'{datetime.now()} main: user terminating...')
            break

    # -------------------------------------------------------
    stop_snapshot_thread_evt.set()
    snapshot_evt.set()
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


# object-oriented implementation for the previous solution
class Worker(object):
    def __init__(self, data_queue):
        self._q = data_queue
        self._evt = threading.Event()
        #passing self works too for a unbound class method
        #self._t = threading.Thread(target=Worker.process, args=(self, 2))
        self._t = threading.Thread(target=self.process, args=(2,))

    def start(self):
        self._t.start()

    def process(self, pop_time_out):
        while not self._evt.is_set():
            try:
                print(f'{datetime.now()} worker.process: popping a queue')
                job = self._q.get(timeout=pop_time_out)
            except:
                print(f'{datetime.now()} worker.process: q timed-out. continue...')
                continue
            print(f'print job {job}')
            self._q.task_done()

    def stop(self):
        print(f'{datetime.now()} worker.stop: stopping...')
        self._evt.set()
        print(f'{datetime.now()} worker.stop: event set')
        self._t.join()
        print(f'{datetime.now()} worker.stop: thread joined')

    def add_job(self, job):
        self._q.put(job)


def test_object_thread():
    data_queue = queue.Queue()
    worker = Worker(data_queue)

    worker.start()
    count = 0
    while True:
        try:
            time.sleep(3)
            worker.add_job(count)
            count += 1
        except KeyboardInterrupt:
            break

    worker.stop()


class SnapshotController(object):
    def __init__(self, data_queue):
        self._stop_thread = threading.Event()
        self._flag_lock = threading.Lock()
        self._flag = False
        self._cond = threading.Condition()
        self._snapshot_failure_wait = threading.Event()

        self._q = data_queue
        self._thread = threading.Thread(target=self._fetch)

    def start(self):
        self._thread.start()

    def stop(self):
        self._snapshot_failure_wait.set()
        self._stop_thread.set()
        # use notfiy if wait_for is called without a timeout in _fetch
        with self._cond: 
            self._cond.notify()
        if self._thread.is_alive():
            self._thread.join()

    def request(self):
        with self._cond:
            if not self._flag:
                self._flag = True
                self._cond.notify()
                return True # indicates the request is acknowledged
            else:
                return False # indicates the request is ignored

    def _fetch0(self):
        while not self._stop_thread.is_set():
            # this basically keeps the lock forever if exceptions keep happening
            with self._cond: 
                if not self._cond.wait_for(lambda: self._flag, timeout=5):
                    print(f'{datetime.now()} ctrl._fetch0 wait_for timedout')
                    continue
                print(f'{datetime.now()} ctrl._fetch0 getting snapshot')

                while True:
                    try:
                        if True:
                            raise RuntimeError('test-error')
                        self._q.put(0)
                        self._flag = False
                    except:
                        print(f'{datetime.now()} ctrl._fetch0 exception')

                    print(f'{datetime.now()} ctrl._fetch0 waiting for 10 sec')
                    if self._snapshot_failure_wait.wait(timeout=10):
                        print(f'{datetime.now()} ctrl._fetch0 not more 10 sec wait')
                        break
                    else:
                        print(f'{datetime.now()} ctrl._fetch0 another attempt')

    def _fetch(self):
        while not self._stop_thread.is_set():
            print(f'{datetime.now()} starting fetch while loop')
            with self._cond:
                if not self._cond.wait_for(
                    #lambda: self._flag,
                    # using stop_thread.is_set() for more responsive program at stop
                    lambda: self._flag or self._stop_thread.is_set(),
                    #timeout=5
                ):
                    print(f'{datetime.now()} ctrl._fetch wait_for timedout ======')
                    continue
                print(f'{datetime.now()} ctrl._fetch getting snapshot')

                try:
                    #if True:
                    #    raise RuntimeError('test-error')
                    self._q.put(0)
                    self._flag = False
                    continue
                except:
                    print(f'{datetime.now()} ctrl._fetch exception')

            # once a while release the lock and block. Without this part, 
            # when a snapshot is need but not successfully retrieved, 
            # the containing while loop can go crazy!
            print(f'{datetime.now()} ctrl._fetch waiting for 15 sec')
            if self._snapshot_failure_wait.wait(timeout=15):
                print(f'{datetime.now()} ctrl._fetch not more 15 sec wait')
                break
            else:
                print(f'{datetime.now()} ctrl._fetch another attempt')


def test_snapshot_contention():
    data_queue = queue.Queue()
    worker = Worker(data_queue)
    ctrl = SnapshotController(data_queue)
    worker.start()
    ctrl.start()

    #ctrl.request()

    count = 0
    while True:
        try:
            time.sleep(2)
            print(f'{datetime.now()} qsize={data_queue.qsize()}')
            if count % 5 == 0:
                print(f'{datetime.now()} main requesting count={count} *****')
                ctrl.request() # request would get blocked forever if _fetch0 is used
            count += 1
        except:
            print(f'{datetime.now()} terminating')
            break
    print(f'count final {count}')
    ctrl.stop()
    worker.stop()


def slow_func():
    for _ in range(20):
        time.sleep(1)


def test_exit_before_join():
    t = threading.Thread(target=func)
    t.start()
    print('exiting...')
    sys.exit(1)


def func_with_except():
    time.sleep(5)
    raise RuntimeError('just-to-make-the-thread-fail')


def func_infinite(evt):
    while not evt.is_set():
        time.sleep(2)
        print('in func infinite')


def test_thread_exception():
    t = threading.Thread(target=func_with_except)
    evt = threading.Event()
    t2 = threading.Thread(target=func_infinite, args=(evt,))
    print(f'is t alive before start? {t.is_alive()}')
    print(f'is t2 alive before start? {t2.is_alive()}')
    t.start()
    t2.start()
    print(f'is t alive before exception? {t.is_alive()}')
    time.sleep(10)
    print(f'is t alive after exception? {t.is_alive()}')
    #t.join() # having it or not doesn't seem to matter. 
    #At least the program doesn't crash like in C++
    print(f'is t2 alive before evt set? {t2.is_alive()}')
    #evt.set()
    #print(f'is t2 alive after evt set? {t2.is_alive()}')
    # you don't have to join t2 and the program doesn't block
    print('joining t2')
    t2.join(3)
    print('t2 joined')
    print(f'is t2 still alive after a timed-out join? {t2.is_alive()}')


if __name__ == '__main__':
    #test_threaded_sum()
    #test_event()
    #test_snapshot_with_event()
    #test_object_thread()
    test_snapshot_contention()
    #test_exit_before_join()
    #test_thread_exception()
