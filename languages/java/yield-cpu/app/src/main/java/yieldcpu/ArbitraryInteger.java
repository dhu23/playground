package yieldcpu;

import java.util.Random;

public class ArbitraryInteger implements Arbitrary<Integer> {
    private final int minInteger;
    private final int maxInteger;
    private final Random random;

    public ArbitraryInteger(int minInteger, int maxInteger, Random random) {
        this.minInteger = minInteger;
        this.maxInteger = maxInteger;
        this.random = random;
    }

    @Override
    public Integer arbitrary() {
        return minInteger + random.nextInt(maxInteger - minInteger);
    }
}
