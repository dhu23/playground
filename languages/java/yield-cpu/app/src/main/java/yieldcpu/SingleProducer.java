package yieldcpu;

import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;

public class SingleProducer extends Producer {
     private final LinkedBlockingQueue<Record> queue;
    public SingleProducer(
            int eventLimit, ArbitraryWord wordGenerator, ArbitraryInteger batchSizeGenerator, ArbitraryInteger breakGenerator, LinkedBlockingQueue<Record> queue) {
        super(eventLimit, wordGenerator, batchSizeGenerator, breakGenerator);
        this.queue = queue;
    }

    @Override
    protected void distribute(String word) {
        this.queue.offer(new Record(word));
    }
}
