package math.game;

import java.util.Random;

public class Subtraction implements Expression {
    private final int minuend;
    private final int subtrahend;
    private final int difference;

    public Subtraction(int minuend, int subtrahend) {
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

    public static Subtraction makeSubtraction(Random random, int limit) {
        int minuend = random.nextInt(2, limit + 1);
        return new Subtraction(minuend, random.nextInt(1, minuend));
    }
}
