package yieldcpu;

import java.util.*;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;

public abstract class Consumer implements Runnable {
    protected final int eventLimit;
    protected final LinkedBlockingQueue<Record> queue;
    protected final AtomicBoolean stopFlag;
    protected final StopWatch stopWatch;
    protected final Map<String, Integer> counter;
    protected final List<Long> performance;
    protected int totalWordCount;

    public Consumer(int eventLimit, LinkedBlockingQueue<Record> queue, AtomicBoolean stopFlag, StopWatch stopWatch) {
        this.eventLimit = eventLimit;
        this.queue = queue;
        this.stopFlag = stopFlag;
        this.stopWatch = stopWatch;
        counter = new HashMap<>();
        performance = new ArrayList<>();
        totalWordCount = 0;
    }

    public List<Long> getPerformance() {
        return performance;
    }

    @Override
    public void run() {
        stopWatch.start();
        try {
            while (totalWordCount < eventLimit) {
                // System.out.println(String.format("%d < %d", totalWordCount, eventLimit));
                runOnce();
            }
            stopFlag.set(true);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }

    protected abstract void runOnce() throws InterruptedException;

    protected void processRecord(Record record) {
        counter.computeIfAbsent(record.data, w -> 0);
        counter.computeIfPresent(record.data, (w, c) -> c + 1);
        ++totalWordCount;

        if (totalWordCount % 2000 == 0) {
            System.out.println(String.format("consumed %s words", totalWordCount));
        }

        // calculate performance
        long latency = System.nanoTime() - record.nano;
        performance.add(latency);
    }
}
