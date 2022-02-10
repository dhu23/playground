import json
from datetime import datetime
from confluent_kafka import (
    Consumer,
    KafkaError,
)

if __name__ == '__main__':
    settings = {
        'bootstrap.servers': 'localhost:9092',
        'group.id': 'my-first-application', # consumer group id
        'client.id': 'client-1', # for easier logging and identification
        'enable.auto.commit': True, 
        # for more cnotrol, set enable.auto.commit to False and
        # call commit() function in the code. 
        'session.timeout.ms': 6000,
        'default.topic.config': {'auto.offset.reset': 'smallest'},
    }

    c = Consumer(settings)
    c.subscribe(['strange-topic'])
    # don't call it again with a second topic. The 2nd subscribe overwrites
    # the first specification. 

    try:
        while True:
            msg = c.poll(0.1) # 0.1 second. blocks until a msg is ready or timeout
            if msg is None:
                continue # timed-out
            elif not msg.error():
                print(f'received message: {msg.value()}')
                data = json.loads(msg.value())
                data['ts'] = datetime.strptime(data['ts'], '%Y-%m-%d %H:%M:%S.%f')
                print(f'recieved message in python format: {data}')
            elif msg.error().code() == KafkaError._PARTITION_EOF:
                pritn(f'End of partition: {msg.topic()}/{msg.partition()}')
            else:
                print(f'error occurred: {msg.error().str()}')

    except KeyboardInterrupt:
        pass


    finally:
        c.close()
