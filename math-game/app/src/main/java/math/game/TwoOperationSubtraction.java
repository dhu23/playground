package math.game;

import java.util.Random;

public class TwoOperationSubtraction implements Expression {
    private final int minuend;
    private final int subtrahend;
    private final int difference;

    public TwoOperationSubtraction(int minuend, int subtrahend) {
        this.minuend = minuend;
        this.subtrahend = subtrahend;
        this.difference = this.minuend - this.subtrahend;
    }

    @Override
    public String display() {
        return String.format("%d - %d", minuend, subtrahend);
    }

    @Override
    public int getValue() {
        return this.difference;
    }

    public static TwoOperationSubtraction makeSubtraction(Random random, int limit) {
        int minuend = random.nextInt(1, limit);
        return new TwoOperationSubtraction(minuend, random.nextInt(minuend));
    }
}
