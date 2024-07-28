package fantasy.impl.spacetime;

import fantasy.impl.event.Event;
import fantasy.impl.event.ImmutableEvent;
import fantasy.impl.event.ThreadedProcessor;

import javax.annotation.Nullable;
import java.time.Instant;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.PriorityQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.function.Consumer;
import java.util.function.Supplier;

public class MachineSpeedTiming extends AbstractWorldTiming {
    // priority queue to maintain timer
    private final SimulationClock simulationClock_;
    protected final ThreadedProcessor<Event<WorldTimeEventPool.EventType, Object>> eventPriorityQueueProcessor_;

    public MachineSpeedTiming(Instant initialTime, SequenceNumber sequenceNumber, @Nullable Instant endTime) {
        super(new SimulationClock(initialTime), endTime, sequenceNumber);
        simulationClock_ = (SimulationClock) getClock();
        this.eventPriorityQueueProcessor_ = new EventPriorityQueueProcessor(
                simulationClock_, this::onEvent_, this::shouldEnd);
        this.eventPriorityQueueProcessor_.start();
    }

    @Override
    protected void receiveEvent(Event<WorldTimeEventPool.EventType, Object> event) {
        this.eventPriorityQueueProcessor_.add(event);
    }

    @Override
    protected void onEvent_(Event<WorldTimeEventPool.EventType, Object> event) {
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
        extends ThreadedProcessor<Event<WorldTimeEventPool.EventType, Object>> {

        private final SimulationClock clock_;
        private final BlockingQueue<Event<WorldTimeEventPool.EventType, Object>> blockingQueue_;
        private final PriorityQueue<Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent>> priorityQueue_;
        private final Consumer<Event<WorldTimeEventPool.EventType, Object>> consumer_;

        public EventPriorityQueueProcessor(SimulationClock clock,
                                           Consumer<Event<WorldTimeEventPool.EventType, Object>> consumer,
                                           Supplier<Boolean> endConditionPredicate) {
            super("event-priority-queue-processor", endConditionPredicate);
            clock_ = clock;
            blockingQueue_ = new LinkedBlockingQueue<>();
            priorityQueue_ = new PriorityQueue<>(new WorldTimeEventComparator());
            consumer_ = consumer;
        }

        @Override
        public void add(Event<WorldTimeEventPool.EventType, Object> event) {
            if (event.data() instanceof WorldTimeEventPool.WorldTimeEvent worldTimeEvent) {
                this.priorityQueue_.add(ImmutableEvent.of(event.type(), worldTimeEvent));
            } else {
                this.blockingQueue_.offer(event);
            }
        }

        @Override
        protected void runOnce_() throws Exception {
            List<Event<WorldTimeEventPool.EventType, Object>> commands = new ArrayList<>();
            blockingQueue_.drainTo(commands);
            commands.forEach(command -> {
                consumer_.accept(command);
            });
            Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event = this.priorityQueue_.poll();
            if (event != null) {
                System.out.println(String.format("got event: %s", event));
                clock_.moveTo(event.data().availableTime());
                consumer_.accept(ImmutableEvent.of(event.type(), event.data()));
            }
        }
    }
}
