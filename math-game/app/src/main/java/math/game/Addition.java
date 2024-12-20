package math.game;

import java.util.Random;

public class Addition implements Expression {
    private final int left;
    private final int right;
    private final int sum;

    public Addition(int left, int right) {
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

    public static Addition makeAddition(Random random, int sumLimit) {
        int left = random.nextInt(1, sumLimit);
        int right = random.nextInt(1, sumLimit - left + 1);
        return new Addition(left, right);
    }
}
