import json
from datetime import datetime
from confluent_kafka import Producer


def simple_produce(p):
    p.produce('first-topic', key=None, value='test message') # non-blocking


def produce_with_callback(p):

    def _acked(err, msg):
        if err is not None:
            print(f'failed to deliver message: {msg.value()}, {err.str()}')
        else:
            print(f'message produced: {msg.value()}')

    try:
        for val in range(1, 200):
            msg = {
                'value': f'myvalue {val}',
                'ts': datetime.now(),
            }
            p.produce(
                'first-topic',
                value=json.dumps(msg, default=str),
                on_delivery=_acked
            )
            p.poll(0.5) # time-out in seconds

    except KeyboardInterrupt:
        pass


if __name__ == '__main__':
    p = Producer({'bootstrap.servers': 'localhost:9092'})

    produce_with_callback(p)
    # blocks until all outstanding produce commands have completed
    # you typically call flush at application teardown
    p.flush(30)
