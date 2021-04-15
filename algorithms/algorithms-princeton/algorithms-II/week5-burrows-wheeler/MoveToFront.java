
import edu.princeton.cs.algs4.BinaryStdOut;
import edu.princeton.cs.algs4.BinaryStdIn;

public class MoveToFront {
    private static final int R = 256;

    public static void encode() {

        char[] sequence = initSequence();

        while (!BinaryStdIn.isEmpty()) {
            char readIn = BinaryStdIn.readChar();
            int readInIndex = findIndex(sequence, readIn);
            BinaryStdOut.write((char) readInIndex);
            
            moveToFront(sequence, readInIndex);
        }
        BinaryStdOut.close();
    }

    public static void decode() {

        char[] sequence = initSequence();

        while (!BinaryStdIn.isEmpty()) {
            int readInIndex = (int) (BinaryStdIn.readChar());

            BinaryStdOut.write(sequence[readInIndex]);
            
            moveToFront(sequence, readInIndex);
        }   
        BinaryStdOut.close();
    }

    private static char[] initSequence() {
        char[] s = new char[R];
        for (char i = 0; i < R; ++i) s[i] = i;
        return s;
    }

    private static int findIndex(char[] a, char c) {
            
        for (int i = 0; i < a.length; ++i) if (a[i] == c) return i;
        throw new UnsupportedOperationException(
                c+" is NOT found in the ASCII sequence");
    }

    private static void moveToFront(char[] a, int index) {
        assert index >= 0 && index < a.length;

        if (index == 0) return;

        char remember = a[index];
        for (int i = index; i > 0; --i) a[i] = a[i-1];
        a[0] = remember;
    }

    public static void main(String[] args) {
        if (args == null || args.length == 0) {
            throw new java.lang.NullPointerException("Null input");
        }

        if (args[0].equals("-")) encode();
        else if (args[0].equals("+")) decode();
    }
}
