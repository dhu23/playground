package yieldcpu;

import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;

public class YieldingConsumer extends Consumer {
    public YieldingConsumer(int eventLimit) {
        super(eventLimit);
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
