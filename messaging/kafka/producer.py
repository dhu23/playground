import json
from datetime import datetime
from confluent_kafka import Producer, KafkaException
from confluent_kafka.admin import AdminClient, NewTopic
import sys

def simple_produce(p, topic):
    p.produce(topic, key=None, value='test message') # non-blocking


def produce_with_callback(p, topic):

    def _acked(err, msg):
        if err is not None:
            print(f'failed to deliver message: {msg.value()}, {err.str()}')
        else:
            #print(f'message produced: {msg.value()}')
            pass

    try:
        print(f'starting loop {datetime.now()}')
        count = 0
        for val in range(1, 1000):
            msg = {
                'value': f'myvalue {val}',
                'ts': datetime.now(),
                'field1': f'field1val-{val}',
                'field2': f'field2val-{val}',
                'nested1': {
                    'n1-field1': f'n1-field1val-{val}',
                    'n1-field2': f'n1-field2val-{val}',
                    'n1-field3': f'n1-field3val-{val}',
                    'n1-field4': f'n1-field4val-{val}',
                },
                'nested2': {
                    'n2-field1': f'n2-field1val-{val}',
                    'n2-field2': f'n2-field2val-{val}',
                    'n2-field3': f'n2-field3val-{val}',
                    'n2-field4': f'n2-field4val-{val}',
                },
            }
            #print(f'{datetime.now()} before produce()')
            try:
                p.produce(
                    topic,
                    value=json.dumps(msg, default=str),
                    key=str(val),
                    partition=val,
                    on_delivery=_acked,
                )
            except KafkaException as ke:
                print(f'kafka error: {str(ke)}')
            #print(f'{datetime.now()} after produce(), before poll()')
            p.poll(0.5) # time-out in seconds
            #print(f'{datetime.now()} after poll()')
            count += 1
            if count % 100 == 0:
                print(f'just finished {count}, {datetime.now()}')
        print(f'closing loop {datetime.now()}')

    except KeyboardInterrupt:
        pass



if __name__ == '__main__':
    config = {'bootstrap.servers': 'localhost:9092'}
    p = Producer(config)

    topic = 'topic-with-3-part'

    cluster_metadata = p.list_topics()
    if topic not in cluster_metadata.topics:
        print('interesting-topic not exist, creating...')
        admin_client = AdminClient(config)
        ret = admin_client.create_topics([NewTopic(topic, 3, 1)])
        ret[topic].result() # blocks until success

    cluster_metadata = p.list_topics()
    part_data = cluster_metadata.topics[topic].partitions[0]
    print(f'id: {part_data.id}')
    print(f'leader: {part_data.leader}')
    print(f'replicas: {part_data.replicas}')
    print(f'isrs: {part_data.isrs}')
    print(f'error: {part_data.error}')

    produce_with_callback(p, topic)
    # blocks until all outstanding produce commands have completed
    # you typically call flush at application teardown
    p.flush(30)
