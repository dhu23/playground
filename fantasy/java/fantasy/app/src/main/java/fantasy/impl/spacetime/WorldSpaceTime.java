package fantasy.impl.spacetime;

import fantasy.impl.LogUtils;
import fantasy.intf.WorldTime;

import java.io.IOException;
import java.util.Random;
import java.util.random.RandomGenerator;

/**
 * provide different space/time utilities as one singleton
 */
public class WorldSpaceTime {
    private static final WorldSpaceTime INSTANCE = new WorldSpaceTime();

    public static WorldSpaceTime getInstance() {
        return INSTANCE;
    }

    private final LogUtils logUtils_;
    private final SequenceNumber sequence_;
    private final RandomGenerator randomGenerator_;
    private final WorldTime worldTime_;

    public WorldSpaceTime() {
        try {
            this.logUtils_ = new LogUtils("./logs");
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        sequence_ = new SequenceNumber();
        randomGenerator_ = new Random();
        worldTime_ = new RealTimeImpl1(sequence_);
    }

    public void begin() {
        worldTime_.start();
    }

    public void end() {
        worldTime_.stop();
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

    public WorldTime getWorldTime() {
        return worldTime_;
    }
}
