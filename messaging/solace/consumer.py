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


class Consumer(MessageHandler):
    def on_message(self, message: InboundMessage):
        print(message)


class ConsumerReconnectionHandler(ReconnectionListener):
    def on_reconnected(self, e: ServiceEvent):
        print('\non_reconnected')
        print(e)


class ConsumerReconnectingHandler(ReconnectionAttemptListener):
    def on_reconnecting(self, e: ServiceEvent):
        print('\non_reconnecting')
        print(e)


class ConsumerServiceInterruptionHandler(ServiceInterruptionListener):
    def on_service_interrupted(self, e: ServiceEvent):
        print('\non_service_interruptted')
        print(e)


if __name__ == '__main__':
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

    msg_service.add_reconnection_listener(ConsumerReconnectionHandler())
    msg_service.add_reconnection_attempt_listener(ConsumerReconnectingHandler())
    msg_service.add_service_interruption_listener(ConsumerServiceInterruptionHandler())

    topic_sub = [TopicSubscription.of(t) for t in ['topic/try/me',]]

    direct_receiver = msg_service.create_direct_message_receiver_builder() \
        .with_subscriptions(topic_sub).build()

    direct_receiver.start()
    print(f'direct subscriber is running? {direct_receiver.is_running()}')

    try:
        direct_receiver.receive_async(Consumer())
        try:
            while True:
                time.sleep(1)
        except KeyboardInterrupt:
            print('\nDisconnecting messaging service')
    finally:
        print('\nTerminating receiver')
        direct_receiver.terminate()
        print('\nDisconnecting Messaging Service')
        msg_service.disconnect()
