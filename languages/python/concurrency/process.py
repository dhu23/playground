import multiprocessing as mp
from datetime import datetime
import time

def read_q(d, l, q, evt):
    d[1] = 1
    d[2] = 2
    count = 10
    #while count > 0:
    while not evt.is_set():
        count -= 1
        print('looping')
        try:
            x = q.get(timeout=2)
            print(f'got {x}')
            l.append(x)
        except:
            print('timeout')
    #while not evt.is_set():
    #    try:
    #        time.sleep(1)
    #        x = q.get(timeout=2)
    #        #d[x] = None
    #        #l1.append(x)
    #        #l2.append(str(x))
    #        print(f'{datetime.now()} got {x}, type={type(x)}')
    #    except:
    #        print(f'{datetime.now()} time-out')

def test_mp_queue():
    #q = mp.Queue()

    with mp.Manager() as m:
        #l1 = m.list()
        l = m.list()
        d = m.dict()
        evt = m.Event()
        q = mp.Queue() #m.Queue()

        p1 = mp.Process(target=read_q)
        p1 = mp.Process(target=read_q, args=(d, l, q, evt))
        p1.start()

        for x in range(10):
            print(f'{datetime.now()} putting {x}')
            q.put(x)

        time.sleep(3)
        #try:
        #    while True:
        #        time.sleep(2)
        #except KeyboardInterrupt:
        #    print(f'{datetime.now()} stopping...')
        #    #evt.set()
        #    #p1.join()
        #    #p1.terminate()
        evt.set()
        p1.join()
        print(d)
        print(l)


if __name__ == '__main__':
    test_mp_queue()
