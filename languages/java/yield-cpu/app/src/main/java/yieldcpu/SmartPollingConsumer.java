package yieldcpu;

import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;

public class SmartPollingConsumer extends TimeOutPollingConsumer {
    public SmartPollingConsumer(int eventLimit, LinkedBlockingQueue<Record> queue, long timeOutInMillis) {
        super(eventLimit, queue, timeOutInMillis);
    }

    @Override
    protected void runOnce() throws InterruptedException {
        if (!queue.isEmpty()) {
            Record record = queue.poll();
            processRecord(record);
        } else {
            Record record = queue.poll(timeOutInMillis, TimeUnit.MILLISECONDS);
            if (record != null) {
                processRecord(record);
            }
        }
    }
}
