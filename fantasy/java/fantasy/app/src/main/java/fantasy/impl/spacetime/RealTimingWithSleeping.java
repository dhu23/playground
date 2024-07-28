package fantasy.impl.spacetime;

import fantasy.impl.event.Event;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import javax.annotation.Nullable;
import java.time.Duration;
import java.time.Instant;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class RealTimingWithSleeping extends AbstractRealTiming {
    private static final Logger logger = LoggerFactory.getLogger(RealTimingWithSleeping.class);
    private final ScheduledExecutorService scheduledExecutorService_;

    public RealTimingWithSleeping(SequenceNumber sequenceNumber, @Nullable Instant endTime) {
        super(endTime, sequenceNumber);
        scheduledExecutorService_ = Executors.newSingleThreadScheduledExecutor();
        this.eventQueue_.start();
    }

    @Override
    protected void receiveEvent(Event<WorldTimeEventPool.EventType, Object> event) {
        Instant now = now();
        Object data = event.data();
        if (data instanceof WorldTimeEventPool.WorldTimeEvent worldTimeEvent) {
            Instant targetInstant = worldTimeEvent.availableTime();
            if (now.isBefore(targetInstant)) {
                Duration wait = Duration.between(now, targetInstant);
                scheduledExecutorService_.schedule(() -> {
                    pushEventToBlockingQueue(event);
                }, wait.toMillis(), TimeUnit.MILLISECONDS);
                return;
            }
        }

        pushEventToBlockingQueue(event);
    }

    @Override
    protected void onEvent_(Event<WorldTimeEventPool.EventType, Object> event) {
        logger.info("processing {}", event);
        processEvent(event);
    }

    @Override
    public void stop() {
        scheduledExecutorService_.shutdownNow();
        super.stop();
    }
}
