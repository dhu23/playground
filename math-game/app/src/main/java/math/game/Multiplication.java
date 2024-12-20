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
        int root = (int) Math.sqrt(productLimit);
        int left = random.nextInt(2, root);
        int right = random.nextInt(2, productLimit / left + 1);

        if (random.nextDouble() < 0.5) {
            return new Multiplication(left, right);
        }
        return new Multiplication(right, left);
    }
}
