from solace.messaging.messaging_service import (
    MessagingService,
    ReconnectionListener,
    ReconnectionAttemptListener,
    ServiceInterruptionListener,
    RetryStrategy, 
    ServiceEvent,
)

from solace.messaging.resources.topic_subscription import (
    TopicSubscription,
)

from solace.messaging.receiver.message_receiver import (
    MessageHandler, 
    InboundMessage,
)

import time
import json
import threading
from queue import Queue

x = 0
main_thread_counter = 0
callback_counter = 0

xlock = threading.Lock()


def report_globals():
    print('x={}'.format(x))
    print('main-thread-counter:{}'.format(main_thread_counter))
    print('callback-counter:{}'.format(callback_counter))
    sum_counters = main_thread_counter + callback_counter
    print(f'sum of counters:{sum_counters}')
    print(f'good? {x == sum_counters}') 
    print(f'small x? {x < sum_counters}, diff:{sum_counters-x}')


def report_threads(fn):
    print('-------------------- START ------------------')
    print('reporting in {}'.format(fn))
    print('# of threads: {}'.format(threading.active_count()))
    print('current thread name: {}'.format(threading.current_thread().name))
    print('-------------------- END --------------------')


def get_snapshot(data_queue):
    time.sleep(15)
    print('putting snapshot 5 in queue')
    data_queue.put(5)


class Consumer(MessageHandler):
    def __init__(self, data_queue):
        self._data_queue = data_queue

    def on_message(self, message: InboundMessage):
        payload_bytes = message.get_payload_as_bytes()
        print(f'receive {payload_bytes}')

        # report_threads('consumer-on-message')

        # during my test, htop shows that there are 4 threads running.
        # and this call-back runs on 'Thread-2'
        global x, xlock
        #xlock.acquire()
        x += 1
        #xlock.release()

        global callback_counter
        callback_counter += 1
        try:
            pyobj = json.loads(payload_bytes)
            #print(pyobj)
            #print(type(pyobj))
        except:
            #print("not a good json obj:", payload_bytes)
            pass

        # ignore the data that's sent in. Instead use the the counter
        print(f'putting {callback_counter} in queue')
        data_queue.put(callback_counter-1)


class ConsumerReconnectionHandler(ReconnectionListener):
    def on_reconnected(self, e: ServiceEvent):
        report_threads('reconnection-handler-on-reconnected')
        print('\non_reconnected')
        print(e)


class ConsumerReconnectingHandler(ReconnectionAttemptListener):
    def on_reconnecting(self, e: ServiceEvent):
        report_threads('reconnecting-handler-on-reconnecting')
        print('\non_reconnecting')
        print(e)


class ConsumerServiceInterruptionHandler(ServiceInterruptionListener):
    def on_service_interrupted(self, e: ServiceEvent):
        report_threads('service-interruption-on-service-interrupted')
        print('\non_service_interruptted')
        print(e)


def process_data(data_queue):
    while True:
        x = data_queue.get()
        if x < 0:
            break
        print(x)
        data_queue.task_done()


if __name__ == '__main__':
    report_threads('main')
    broker_props = {
        'solace.messaging.transport.host': 'localhost:55555',
        'solace.messaging.service.vpn-name': 'default',
        'solace.messaging.authentication.scheme.basic.username': 'default',
        'solace.messaging.authentication.scheme.basic.password': ''
    }

    msg_service = MessagingService.builder().from_properties(broker_props) \
        .with_reconnection_retry_strategy(RetryStrategy.parametrized_retry(20, 3)) \
        .build()

    msg_service.connect()
    print('message_service connected')

    report_threads('main-after msg_service.connect()')

    msg_service.add_reconnection_listener(ConsumerReconnectionHandler())
    msg_service.add_reconnection_attempt_listener(ConsumerReconnectingHandler())
    msg_service.add_service_interruption_listener(ConsumerServiceInterruptionHandler())

    report_threads('main-after adding listeners')

    topic_sub = [TopicSubscription.of(t) for t in ['try-me',]]

    direct_receiver = msg_service.create_direct_message_receiver_builder() \
        .with_subscriptions(topic_sub).build()

    direct_receiver.start()
    print(f'direct subscriber is running? {direct_receiver.is_running()}')
    report_threads('main-after direct_receiver.start()')

    try:

        data_queue = Queue()
        direct_receiver.receive_async(Consumer(data_queue)) # just register the callback
        report_threads('main-after direct_receiver.receive_async()')

        snapshot_t = threading.Thread(target=get_snapshot, args=(data_queue,))
        process_t = threading.Thread(target=process_data, args=(data_queue,))

        snapshot_t.start()
        snapshot_t.join()


        try:
            while True:
                time.sleep(0.1)
                counter = 0
                if main_thread_counter >= 8000000: 
                    continue
                while counter < 20000: 
                    # some logic to simulate another thread accessing
                    # x from a different thread, without any locking
                    #xlock.acquire()
                    x += 1
                    #xlock.release()
                    counter += 1
                    main_thread_counter += 1
        except KeyboardInterrupt:
            print('\nDisconnecting messaging service')
    finally:
        report_threads('main-before direct_receiver.terminate()')
        print('\nTerminating receiver')
        direct_receiver.terminate()
        report_threads('main-before msg_service.disconnect()')
        print('\nDisconnecting Messaging Service')
        msg_service.disconnect()

        report_globals() # to verify unprotected shared data access

        report_threads('main-last line()')
