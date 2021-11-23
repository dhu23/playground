from collections import namedtuple
from enum import Enum

__all__ = [
    'QMessage',
    'MsgType',
    'OrderState',
]

QMessage = namedtuple('QMessage', ['msgtype', 'data'])

class MsgType(Enum):
    OrderUpdate = 1
    OrderSnapshot = 2
    OrderSnapshotError = 3
    OrderStateClear = 4


class OrderState(object):
    def __init__(self):
        self._store = {}
        self._realtime_received = False
        self._snapshot_received = False

    @property
    def is_realtime_received(self):
        return self._realtime_received

    @property
    def is_snapshot_received(self):
        return self._snapshot_received

    @property
    def orders(self):
        return self._store

    @staticmethod
    def get_key(order):
        return order['ClientOrderId']

    @staticmethod
    def get_ver(order):
        return order['Version']

    def _clear(self):
        self._prev_store = self.store

        self._store = {}
        self._realtime_received = False
        self._snapshot_received = False

    def _update_order(self, order):
        order_key = OrderState.get_key(order)
        if order_key not in self._store:
            self._store[order_key] = order
            return True
        else:
            existing = self._store[order_key]
            if OrderState.get_ver(existing) < OrderState.get_ver(order):
                self._store[order_key] = order
                return True
            else:
                return False

    def _on_order_update(self, order_update):
        self._update_order(order_udpate)
        if self.is_snapshot_received:
            return order_update
        else:
            return None

    def _on_order_snapshot(self, order_snapshot):
        if order_snapshot:
            print(f'received snapshot, len={len(order_snapshot)}')
            for _each_update in order_snapshot:
                self._update_order(_each_update)
            self._snapshot_received = True
            return self.orders
        else:
            return None

    def on_message(self, msg):
        mtype, data = msg
        if mtype == MsgType.OrderUpdate:
            return self._on_order_update(data)
        elif mtype == MsgType.OrderSnapshot:
            return self._on_order_snapshot(data)
        elif mtype == MsgType.OrderSnapshotError:
            pass
        elif mtype == MsgType.OrderStateClear:
            self._clear()
        else:
            print(f'unknown message type: {msg}')
