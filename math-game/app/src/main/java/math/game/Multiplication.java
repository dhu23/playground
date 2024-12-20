package math.game;

import java.util.Random;

public class Multiplication implements Expression {
    private final int left;
    private final int right;
    private final int product;

    public Multiplication(int left, int right) {
        this.left = left;
        this.right = right;
        this.product = this.left * this.right;
    }

    @Override
    public String display() {
        return String.format("%d x %d", this.left, this.right);
    }

    @Override
    public int getValue() {
        return this.product;
    }

    public static Multiplication makeMultiplication(Random random, int productLimit) {
        int half = productLimit / 2;
        int left = random.nextInt(2, half);
        int right = random.nextInt( productLimit / left);
        return new Multiplication(left, right);
    }
}
