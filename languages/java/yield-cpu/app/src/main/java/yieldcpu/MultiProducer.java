package yieldcpu;

import org.checkerframework.checker.units.qual.A;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;

public class MultiProducer extends Producer {
    private final List<BlockingQueue<Record>> queueList;
    private final Set<Consumer> consumerSet;

    public MultiProducer(
            int eventLimit, ArbitraryWord wordGenerator,
            ArbitraryInteger batchSizeGenerator, ArbitraryInteger breakGenerator) {
        super(eventLimit, wordGenerator, batchSizeGenerator, breakGenerator);
        this.queueList = new ArrayList<>();
        consumerSet = new HashSet<>();
    }

    @Override
    protected void distribute(String word) {
        queueList.forEach(queue -> queue.offer(new Record(word)));
    }

    public void registerConsumer(Consumer consumer) {
        if (!consumerSet.contains(consumer)) {
            queueList.add(consumer.getQueue());
        }
    }
}
