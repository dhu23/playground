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

    outbound_msg_builder = msg_service.message_builder() \
        .with_application_message_id('sample_id') \
        .with_property('application', 'samples') \
        .with_property('language', 'Python')

    def _create_msg(idx, key):
        outbound_msg = outbound_msg_builder \
            .with_application_message_id(f'New {idx}') \
            .build(f'{{"{key}":{idx}}}')
        print(f'outbound message {outbound_msg}')
        print(f'payload as bytes: {outbound_msg.get_payload_as_bytes()}')
        print(f'payload as string: {outbound_msg.get_payload_as_string()}')
        return outbound_msg

    def _publish_a(idx, topic):
        direct_publisher.publish(
            destination=topic, 
            message=_create_msg(idx, 'a')
        )

    def _publish_b(idx, topic):
        if idx % 3 == 0:
            direct_publisher.publish(
                destination=topic,
                message=_create_msg(idx, 'b')
            )

    def _publish_c(idx, topic):
        if idx % 5 == 0:
            direct_publisher.publish(
                destination=topic,
                message=_create_msg(idx, 'c')
            )

    total_count = 0
    try:
        idx = 0
        while idx <= 20: # send 5 in a batch, continuously
            topic = Topic.of('try-me')

            _publish_a(idx, topic)
            _publish_b(idx, topic)
            _publish_c(idx, topic)

            idx += 1
            total_count += 3
            time.sleep(1)

    except KeyboardInterrupt:
        print('\nTerminating publisher')
        direct_publisher.terminate()
        print('\ndisconnecting messaging service')
        msg_service.disconnect()

    print(f'total-count:{total_count}')
