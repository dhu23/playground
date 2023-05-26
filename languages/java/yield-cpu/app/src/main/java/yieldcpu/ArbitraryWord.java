package yieldcpu;

import java.util.Random;

public class ArbitraryWord implements Arbitrary<String> {
    private final int minWordLength;
    private final int maxWordLength;
    private final Random random;
    private final ArbitraryLowerCaseLetter arbitraryLowerCaseLetter;

    public ArbitraryWord(int minWordLength, int maxWordLength, Random random, ArbitraryLowerCaseLetter arbitraryLowerCaseLetter) {
        this.minWordLength = minWordLength;
        this.maxWordLength = maxWordLength;
        this.random = random;
        this.arbitraryLowerCaseLetter = arbitraryLowerCaseLetter;
    }


    @Override
    public String arbitrary() {
        int length = random.nextInt(maxWordLength - minWordLength) + minWordLength;
        StringBuilder builder = new StringBuilder();

        while (length > 0) {
            builder.append(arbitraryLowerCaseLetter.arbitrary());
            --length;
        }

        return builder.toString();
    }
}
