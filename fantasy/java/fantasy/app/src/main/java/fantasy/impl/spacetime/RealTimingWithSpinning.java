package fantasy.impl.spacetime;

import fantasy.impl.event.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import javax.annotation.Nullable;
import java.time.Instant;


public class RealTimingWithSpinning extends AbstractRealTiming {
    private static final Logger logger = LoggerFactory.getLogger(RealTimingWithSpinning.class);

    public RealTimingWithSpinning(SequenceNumber sequenceNumber, @Nullable Instant endTime) {
        super(endTime, sequenceNumber);
        this.eventQueue_.start();
    }

    protected void receiveEvent(Event<WorldTimeEventPool.EventType, Object> event) {
        pushEventToBlockingQueue(event);
    }

    @Override
    protected void onEvent_(Event<WorldTimeEventPool.EventType, Object> event) {
        logger.info("processing {}", event);
        if (event.data() instanceof WorldTimeEventPool.WorldTimeEvent worldTimeEvent) {
            Instant now = now();

            if (now.isBefore(worldTimeEvent.availableTime())) {
                pushEventToBlockingQueue(event);
                return;
            }
        }
        processEvent(event);
    }
}
