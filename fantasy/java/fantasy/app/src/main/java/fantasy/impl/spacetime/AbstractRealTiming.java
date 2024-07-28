package fantasy.impl.spacetime;

import fantasy.impl.event.Event;
import fantasy.impl.event.ThreadedProcessor;
import fantasy.intf.WorldClock;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import javax.annotation.Nullable;
import java.time.Instant;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.function.Consumer;
import java.util.function.Supplier;

public abstract class AbstractRealTiming extends AbstractWorldTiming {
    private static final Logger logger = LoggerFactory.getLogger(AbstractRealTiming.class);

    protected final ThreadedProcessor<Event<WorldTimeEventPool.EventType, Object>> eventQueue_;

    public AbstractRealTiming(@Nullable Instant endTime, SequenceNumber sequenceNumber) {
        super(new RealWorldWallClock(), endTime, sequenceNumber);
        this.eventQueue_ = new EventBlockingQueueProcessor<>(this::onEvent_, this::shouldEnd);
    }

    @Override
    public void stop() {
        this.eventQueue_.stop();
    }

    @Override
    public void await() {
        try {
            this.eventQueue_.getLatch().await();
        } catch (InterruptedException e) {
            // let it through
        }
    }

    protected void pushEventToBlockingQueue(Event<WorldTimeEventPool.EventType, Object> event) {
        this.eventQueue_.add(event);
    }

    public class EventBlockingQueueProcessor<T> extends ThreadedProcessor<T> {
        private final BlockingQueue<T> queue_;
        private final Consumer<T> consumer_;

        public EventBlockingQueueProcessor(Consumer<T> consumer,
                                           Supplier<Boolean> endConditionPredicate) {
            super("event-blocking-queue-processing-thread", endConditionPredicate);
            this.queue_ = new LinkedBlockingQueue<>();
            this.consumer_ = consumer;
        }

        @Override
        public void add(T event) {
            this.queue_.offer(event);
        }

        @Override
        protected void runOnce_() throws Exception {
            T data = queue_.poll(100, TimeUnit.MILLISECONDS);
            if (data != null) {
                // System.out.println("got something!!!!!!!");
                consumer_.accept(data);
            } else {
                // System.out.println("got null");
            }
        }
    }
}
