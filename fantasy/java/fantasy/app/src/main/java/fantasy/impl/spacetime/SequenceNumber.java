package fantasy.impl.spacetime;

import java.util.concurrent.atomic.AtomicLong;

public class SequenceNumber {
    private final AtomicLong counter_;

    public SequenceNumber() {
        counter_ = new AtomicLong(0);
    }

    public long getId() {
        return counter_.getAndIncrement();
    }
}
