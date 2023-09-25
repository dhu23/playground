package jlib.impl.timer;

import com.google.common.collect.Streams;
import jlib.data.timer.TimerEvent;
import jlib.intf.timer.TimerRegistrar;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.junit.jupiter.MockitoExtension;

import java.sql.Time;
import java.time.Instant;
import java.util.List;
import java.util.concurrent.*;
import java.util.stream.IntStream;
import java.util.stream.Stream;

@ExtendWith(MockitoExtension.class)
public class TimerRegistrarImplTest {
    private ScheduledExecutorService scheduledExecutorService;
    private BlockingQueue<ExecutionResult> executedResultQueue;

    @BeforeEach
    public void setup() {
        scheduledExecutorService = Executors.newScheduledThreadPool(5);
        executedResultQueue = new LinkedBlockingQueue<>();
    }

    @Test
    public void testImplementation1() {
        TimerRegistrarImpl1 timerRegistrar = new TimerRegistrarImpl1(scheduledExecutorService);
        runTest(timerRegistrar);
        System.out.println(executedResultQueue);
    }

    protected void runTest(TimerRegistrar timerRegistrar) {
        Stream<Long> delayStream = Stream.of(1000L, 2000L, 3000L, 2000L, 3000L, 4000L, 3000L, 4000L);
        Stream<Integer> taskIdStream = Stream.iterate(1, i -> i + 1);
        Streams.forEachPair(delayStream, taskIdStream, (delay, taskId) -> {
            timerRegistrar.register(delay, String.format("task#%d", taskId), this::consumer);
        });
        try {
            Thread.sleep(10000);
        } catch (InterruptedException e) {

        }
    }

    protected void consumer(TimerEvent timerEvent) {
        executedResultQueue.add(new ExecutionResult(timerEvent));
    }

    protected static class ExecutionResult {
        public TimerEvent timerEvent;
        public Instant executionTime;
        public long lagInMillis;

        public ExecutionResult(TimerEvent timerEvent) {
            this.timerEvent = timerEvent;
            executionTime = Instant.now();
            lagInMillis = executionTime.toEpochMilli() - timerEvent.eventTime().toEpochMilli();
        }

        @Override
        public String toString() {
            return String.format("`TimerEvent: %s, `ExecutionTime: %s, `lagInMillis: %d", timerEvent, executionTime, lagInMillis);
        }
    }
}
