package yieldcpu;

import java.util.*;
import java.util.concurrent.BlockingDeque;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;

public abstract class Consumer implements Runnable {
    protected final int eventLimit;
    protected final LinkedBlockingQueue<Record> queue;

    protected final Map<String, Integer> counter;
    protected final List<Long> performance;
    protected int totalWordCount;

    public Consumer(int eventLimit) {
        this.eventLimit = eventLimit;

        this.queue = new LinkedBlockingQueue<>();
        counter = new HashMap<>();
        performance = new ArrayList<>();
        totalWordCount = 0;
    }

    public List<Long> getPerformance() {
        return performance;
    }

    public BlockingQueue<Record> getQueue() {
        return this.queue;
    }

    @Override
    public void run() {
        try {
            while (totalWordCount < this.eventLimit) {
                // System.out.println(String.format("%d < %d", totalWordCount, eventLimit));
                runOnce();
            }
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }

    protected abstract void runOnce() throws InterruptedException;

    protected void processRecord(Record record) {
        long latency = System.nanoTime() - record.nano;

        counter.computeIfAbsent(record.data, w -> 0);
        counter.computeIfPresent(record.data, (w, c) -> c + 1);
        ++totalWordCount;

        if (totalWordCount % 1000 == 0) {
            System.out.println(String.format("%s: consumed %s words",
                    this.getClass().getSimpleName(), totalWordCount));
        }

        // calculate performance
        performance.add(latency);
    }
}
