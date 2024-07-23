package fantasy.impl.spacetime;

import fantasy.impl.event.Event;

import javax.annotation.Nullable;
import java.time.Duration;
import java.time.Instant;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class RealTimingWithSleeping extends AbstractRealTiming {
    private final ScheduledExecutorService scheduledExecutorService_;

    public RealTimingWithSleeping(SequenceNumber sequenceNumber, @Nullable Instant endTime) {
        super(new RealWorldWallClock(), endTime, sequenceNumber);
        scheduledExecutorService_ = Executors.newSingleThreadScheduledExecutor();
        this.eventQueue_.start();
    }

    @Override
    protected void receiveEvent(Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event) {
        Instant now = now();
        Instant targetInstant = event.data().availableTime();
        if (now.isBefore(targetInstant)) {
            Duration wait = Duration.between(now, targetInstant);
            scheduledExecutorService_.schedule(() -> {
                pushEventToBlockingQueue(event);
            }, wait.toMillis(), TimeUnit.MILLISECONDS);
        } else {
            pushEventToBlockingQueue(event);
        }
    }

    @Override
    protected void onEvent_(Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event) {
        processEvent(event);
    }

    @Override
    public void stop() {
        scheduledExecutorService_.shutdownNow();
        super.stop();
    }
}
