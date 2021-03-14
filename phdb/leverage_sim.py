# simulate leveraged ETF return
# a normal benchmark return in N days:
# I = (1+p1)(1+p2)...(1+pn), p1...pn 
# then leveraged ETF return, would be 
# J = (1+p1')(1+p2')...(1+pn'), where p2'/p1' = r*(p2/p1), etc

# general brownian motion
# St = S0*exp[ (mu-0.5*sigma^2) * t + sigma * Wt ]
# Wn(t) = sum [ z_i * sqrt (delta_t) ]

import numpy as np
import pandas as pd
from math import exp
import matplotlib.pyplot as plt


# assuming dt = 1.0 

def brownian_path(n):
    zs = np.random.normal(0, 1, n)
    zs[0] = 0.0
    return np.cumsum(zs)


def _calc_price(mu, sigma, ti, wt, s0=1.0):
    x = (mu - 0.5*(sigma**2)) * ti + sigma * wt
    return s0 * exp(x)


def simulate_once(mu, sigma, n, leverage):
    assert sigma > 0, 'sigma has to be greater than 0'

    bpath = brownian_path(n)
    df = pd.DataFrame({'w' : bpath, 't' : range(n)})
    
    def _f(row):
        return _calc_price(mu, sigma, row['t'], row['w'])

    df['s'] = df.apply(_f, axis=1)
    ratios = df['s'][1:].to_numpy() / df['s'][:-1].to_numpy()
    growths = ratios-1

    df['ratio'] = 1.0
    df.loc[1:, 'ratio'] = ratios

    df['growth'] = 0.0
    df.loc[1:, 'growth'] = growths

    df['growth_leveraged'] = leverage * df['growth']
    df['ratio_leveraged'] = 1+df['growth_leveraged']
    df['s_leveraged'] = np.cumprod(df['ratio_leveraged'])

    return df


def simulate(mu, sigma, n, leverage, sample_size):
    ret = []
    for i in range(sample_size):
        df = simulate_once(mu, sigma, n, leverage)
        payoff = df.iloc[-1]['s_leveraged'] / df.iloc[-1]['s']
        ret.append(payoff)
    
    lose_money_count = len(list(filter(lambda x : x < 1.0, ret)))
    make_money_count = len(list(filter(lambda x : x >= 1.0, ret)))

    print('total experiment count:%d' % sample_size)
    print('lose month=%d, make_money=%d' % (lose_money_count, make_money_count))

    return make_money_count / sample_size


if __name__ == '__main__':

    # long term investment in a leverage ETF loses money in a volatile market 

    leverage, experiment_size = 2, 256
    simulate(0.005, 0.01, 1024, leverage, experiment_size)
    simulate(0.005, 0.05, 1024, leverage, experiment_size)
    simulate(0.005, 0.07, 1024, leverage, experiment_size)
    simulate(0.005, 0.1, 1024, leverage, experiment_size)
    
