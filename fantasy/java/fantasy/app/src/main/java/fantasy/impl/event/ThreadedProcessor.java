package fantasy.impl.event;

import javax.annotation.Nullable;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.function.Predicate;
import java.util.function.Supplier;

public abstract class ThreadedProcessor<T> {
    private final Thread thread_;
    private final CountDownLatch stopLatch_;
    private boolean startedOnce_;
    private Supplier<Boolean> endConditionPredicate_;

    public ThreadedProcessor(String name,
                             Supplier<Boolean> endConditionPredicate) {
        this.thread_ = new Thread(this::run_, String.format("%s-thread", name));
        this.stopLatch_ = new CountDownLatch(1);
        startedOnce_ = false;
        endConditionPredicate_ = endConditionPredicate;
    }

    public CountDownLatch getLatch() {
        return stopLatch_;
    }

    public void start() {
        if (!startedOnce_) {
            startedOnce_ = true;
            this.thread_.start();
        }
    }

    public void stop() {
        signalStop();
        try {
            this.thread_.join();
        } catch (InterruptedException e) {
        }
    }

    public void signalStop() {
        this.stopLatch_.countDown();
    }

    protected void run_() {
        while (this.stopLatch_.getCount() > 0) {
            try {
                runOnce_();
                if (endConditionPredicate_.get()) {
                    signalStop();
                }
            } catch (Exception e) {
                // ignore it
            }
        }
    }

    public abstract void add(T data);
    protected abstract void runOnce_() throws Exception;
}
