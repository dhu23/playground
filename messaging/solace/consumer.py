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


def report_threads(fn):
    #print('-------------------- START ------------------')
    #print('reporting in {}'.format(fn))
    #print('# of threads: {}'.format(threading.active_count()))
    #print('current thread name: {}'.format(threading.current_thread().name))
    #print('-------------------- END --------------------')
    pass


def get_snapshot(data_queue):
    time.sleep(9)
    print('putting snapshot 5 in queue')
    data_queue.put({'a': 11, 'b': 9, 'c': 5})


class DataStore(object):
    def __init__(self):
        self._data = {}
        self._snapshot_received = False

    @property
    def snapshot_received(self):
        return self._snapshot_received

    def on_update(self, key, upd):
        print('on-update')
        if key not in self._data:
            self._data[key] = upd
            return True
        else:
            if self._data[key] < upd:
                self._data[key] = upd
                return True
            else:
                print(f'ignore upd:{upd}')
        return False

    def on_snapshot(self, snapshot):
        print('on-snapshot------')
        for k, u in snapshot.items():
            self.on_update(k, u)
        self._snapshot_received = True


class Consumer(MessageHandler):
    def __init__(self, data_queue):
        self._data_queue = data_queue

    def on_message(self, message: InboundMessage):
        #print(f'receive message: {message}')
        #print(f'receive bytes: {message.get_payload_as_bytes()}')
        #print(f'receive string: {message.get_payload_as_string()}')

        # report_threads('consumer-on-message')
        # during my test, htop shows that there are 4 threads running.
        # and this call-back runs on 'Thread-2'

        try:
            s = message.get_payload_as_string()
            print('payload as string:', s, type(s))
            pyobj = json.loads(s)
            # print('pyobj=', pyobj, type(pyobj), len(pyobj))
            # should have only one key
            assert len(pyobj) == 1
            key = list(pyobj.keys())[0]
            self._data_queue.put((key, pyobj[key]))
        except:
            print("not a good json obj:", message)


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

output = []
def process_data(data_queue, data_store):
    global output
    while True:
        print('popping.......')
        x = data_queue.get()
        print(f'poped {x}')
        print(x)
        if isinstance(x, dict):
            data_store.on_snapshot(x)
            to_output = True
        elif isinstance(x, tuple):
            k, u = x
            to_output = data_store.on_update(k, u)
        elif isinstance(x, int) and x == -1:
            break
        else:
            print('unknown data:%s, %s' % (x, type(x)))
            to_output = False
        data_queue.task_done()

        if data_store.snapshot_received:
            print('adding to the output =====>')
            output.append((x, to_output))


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

        data_store = DataStore()
        print(f'received snapshot? {data_store.snapshot_received}')
        process_t = threading.Thread(target=process_data, args=(data_queue, data_store))

        snapshot_t.start()
        process_t.start()

        snapshot_t.join()

        try:
            while True:
                time.sleep(0.1)
        except KeyboardInterrupt:
            print('\nDisconnecting messaging service')
    finally:
        report_threads('main-before direct_receiver.terminate()')
        print('\nTerminating receiver')
        direct_receiver.terminate()
        report_threads('main-before msg_service.disconnect()')
        print('\nDisconnecting Messaging Service')
        msg_service.disconnect()
        print('msg svc disconnected')
        report_threads('main-last line()')

        data_queue.join()
        print('data-queue joined')
        data_queue.put(-1)
        process_t.join()
        print(output)
