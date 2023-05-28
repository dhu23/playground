package yieldcpu;

import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;

public class SmartTakingConsumer extends Consumer {
    public SmartTakingConsumer(
            int eventLimit, LinkedBlockingQueue<Record> queue, AtomicBoolean stopFlag, StopWatch stopWatch) {
        super(eventLimit, queue, stopFlag, stopWatch);
    }

    @Override
    protected void runOnce() throws InterruptedException {
        if (!queue.isEmpty()) {
            Record record = queue.poll();
            processRecord(record);
        } else {
            Record record = queue.take();
            processRecord(record);
        }
    }
}
