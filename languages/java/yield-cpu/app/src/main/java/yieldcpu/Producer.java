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
public abstract class Producer implements Runnable {
    private final int eventLimit;
    private final ArbitraryWord wordGenerator;
    private final ArbitraryInteger batchSizeGenerator;
    private final ArbitraryInteger breakGenerator;


    public Producer(
            int eventLimit, ArbitraryWord wordGenerator,
            ArbitraryInteger batchSizeGenerator, ArbitraryInteger breakGenerator) {
        this.eventLimit = eventLimit;
        this.wordGenerator = wordGenerator;
        this.batchSizeGenerator = batchSizeGenerator;
        this.breakGenerator = breakGenerator;
    }

    @Override
    public void run() {
        int producedCount = 0;
        try {
            while (true) {
                int batchSize = this.batchSizeGenerator.arbitrary();

                int wordCount = 0;
                while (wordCount < batchSize) {
                    String word = this.wordGenerator.arbitrary();
                    distribute(word);
                    ++wordCount;

                    ++producedCount;
                    if (producedCount == this.eventLimit) {
                        return;
                    }
                }

                int sleepInMillis = breakGenerator.arbitrary();
                Thread.sleep(sleepInMillis);
            }
        } catch (InterruptedException e) {

        }
    }

    protected abstract void distribute(String word);
}
