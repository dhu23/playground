package yieldcpu;

import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;

public class YieldingConsumer extends Consumer {
    public YieldingConsumer(
            int eventLimit,
            LinkedBlockingQueue<Record> queue, AtomicBoolean stopFlag, StopWatch stopWatch) {
        super(eventLimit, queue, stopFlag, stopWatch);
    }

    @Override
    protected void runOnce() throws InterruptedException {
        if (!queue.isEmpty()) {
            Record record = queue.poll();
            processRecord(record);
        } else {
            Thread.yield();
        }
    }
}
