package fantasy.impl;

import com.google.common.base.Preconditions;

import java.util.random.RandomGenerator;

public class RandomUtils {
    /**
     * Given a random variable of `chance`, (i.e. 0.1234 = 12.34%),
     * roll a dice to see if it should happen. It assumes 4 decimal precision.
     * @param chance
     * @param randomGenerator
     * @return
     */
    public static boolean roll(double chance, RandomGenerator randomGenerator) {
        Preconditions.checkState(chance >= 0.0);
        int threshold = (int) (chance * 10000);
        if (threshold == 0) {
            return false;
        } else if (threshold >= 10000) {
            return true;
        }
        // roll within [0, 9999], if the number is less than threshold, return true
        return randomGenerator.nextInt(0, 10000) < threshold;
    }
}
