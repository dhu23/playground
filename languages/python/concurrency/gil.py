import time
from threading import Thread

COUNT = 50000000

def countdown(n): # a CPU-bound task
    while n > 0:
        n -= 1


def run_one_thread():
    start = time.time()
    countdown(COUNT)
    end = time.time()

    print('Time taken in seconds -', end-start)

# context switching between two threads slows it down
# with GIL, it is essentially single-threaded
def run_two_thread(): 
    t1 = Thread(target=countdown, args=(COUNT//2,))
    t2 = Thread(target=countdown, args=(COUNT//2,))

    start = time.time()
    t1.start()
    t2.start()
    t1.join()
    t2.join()

    end = time.time()

    print('Time taken in seconds -', end-start)


if __name__ == '__main__':
    run_one_thread()
    run_two_thread()
    
