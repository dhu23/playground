package fantasy.impl.spacetime;

import fantasy.impl.LogUtils;
import fantasy.intf.WorldClock;
import fantasy.intf.WorldTiming;

import java.io.IOException;
import java.time.Duration;
import java.time.Instant;
import java.util.Random;
import java.util.concurrent.CountDownLatch;
import java.util.random.RandomGenerator;

/**
 * provide different space/time utilities as one singleton
 */
public class WorldSpaceTime {
    private static final WorldSpaceTime INSTANCE = new WorldSpaceTime();

    public static WorldSpaceTime getInstance() {
        return INSTANCE;
    }

    private final CountDownLatch stopLatch_;
    private final LogUtils logUtils_;
    private final SequenceNumber sequence_;
    private final RandomGenerator randomGenerator_;
    private WorldTiming worldTiming_;

    public WorldSpaceTime() {
        stopLatch_ = new CountDownLatch(1);
        try {
            this.logUtils_ = new LogUtils("./logs");
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        sequence_ = new SequenceNumber();
        randomGenerator_ = new Random();
        worldTiming_ = null;
    }

    public void setUp(Duration duration) {
        Instant now = Instant.now();
        worldTiming_ = new RealTimingWithSpinning(sequence_, now.plus(duration));
//        worldTiming_ = new RealTimingWithSleeping(sequence_, now.plus(duration));
//        worldTiming_ = new MachineSpeedTiming(now, sequence_, now.plus(duration));
    }

    public void await() {
        worldTiming_.await();
        worldTiming_.stop();
    }

    public LogUtils getLog() {
        return logUtils_;
    }

    public SequenceNumber getSequence() {
        return sequence_;
    }

    public RandomGenerator getRandomGenerator() {
        return randomGenerator_;
    }

    public WorldClock getClock() {
        return worldTiming_.getClock();
    }

    public WorldTiming getWorldTiming() {
        return worldTiming_;
    }
}
