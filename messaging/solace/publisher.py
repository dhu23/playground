import os
import time

from solace.messaging.messaging_service import (
    MessagingService,
    ReconnectionListener,
    ReconnectionAttemptListener,
    ServiceInterruptionListener,
    RetryStrategy,
    ServiceEvent,
)

from solace.messaging.resources.topic import (
    Topic,
)

from solace.messaging.publisher.direct_message_publisher import (
    PublishFailureListener,
)

class ServiceEventHandler(
    ReconnectionListener,
    ReconnectionAttemptListener,
    ServiceInterruptionListener,
):
    def on_reconnected(self, e : ServiceEvent):
        print('\non_reconnected')
        print(e)

    def on_reconnecting(self, e : ServiceEvent):
        print('\non_reconnecting')
        print(e)

    def on_service_interrupted(self, e : ServiceEvent):
        print('\non_service_interrupted')
        print(e)


class PublishErrorHandling(PublishFailureListener):
    def on_failed_publish(self, e : "FailedPublishEvent"):
        print('\non-failed-publish')


if __name__ == '__main__':
    broker_props = {
        'solace.messaging.transport.host' : 'localhost:55555',
        'solace.messaging.service.vpn-name': 'default',
        'solace.messaging.authentication.scheme.basic.username': 'default',
        'solace.messaging.authentication.scheme.basic.password': ''
    }

    msg_service = MessagingService.builder().from_properties(broker_props) \
        .with_reconnection_retry_strategy(RetryStrategy.parametrized_retry(20, 3)) \
        .build()

    msg_service.connect()
    print(f'message_service connected? {msg_service.is_connected}')

    service_handler = ServiceEventHandler()
    msg_service.add_reconnection_listener(service_handler)
    msg_service.add_reconnection_attempt_listener(service_handler)
    msg_service.add_service_interruption_listener(service_handler)

    direct_publisher = msg_service.create_direct_message_publisher_builder().build()
    direct_publisher.set_publish_failure_listener(PublishErrorHandling())

    direct_publisher.start()
    print(f'direct publisher ready? {direct_publisher.is_ready()}')

    msg_body = 'This is the body of the msg'
    outbound_msg_builder = msg_service.message_builder() \
        .with_application_message_id('sample_id') \
        .with_property('application', 'samples') \
        .with_property('language', 'Python')

    total_count = 0
    try:
        count = 1
        while True:
            while count <= 50: # send 5 in a batch, continuously
                topic = Topic.of('try-me')
                outbound_msg = outbound_msg_builder \
                    .with_application_message_id(f'NEW {count}') \
                    .build(f'{msg_body} + {total_count}')

                direct_publisher.publish(destination=topic, message=outbound_msg)

                # print(f'published message on {topic}')
                count += 1
                total_count += 1

                if total_count % 500 == 0:
                    print(f'sent {total_count} messages')

                #time.sleep(0.01)

            #print('\n a new batch')
            count = 1
            time.sleep(0.1)

    except KeyboardInterrupt:
        print('\nTerminating publisher')
        direct_publisher.terminate()
        print('\ndisconnecting messaging service')
        msg_service.disconnect()

    print(f'total-count:{total_count}')
