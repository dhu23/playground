package fantasy.impl.spacetime;

import com.google.common.annotations.VisibleForTesting;
import fantasy.impl.event.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import javax.annotation.Nullable;
import java.time.Instant;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.function.Consumer;
import java.util.function.Supplier;


public class RealTimingWithSpinning extends AbstractRealTiming {
    private static final Logger logger = LoggerFactory.getLogger(RealTimingWithSpinning.class);

    public RealTimingWithSpinning(SequenceNumber sequenceNumber, @Nullable Instant endTime) {
        super(new RealWorldWallClock(), endTime, sequenceNumber);
        this.eventQueue_.start();
    }

    protected void receiveEvent(Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event) {
        pushEventToBlockingQueue(event);
    }

    @Override
    protected void onEvent_(Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event) {
        WorldTimeEventPool.WorldTimeEvent worldTimeEvent = event.data();
        Instant now = now();

        if (now.isBefore(worldTimeEvent.availableTime())) {
            pushEventToBlockingQueue(event);
        } else {
            processEvent(event);
        }
    }
}
