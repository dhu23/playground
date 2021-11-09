import threading
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
    for _ in range(1000000):
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


if __name__ == '__main__':
    test_threaded_sum()
