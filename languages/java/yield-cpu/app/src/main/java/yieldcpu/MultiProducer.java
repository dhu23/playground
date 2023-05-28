package yieldcpu;

import org.checkerframework.checker.units.qual.A;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;

public class MultiProducer extends Producer {
    private final List<LinkedBlockingQueue<Record>> queueList;

    public MultiProducer(
            int eventLimit, ArbitraryWord wordGenerator,
            ArbitraryInteger batchSizeGenerator, ArbitraryInteger breakGenerator) {
        super(eventLimit, wordGenerator, batchSizeGenerator, breakGenerator);
        this.queueList = new ArrayList<>();
    }

    @Override
    protected void distribute(String word) {
        queueList.forEach(queue -> queue.offer(new Record(word)));
    }

    public void register(LinkedBlockingQueue<Record> queue) {
        this.queueList.add(queue);
    }
}
