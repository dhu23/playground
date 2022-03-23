import multiprocessing as mp
from datetime import datetime
import time

def test_mp_queue():
    q = mp.Queue()
    evt = mp.Event()

    def read_q():
        while not evt.is_set():
            try:
                x = q.get(timeout=2)
                print(f'{datetime.now()} got {x}')
            except:
                print(f'{datetime.now()} time-out')

    p1 = mp.Process(target=read_q)
    p1.start()

    for x in range(10):
        print(f'{datetime.now()} putting {x}')
        q.put(x)

    try:
        while True:
            time.sleep(2)
    except KeyboardInterrupt:
        print(f'{datetime.now()} stopping...')
        evt.set()
        p1.join()


if __name__ == '__main__':
    test_mp_queue()
