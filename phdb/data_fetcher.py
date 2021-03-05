import requests
import urllib
import datetime
import os
import itertools
import multiprocessing as mp

import util

def fetch_url_data(url):
    print('retrieving by requests from %s' % url)
    try:
        return requests.get(url)

    except Exception as e:
        print('failed to read from %s' % url)
        return None

def save_url_data2(url, target_file_name):
    print('retrieving by urllib from %s to %s' % (url, target_file_name))
    urllib.request.urlretrieve(url, target_file_name)


def fetch_wsj(args):
    ticker, root_dir, start_date, end_date = args
    # def fetch_wsj(ticker, root_dir, start_date, end_date):
    url = 'http://quotes.wsj.com/%s/historical-prices/download?MOD_VIEW=page&num_rows=6299.041666666667&range_days=6299.041666666667&startDate=%s&endDate=%s' % (ticker, util.to_mmddyyyy(start_date), util.to_mmddyyyy(end_date))
    

    start_dstr, end_dstr = util.to_yyyymmdd(start_date), util.to_yyyymmdd(end_date)
    r = fetch_url_data(url)
    if r is None:
        return 

    if not r.text[1:]:
        print('No data was fetched for %s between %s and %s' %
                (ticker, start_dstr, end_dstr))
        return

    file_name = os.path.join(root_dir, ticker+".csv")

    print('writing %s data to file %s)' % (ticker, file_name))
    with open(file_name, 'wb') as f:
        f.write(r.content)
    print('done writing %s data' % ticker)
    

if __name__ == "__main__":

    start = datetime.datetime.now()

    today = start.date()
    price_begin_date = datetime.date(2000, 1, 1)

    PHDB_STORE_ROOT = "/home/daowen/storage/phdb/sp500stocks"
    today_root_dir = os.path.join(PHDB_STORE_ROOT, util.to_yyyymmdd(today))
    util.mk_dir(today_root_dir)

    with open('snp500_constituents.csv') as f:
        symbols = []
        for line in f.readlines()[1:]:
            symbols.append(line.split(',')[0])

    #for s in symbols:
    #    fetch_wsj(s, today_root_dir, price_begin_date, today)

    jobs = [(s, today_root_dir, price_begin_date, today) for s in symbols]
    with mp.Pool(6) as p:
        p.map(fetch_wsj, jobs)

    print('elapsed time=', datetime.datetime.now()-start)
