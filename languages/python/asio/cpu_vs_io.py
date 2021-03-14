# run it in:
# pipenv shell


import time
import functools
from urllib.request import urlopen
from threading import Thread
from multiprocessing import Process

import asyncio
import aiohttp

#import gevent.monkey
#gevent.monkey.patch_all()


class timeit(object):
    def __call__(self, f):
        @functools.wraps(f)
        def decorated(*args, **kwargs):
            with self:
                return f(*args, **kwargs)
        return decorated

    def __enter__(self):
        self.start_time = time.time()

    def __exit__(self, *args, **kwargs):
        elapsed = time.time() - self.start_time
        print('{:.3} sec'.format(elapsed))


def cpu_bound(a, b):
    return a**b


def io_bound(urls):
    data = []
    for url in urls:
        data.append(urlopen(url).read())
    return data


@timeit()
def looped(n, func, *args):
    for i in range(n):
        func(*args)

@timeit()
def threaded(n_threads, func, *args):
    jobs = []
    for i in range(n_threads):
        thread = Thread(target=func, args=args)
        jobs.append(thread)

    for j in jobs:
        j.start()

    for j in jobs:
        j.join()


@timeit()
def multiprocessed(n_threads, func, *args):
    processes = []
    for i in range(n_threads):
        p = Process(target=func, args=args)
        processes.append(p)

    for p in processes:
        p.start()

    for p in processes:
        p.join()


@timeit()
def green_threaded(n_threads, func, *args):
    jobs = []
    for i in range(n_threads):
        jobs.append(gevent.spawn(func, *args))

    gevent.wait(jobs)



# we cannot mix synchronous and asynchronous operations
# we must make all of them asynchronous. Naturally, not for everything, 
# there are asynchronous libraries. 

# some of the code remains blocking, and it must somehow be run so that it
# does not block our event loop. For this, there is a good run_in_executor()
# method, whic runs what we passed to it in a one of the threads of the 
# built-in pool without blocking the main thread with the event loop. 

# We use this functionality for our CPU-bound function. We will rewrite the
# I/O-bound function completely to await those moments where we are waiting 
# for an event. 



async def async_func(N, func, *args):
    coros = [func(*args) for _ in range(N)]
    # run awaitable objects concurrently
    await asyncio.gather(*coros)


async def a_cpu_bound(a, b):
    result = await loop.run_in_executor(None, cpu_bound, a, b)
    return result


async def a_io_bound(urls):
    # create a coroutine function where we will download from individual url
    async def download_coroutine(session, url):
        async with session.get(url, timeout=10) as response:
            await response.text()

    # set an aiohttp session and download all our urls
    async with aiohttp.ClientSession(loop=loop) as session:
        for url in urls:
            await download_coroutine(session, url)



if __name__ == '__main__':
    a, b = 7777, 200000
    urls = [
        'http://google.com',
        'http://yahoo.com',
        'http://linkedin.com',
        'http://facebook.com',
    ]
    
    print('----- looped ------')
    looped(10, cpu_bound, a, b)
    looped(10, io_bound, urls)

    print('----- multithreaded ------')
    threaded(10, cpu_bound, a, b)
    threaded(10, io_bound, urls)

    print('----- multiprocessing ------')
    multiprocessed(10, cpu_bound, a, b)
    multiprocessed(10, io_bound, urls)

    # print('----- greenthreaded -----')
    #green_threaded(10, cpu_bound, a, b)
    #green_threaded(10, io_bound, urls)


    print('----- asyncio ------')
    loop = asyncio.get_event_loop()
    with timeit():
        loop.run_until_complete(async_func(10, a_cpu_bound, a, b))

    with timeit():
        loop.run_until_complete(async_func(10, a_io_bound, urls))


