import time
from pprint import pprint as pp
import json

from concurrent import (
    Worker,
    ConcurrentFetcher,
)

from order import (
    QMessage,
    MsgType,
    OrderState,
)

class OrderSnapshotFetcher(object):
    def __init__(self):
        pass

    def set_up(self):
        pass

    def make_error(self, msg):
        return QMessage(msgtype=MsgType.OrderSnapshotError, data=msg)

    def __call__(self):
        print('getting snapshot....')
        time.sleep(5) # simulate synchronous call delay
        print('fetched snapshot. returning...')
        return QMessage(
            msgtype=MsgType.OrderSnapshot,
            data=None
        )


if __name__ == '__main__':
    order_state = OrderState()

    worker = Worker(order_state)
    data_queue = worker.data_queue

    order_snapshot_fetcher = OrderSnapshotFetcher()
    order_snapshot_ctrl = ConcurrentFetcher(order_snapshot_fetcher, data_queue)

    try:
        worker.start()
        order_snapshot_ctrl.start()

        print(f'queue size:{data_queue.qsize()}')
        count = 0

        try:
            while True:
                time.sleep(3)
                if count % 5 == 0:
                    print(f'count {count}')
                    order_snapshot_ctrl.request()

                count += 1
                print(f'realtime: {order_state.is_realtime_received}')
                print(f'snapshot: {order_state.is_snapshot_received}')
                print(f'orders: {len(order_state.orders)}')
        except:
            print('\n disconnecting....')

    finally:
        print(f'final count={count}')
        order_snapshot_ctrl.stop()
        worker.stop()
