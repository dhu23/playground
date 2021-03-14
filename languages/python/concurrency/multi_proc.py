import multiprocessing as mp
import random
import string
import time

output = mp.Queue()

def rand_string(length, pos, output):
    print 'processing len=', length, 'pos=', pos
    rand_str = ''.join(
        random.choice(
            string.ascii_lowercase +
            string.ascii_uppercase +
            string.digits) for i in range(length))
    output.put((pos, rand_str))

processes = [mp.Process(
    target=rand_string,
    args=(5, x, output)) for x in range(4)]

for p in processes:
    p.start()

for i, p in enumerate(processes):
    print i
    p.join()


results = [output.get() for p in processes]

print results


def cube(x): return x**3

pool = mp.Pool(processes=4)
results = [pool.apply(cube, args=(x,)) for x in range(1, 7)]
print results

pool = mp.Pool(processes=4)
results = pool.map(cube, range(1, 7))
print results

pool = mp.Pool(processes=4)
results = [pool.apply_async(cube, args=(x,)) for x in range(1, 7)]
output = [p.get() for p in results]
print output
