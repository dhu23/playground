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

    def my_assign(consumer, partitions):
        for part in partitions:
            print(f'received assignment: {part.topic}/{part.partition}/{part.offset}')
            if part.partition == 0:
                #part.offset = 2299
                pass
            elif part.partition == 1:
                part.offset = 2420
                #pass
            elif part.partition == 2:
                part.offset = 2380
                #pass
            else:
                pass
            print(f'adjusted assignment: {part.topic}/{part.partition}/{part.offset}')
        consumer.assign(partitions)


    c = Consumer(settings)
    c.subscribe(['topic-with-3-part'], on_assign=my_assign)
    # don't call it again with a second topic. The 2nd subscribe overwrites
    # the first specification. 

    try:
        while True:
            msg = c.poll(0.1) # 0.1 second. blocks until a msg is ready or timeout
            if msg is None:
                continue # timed-out
            elif not msg.error():
                #print(f'received message: {msg.value()}')
                data = json.loads(msg.value())
                data['ts'] = datetime.strptime(data['ts'], '%Y-%m-%d %H:%M:%S.%f')
                print(
                    f'recieved data => key: {msg.key()} offset: {msg.offset()} '
                    f'part: {msg.partition()} payload: omitted')
            elif msg.error().code() == KafkaError._PARTITION_EOF:
                print(f'End of partition: {msg.topic()}/{msg.partition()}')
            else:
                print(f'error occurred: {msg.error().str()}')

    except KeyboardInterrupt:
        pass


    finally:
        c.close()
