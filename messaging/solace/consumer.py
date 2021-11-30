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

snapshot_lock = threading.Lock()
cond = threading.Condition(snapshot_lock)
need_for_snapshot = False
keep_snapshot_thread = True

def report_threads(fn):
    print('-------------------- START ------------------')
    print('reporting in {}'.format(fn))
    print('# of threads: {}'.format(threading.active_count()))
    print('current thread name: {}'.format(threading.current_thread().name))
    #for i, t in enumerate(threading.enumerate()):
    #    print(f'thread {i} - {t.name}')
    print('-------------------- END --------------------')
    pass


def get_snapshot():
    time.sleep(9)
    return {'a': 11, 'b': 9, 'c': 5}


def fetch_snapshot(data_queue):
    global need_for_snapshot
    global keep_snapshot_thread

    def _need_snapshot():
        global need_for_snapshot
        return need_for_snapshot

    while True:
        with cond:
            cond.wait_for(_need_snapshot)
            print(f'need snapshot thread? {keep_snapshot_thread}')
            if not keep_snapshot_thread:
                break
            print('going for another loop...')
            ss = get_snapshot()
            data_queue.put(ss)
            need_for_snapshot = False


output = []
def process_data(data_queue, data_store):
    '''
    dict -> snapshot data
    tuple -> key/value pair update
    -1 -> terminate
    -2 -> get-snapshot
    '''
    global output
    global need_for_snapshot
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
        elif isinstance(x, int):
            if x == -1:
                data_queue.task_done()  # otherwise q can't join
                break
            elif x == -2: # get snapshots
                #print('need to get a snapshot, sleeping for a few seconds...')
                #get_snapshot(data_queue) # it'll be nice to move blocker out
                with cond:
                    need_for_snapshot = True
                    cond.notify()
        else:
            print('unknown data:%s, %s' % (x, type(x)))
            to_output = False
        data_queue.task_done()

        if data_store.snapshot_received:
            print('adding to the output =====>')
            output.append((x, to_output))


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
    def __init__(self, data_queue: Queue):
        super().__init__()
        self._data_queue = data_queue

    def on_message(self, message: InboundMessage):
        #print(f'receive message: {message}')
        #print(f'receive bytes: {message.get_payload_as_bytes()}')
        #print(f'receive string: {message.get_payload_as_string()}')

        report_threads('consumer-on-message')
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
    def __init__(self, data_queue: Queue):
        super().__init__()
        self.data_queue = data_queue

    def on_reconnected(self, e: ServiceEvent):
        report_threads('reconnection-handler-on-reconnected')
        print('\non_reconnected')
        self.data_queue.put(-2)
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


class SolaceSubscriber(object):
    def __init__(self, broker_props, data_queue):

        self.props = broker_props
        self.msg_svc = MessagingService.builder().from_properties(self.props) \
            .with_reconnection_retry_strategy(
                RetryStrategy.forever_retry(retry_interval=5000) # every 5 sec
            ).build()

        self.msg_svc.add_reconnection_listener(ConsumerReconnectionHandler(data_queue))
        self.msg_svc.add_reconnection_attempt_listener(ConsumerReconnectingHandler())
        self.msg_svc.add_service_interruption_listener(ConsumerServiceInterruptionHandler())

    def start(self, topics, data_handler):
        '''start receive async message for a given data handler'''
        self.msg_svc.connect()
        print('messaging service connected')
        _topic_sub = [TopicSubscription.of(t) for t in topics]
        self.direct_receiver = self.msg_svc.create_direct_message_receiver_builder() \
            .with_subscriptions(_topic_sub).build()
        self.direct_receiver.start()
        print(f'direct subscriber is running? {self.direct_receiver.is_running()}')
        self.direct_receiver.receive_async(data_handler)

    def stop(self):
        print('Terminating receiver...')
        self.direct_receiver.terminate()
        print('Disconnecting messaging service...')
        self.msg_svc.disconnect()
        print('Messaging service disconnected.')


if __name__ == '__main__':
    report_threads('main')
    broker_props = {
        'solace.messaging.transport.host': 'localhost:55555',
        'solace.messaging.service.vpn-name': 'default',
        'solace.messaging.authentication.scheme.basic.username': 'default',
        'solace.messaging.authentication.scheme.basic.password': ''
    }

    data_queue = Queue()
    sol_sub = SolaceSubscriber(broker_props, data_queue)

    try:
        sol_sub.start(['try-me'], Consumer(data_queue))
        report_threads('main-after direct_receiver.receive_async()')

        #snapshot_t = threading.Thread(target=get_snapshot, args=(data_queue,))
        snapshot_t = threading.Thread(target=fetch_snapshot, args=(data_queue,))

        data_store = DataStore()
        # print(f'received snapshot? {data_store.snapshot_received}')
        process_t = threading.Thread(target=process_data, args=(data_queue, data_store))

        snapshot_t.start()
        process_t.start()
        data_queue.put(-2)


        try:
            while True:
                time.sleep(0.1)
        except KeyboardInterrupt:
            print('\nDisconnecting messaging service')
    finally:
        sol_sub.stop()

        # final processing of the queue messaging and clean-up

        keep_snapshot_thread = False
        with cond:
            need_for_snapshot = True
            cond.notify()
        snapshot_t.join() # this thread can technically be merged with main

        print(output)
        print('process-t joined')

        print(data_queue.qsize())

        data_queue.put(-1)
        process_t.join()
        data_queue.join()
        print('data-queue joined')
