package math.game;

import java.util.Random;

public class TwoOperandAddition implements Expression {
    private final int left;
    private final int right;
    private final int sum;

    public TwoOperandAddition(int left, int right) {
        this.left = left;
        this.right = right;
        this.sum = this.left + this.right;
    }

    @Override
    public String display() {
        return String.format("%d + %d", left, right);
    }

    @Override
    public int getValue() {
        return this.sum;
    }

    public static TwoOperandAddition makeAddition(Random random, int limit) {
        return new TwoOperandAddition(random.nextInt(1, limit), random.nextInt(1, limit));
    }
}
