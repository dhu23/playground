import pandas as pd
import numpy as np
import datetime
import os
import util
import operator
import matplotlib.pyplot as plt


# ---------------- user space functions ---------------------
def standarize(hd):
    # make the data frame indexed by date
    hd['ts'] = pd.to_datetime(hd['Date'])
    return hd


def load_csv(location, ticker_type, ticker, date):
    file_name = os.path.join(
            location, ticker_type, util.to_yyyymmdd(date), ticker+'.csv')
    return pd.read_csv(file_name)


def load_mutualfund(ticker):
    return load_csv('/home/daowen/storage/phdb', 'mutualfund',
                    ticker, datetime.date(2020, 1, 8))


def load_etf(ticker):
    return load_csv('/home/daowen/storage/phdb', 'etf',
                    ticker, datetime.date(2020, 1, 8))


def load_hist(ticker):
    loaders = {
        'FOCPX': load_mutualfund,
        'SPY': load_etf,
        }
    return standarize(loaders[ticker](ticker))


# ---------------- analysis functions ---------------------
def show_price(hd, px_name):
    plt.figure(figsize=(16, 12))
    plt.plot(pd.to_datetime(hd['Date'], infer_datetime_format=True),
             hd[px_name])


def harmonic_mean(prices):  # for calculating avg for fixed fund investment
    return len(prices)/sum(1/prices)


def avg_cost(prices, funds):
    if isinstance(funds, (int, float)):
        funds = np.array([funds]*len(prices))

    shares = funds/prices
    max_row_count = pd.get_option('display.max_rows')
    info_df = pd.DataFrame(
            data={'price': prices, 'fund': funds, 'shares': shares})
    pd.set_option('display.max_rows', info_df.shape[0]+1)
    # print(info_df)
    pd.set_option('display.max_rows', max_row_count)
    return sum(funds)/sum(shares)


def avg_cost_exact(hd, alloc_spec):  # alloc_spec is a list of dates and funds
    dates = map(operator.itemgetter(0), alloc_spec)
    funds = map(operator.itemgetter(1), alloc_spec)
    selected = hd.loc[pd.to_datetime(hd['Date']).isin(dates)]

    return avg_cost(selected['Open'], funds)


def avg_cost_asof(hd, alloc_dates, alloc_funds):
    prices = util.sample_asof(
            pd.to_datetime(hd['Date'], infer_datetime_format=True),
            alloc_dates,
            hd['Close'])[0]
    return avg_cost(np.array(prices), alloc_funds)


def schedule_MOD(sdate, edate, invest):
    ret = pd.DataFrame(index=pd.date_range(sdate, edate, freq='1M'))
    ret['invest'] = invest
    return ret


# use asof date investment for a given sampling
def avg_cost1(hd, get_date_func, fix_fund, sdate, edate):
    return avg_cost_asof(hd, get_date_func(sdate, edate), fix_fund)


# ----------- data frame strategy analysis functions ---------------
def dollar_cost_averaging(std_hd, invest_schedule):
    assert 'invest' in invest_schedule
    strat = pd.merge_asof(
            invest_schedule, std_hd, left_index=True, right_on='ts')
    strat['shares'] = strat['invest'] / strat['Close']
    strat['cumshares'] = np.cumsum(strat['shares'])
    strat['cuminvest'] = np.cumsum(strat['invest'])

    strat['avg_cost'] = strat['cuminvest'] / strat['cumshares']

    return strat
