package yieldcpu;

import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * this class generates String data
 * each string is a word with [m, n) random letters
 * every time it decides to generate, it generates [a, b) words
 * then it takes a break between [x, y) milliseconds
 * it does it until N words were generated to the queue
 */
public class Producer implements Runnable {
    private final AtomicBoolean stopFlag;
    private final ArbitraryWord wordGenerator;
    private final ArbitraryInteger batchSizeGenerator;
    private final ArbitraryInteger breakGenerator;
    private final LinkedBlockingQueue<Record> queue;

    public Producer(
            AtomicBoolean stopFlag, ArbitraryWord wordGenerator,
            ArbitraryInteger batchSizeGenerator, ArbitraryInteger breakGenerator,
            LinkedBlockingQueue<Record> queue) {
        this.stopFlag = stopFlag;
        this.wordGenerator = wordGenerator;
        this.batchSizeGenerator = batchSizeGenerator;
        this.breakGenerator = breakGenerator;
        this.queue = queue;
    }

    @Override
    public void run() {
        int totalWordCount = 0;
        int batchCount = 0;
        try {
            while (true) {
                int batchSize = batchSizeGenerator.arbitrary();

                {
                    int wordCount = 0;
                    while (wordCount < batchSize) {
                        String word = wordGenerator.arbitrary();
                        queue.offer(new Record(word));
                        ++wordCount;

                        if (stopFlag.get()) {
                            return;
                        }
                    }
                }

                int sleepInMillis = breakGenerator.arbitrary();
//                System.out.println(
//                        String.format("sleeping %d ms after batch=%d of size=%d",
//                                sleepInMillis, batchCount, batchSize));
                Thread.sleep(sleepInMillis);

                totalWordCount += batchSize;
                ++batchCount;

                // System.out.println(String.format("produced %d words", totalWordCount));
            }
        } catch (InterruptedException e) {

        }
    }
}
