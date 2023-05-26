package yieldcpu;

import org.checkerframework.checker.units.qual.A;

import java.util.Random;

public class ArbitraryLowerCaseLetter implements Arbitrary<Character> {
    private final ArbitraryInteger arbitraryInteger;

    public ArbitraryLowerCaseLetter(Random random) {
        this.arbitraryInteger = new ArbitraryInteger(0, 26, random);
    }

    @Override
    public Character arbitrary() {
        return (char) ('a' + arbitraryInteger.arbitrary());
    }
}
