package fantasy.impl.spacetime;

import fantasy.impl.event.Event;

import java.time.Duration;
import java.time.Instant;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class RealTimeWithSleeping extends AbstractWorldTime {
    private final ScheduledExecutorService scheduledExecutorService_;

    public RealTimeWithSleeping(SequenceNumber sequenceNumber) {
        super(sequenceNumber);
        scheduledExecutorService_ = Executors.newSingleThreadScheduledExecutor();
    }

    @Override
    protected void receiveEvent(Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event) {
        Instant now = Instant.now();
        Instant targetInstant = event.data().availableTime();
        if (now.isBefore(targetInstant)) {
            Duration wait = Duration.between(now, targetInstant);
            scheduledExecutorService_.schedule(() -> {
                pushEventToQueue(event);
            }, wait.toMillis(), TimeUnit.MILLISECONDS);
        } else {
            pushEventToQueue(event);
        }
    }

    @Override
    protected void onEvent_(Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event) {
        processEvent(event);
    }
}
