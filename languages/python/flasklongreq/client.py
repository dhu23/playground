import requests
import datetime
# import logging

URL = 'http://0.0.0.0:5010'

def make_url(*args):
    url = [URL]
    url.extend(args)
    return '/'.join(url)


def log(s):
    print('%s: %s' % (datetime.datetime.now(), s))


def get_resp(url):
    log('sending %s...' % url)
    resp = requests.get(url)
    if resp.status_code != 200:
        log('failed %s' % url)
    else:
        log('result=%s, url=%s' % (resp.json(), url))


def get_resp2(url):
    headers = {
        'cache-control': 'no-cache',
        'x-dreamfactory-api-key': "THISISADUMMYKEY"
    }
    log('sending2 %s...' % url)
    resp = requests.request("GET", url, headers=headers)
    log('result2=%s, url=%s' % (resp.json(), url))


def slow_fib(n):
    return make_url('slow', str(n))


def fast_fib(n):
    return make_url('fast', str(n))


if __name__ == '__main__':
    get_resp2(fast_fib(10))
    get_resp2(slow_fib(37))
    get_resp2(fast_fib(45))
