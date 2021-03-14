from collections import namedtuple
import operator

PQ = namedtuple("PQ", ["price", "quantity"])

def _match_order(match_side, residual_side, order, pred):
    pass

class Book(object):
    def __init__(self):
        self.bids = []
        self.asks = []

    def add_order(self, order, is_buy):
        if is_buy:
            return _match_order(self.asks, self.bids, order, operator.le)
        else:
            return _match_order(self.bids, self.asks, order, operator.ge)
        
