package fantasy.impl.spacetime;

import fantasy.impl.event.Event;
import fantasy.impl.event.ThreadedProcessor;

import javax.annotation.Nullable;
import java.time.Instant;
import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.concurrent.CountDownLatch;
import java.util.function.Consumer;
import java.util.function.Supplier;

public class MachineSpeedTiming extends AbstractWorldTiming {
    // priority queue to maintain timer
    private final SimulationClock simulationClock_;
    protected final ThreadedProcessor<Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent>> eventPriorityQueueProcessor_;

    public MachineSpeedTiming(Instant initialTime, SequenceNumber sequenceNumber, @Nullable Instant endTime) {
        super(new SimulationClock(initialTime), endTime, sequenceNumber);
        simulationClock_ = (SimulationClock) getClock();
        this.eventPriorityQueueProcessor_ = new EventPriorityQueueProcessor(
                simulationClock_, this::onEvent_, this::shouldEnd);
        this.eventPriorityQueueProcessor_.start();
    }

    @Override
    protected void receiveEvent(Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event) {
        this.eventPriorityQueueProcessor_.add(event);
    }

    @Override
    protected void onEvent_(Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event) {
        // move the clock
        processEvent(event);
    }

    @Override
    public void stop() {
        this.eventPriorityQueueProcessor_.stop();
    }

    @Override
    public void await() {
        try {
            this.eventPriorityQueueProcessor_.getLatch().await();
        } catch (InterruptedException e) {
            // let it through
        }
    }

    public static class WorldTimeEventComparator
            implements Comparator<Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent>> {

        @Override
        public int compare(Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> left,
                           Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> right) {
            Instant leftTime = left.data().availableTime();
            Instant rightTime = right.data().availableTime();
            if (leftTime.isBefore(rightTime)) {
                return -1;
            } else if (rightTime.isBefore(leftTime)) {
                return 1;
            } else {
                return 0;
            }
        }
    }

    public static class EventPriorityQueueProcessor
        extends ThreadedProcessor<Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent>> {

        private final SimulationClock clock_;
        private final PriorityQueue<Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent>> priorityQueue_;
        private final Consumer<Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent>> consumer_;

        public EventPriorityQueueProcessor(SimulationClock clock,
                                           Consumer<Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent>> consumer,
                                           Supplier<Boolean> endConditionPredicate) {
            super("event-priority-queue-processor", endConditionPredicate);
            clock_ = clock;
            priorityQueue_ = new PriorityQueue<>(new WorldTimeEventComparator());
            consumer_ = consumer;
        }

        @Override
        public void add(Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event) {
            this.priorityQueue_.add(event);
        }

        @Override
        protected void runOnce_() throws Exception {
            Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event = this.priorityQueue_.poll();
            if (event != null) {
                System.out.println(String.format("got event: %s", event));
                clock_.moveTo(event.data().availableTime());
                consumer_.accept(event);
            }
        }
    }
}
