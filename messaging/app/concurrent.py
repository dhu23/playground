from threading import (
    Thread,
    Event,
    Condition,
    Lock,
)

from queue import Queue

__all__ = [
    'Worker',
    'ConcurrentFetcher',
]

class Worker(object):
    def __init__(self, msg_processor, publisher=None):
        self._processor = msg_processor
        self._publisher = publisher
        self._stop_event = Event()
        self._q = Queue()
        self._thread = Thread(target=self._process, name='WorkerThread')

    @property
    def data_queue(self):
        return self._q

    def start(self):
        self._thread.start()

    def stop(self):
        self._stop_event.set()
        if self._thread.is_alive():
            self._thread.join()

    def _process(self):
        while not self._stop_event.is_set():
            try:
                qmsg = self._q.get(timeout=2)
            except:
                continue
            ret = self._processor.on_message(qmsg)
            if self._publisher and ret:
                self._publisher.send(ret)
            self._q.task_done()

    def add_job(self, qmsg):
        if self._thread.is_alive():
            self._thread.put(qmsg)
            return True
        else:
            return False


class ConcurrentFetcher(object):
    def __init__(
        self, data_fetcher, data_queue: Queue,
        cond_timeout=5, failure_wait_timeout=15,
    ):
        self._fetcher = data_fetcher
        self._q = data_queue
        self._cond_timeout = cond_timeout
        self._failure_wait_timeout = failure_wait_timeout

        self._stop_event = Event()
        self._flag_lock = Lock()
        self._flag = False
        self._cond = Condition(self._flag_lock)
        self._failure_wait = Event()

    def start(self):
        self._thread = Thread(target=self._run, name='FetcherThread')
        self._thread.start()

    def stop(self):
        self._failure_wait.set()
        self._stop_event.set()
        self._thread.join()
        print('FetcherThread joined')

    def request(self):
        with self._cond:
            if self._flag:
                return False # already in the process of getting data
            else:
                print('requesting data fetching through cond notifying')
                self._flag = True
                self._cond.notify()
                return True

    def _run(self):
        self._fetcher.set_up()
        while not self._stop_event.is_set():
            with self._cond:
                if not self._cond.wait_for(
                    lambda: self._flag,
                    timeout=self._cond_timeout
                ):
                    print('timed-out in cnod.wait_for')
                    continue

                try:
                    self._q.put(self._fetcher())
                    self._flag = False
                    print('received snapshot. turning off request flag')
                    continue

                except Exception as e:
                    self._q.put(self._fetcher.make_error(str(e)))

            print(f'waiting {self._failure_wait_timeout} sec before another try')
            if self._failure_wait.wait(timeout=self._failure_wait_timeout):
                break
        print('finished fetch thread')
