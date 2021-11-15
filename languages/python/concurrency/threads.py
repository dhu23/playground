import threading
import queue
import os
import time

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


if __name__ == '__main__':
    #test_threaded_sum()
    test_event()
