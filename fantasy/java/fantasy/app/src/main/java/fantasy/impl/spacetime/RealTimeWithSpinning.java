package fantasy.impl.spacetime;

import com.google.common.annotations.VisibleForTesting;
import fantasy.impl.event.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.time.Instant;


public class RealTimeWithSpinning extends AbstractWorldTime {
    private static final Logger logger = LoggerFactory.getLogger(RealTimeWithSpinning.class);

    public RealTimeWithSpinning(SequenceNumber sequenceNumber) {
        super(sequenceNumber);
    }

    protected void receiveEvent(Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event) {
        pushEventToQueue(event);
    }

    @VisibleForTesting
    protected void onEvent_(Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event) {
        WorldTimeEventPool.WorldTimeEvent worldTimeEvent = event.data();
        Instant now = Instant.now();

        if (now.isBefore(worldTimeEvent.availableTime())) {
            pushEventToQueue(event);
            return;
        }

        processEvent(event);
    }
}
