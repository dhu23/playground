import json
from datetime import datetime
from confluent_kafka import Producer
from confluent_kafka.admin import AdminClient, NewTopic
import sys

def simple_produce(p, topic):
    p.produce(topic, key=None, value='test message') # non-blocking


def produce_with_callback(p, topic):

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
                topic,
                value=json.dumps(msg, default=str),
                on_delivery=_acked
            )
            p.poll(0.5) # time-out in seconds

    except KeyboardInterrupt:
        pass


if __name__ == '__main__':
    config = {'bootstrap.servers': 'localhost:9092'}
    p = Producer(config)

    cluster_metadata = p.list_topics()
    if 'interesting-topic' not in cluster_metadata.topics:
        print('interesting-topic not exist, creating...')
        admin_client = AdminClient(config)
        ret = admin_client.create_topics([NewTopic('interesting-topic', 1, 1)])
        ret['interesting-topic'].result() # blocks until success

    cluster_metadata = p.list_topics()
    part_data = cluster_metadata.topics['interesting-topic'].partitions[0]
    print(f'id: {part_data.id}')
    print(f'leader: {part_data.leader}')
    print(f'replicas: {part_data.replicas}')
    print(f'isrs: {part_data.isrs}')
    print(f'error: {part_data.error}')

    produce_with_callback(p, 'strange-topic')
    # blocks until all outstanding produce commands have completed
    # you typically call flush at application teardown
    p.flush(30)
