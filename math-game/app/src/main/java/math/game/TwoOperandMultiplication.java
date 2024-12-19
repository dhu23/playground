package math.game;

import java.util.Random;

public class TwoOperandMultiplication implements Expression {
    private final int left;
    private final int right;
    private final int product;

    public TwoOperandMultiplication(int left, int right) {
        this.left = left;
        this.right = right;
        this.product = this.left * this.right;
    }

    @Override
    public String display() {
        return String.format("%d * %d", this.left, this.right);
    }

    @Override
    public int getValue() {
        return this.product;
    }

    public static TwoOperandMultiplication makeMultiplication(Random random, int productLimit) {
        int half = productLimit / 2;
        int left = random.nextInt(2, half);
        int right = random.nextInt(2, productLimit / left);
        return new TwoOperandMultiplication(left, right);
    }
}
