package yieldcpu;

import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;

public class TimeOutPollingConsumer extends Consumer {
    protected final long timeOutInMillis;
    public TimeOutPollingConsumer(int eventLimit, long timeOutInMillis) {
        super(eventLimit);
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
