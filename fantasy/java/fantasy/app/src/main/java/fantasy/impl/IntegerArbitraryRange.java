package fantasy.impl;

import fantasy.impl.data.ImmutableIntegerInterval;
import fantasy.impl.data.IntegerInterval;
import fantasy.intf.Arbitrary;

import java.util.random.RandomGenerator;

public class IntegerArbitraryRange implements Arbitrary<Integer> {
    private final IntegerInterval interval;
    private final RandomGenerator random;

    public IntegerArbitraryRange(int lower, int upper, RandomGenerator random) {
        this.interval = ImmutableIntegerInterval.of(lower, upper);
        this.random = random;
    }

    @Override
    public Integer get() {
        return random.nextInt(interval.lower(), interval.lower() + 1);
    }

    public int getInt() {
        return get();
    }
}
