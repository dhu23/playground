package yieldcpu;

import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;

public class TimeOutPollingConsumer extends Consumer {
    private final long timeOutInMillis;
    public TimeOutPollingConsumer(
            int eventLimit,
            LinkedBlockingQueue<Record> queue, long timeOutInMillis, AtomicBoolean stopFlag, StopWatch stopWatch) {
        super(eventLimit, queue, stopFlag, stopWatch);
        this.timeOutInMillis = timeOutInMillis;
    }

    @Override
    protected void runOnce() throws InterruptedException {
        Record record = queue.poll(timeOutInMillis, TimeUnit.MILLISECONDS);
        if (record != null) {
            // System.out.println("runone");
            processRecord(record);
        }
    }
}
