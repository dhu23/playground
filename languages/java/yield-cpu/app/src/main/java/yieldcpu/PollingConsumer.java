package yieldcpu;

import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;

public class PollingConsumer extends Consumer {
    public PollingConsumer(int eventLimit) {
        super(eventLimit);
    }

    @Override
    protected void runOnce() {
        if (!queue.isEmpty()) {
            Record record = queue.poll();
            processRecord(record);
        }
    }
}
