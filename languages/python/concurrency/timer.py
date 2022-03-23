from threading import Timer, current_thread
from datetime import datetime
from queue import Queue
import time

q = Queue()

def foo():
    print(f'{datetime.now()} running foo() in {current_thread().name} ')


class RepeatedTimer(object):
    def __init__(self, interval, delay, function, *args, **kwargs):
        self._timer = None
        self._interval = interval
        self._delay = delay
        self._function = function
        self._args = args
        self._kwargs = kwargs
        self._is_running = False
        #self.start()

    def _run(self):
        #self._is_running = False
        self._schedule_next()
        self._function(*self._args, **self._kwargs)

    def _schedule_next(self):
        print(f'{datetime.now()} scheduling the next')
        #if not self._is_running:
        self._timer = Timer(self._interval, self._run)
        self._timer.start()
        #self._is_running = True

    def start(self):
        if self._is_running:
            return
        print(f'{datetime.now()} starting')
        self._start_timer = Timer(self._delay, self._run)
        self._start_timer.start()
        self._is_running = True

    def stop(self):
        if self._timer is None:
            print(f'{datetime.now()} never scheduled')
        else:
            print(f'{datetime.now()} RT stopped')
            self._timer.cancel()
        #self._is_running = False
        if self._start_timer is not None:
            print(f'{datetime.now()} start timer live? {self._start_timer.is_alive()}')
            self._start_timer.cancel()
        else:
            print(f'{datetime.now()} never started')
        self._is_running = False

if __name__ == '__main__':
    #t = Timer(5.0, foo)
    #print(f'{datetime.now()} starting...')
    #t.start()
    #count = 5
    #while True:
    #    print(f'{datetime.now()} another while loop starts, count={count}')
    #    count -= 1
    #    if count == 0:
    #        break
    #    print(f'{datetime.now()} waiting for q...')
    #    x = q.get()
    #    print(f'{datetime.now()} got an item')
    #    t = Timer(5.0, foo)
    #    t.start()


    rt = RepeatedTimer(0.1, 2, foo)
    rt.start()

    try:
        while True:
            time.sleep(2)
    except KeyboardInterrupt:
        rt.stop()
    
