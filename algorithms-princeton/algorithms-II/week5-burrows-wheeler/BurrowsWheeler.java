import edu.princeton.cs.algs4.BinaryStdIn;
import edu.princeton.cs.algs4.BinaryStdOut;

// import edu.princeton.cs.algs4.StdOut;

import edu.princeton.cs.algs4.Stack;
import edu.princeton.cs.algs4.ST;

import java.util.ArrayList;

public class BurrowsWheeler {

    private static final int R = 256;
    
    public static void encode() {
        String input = BinaryStdIn.readString();
        CircularSuffixArray cfa = new CircularSuffixArray(input);

        int len = cfa.length();
        for (int i = 0; i < len; ++i) {
            if (cfa.index(i) == 0) {
                BinaryStdOut.write(i);
                break;
            }
        }
        for (int i = 0; i < len; ++i) {
            int idx = cfa.index(i) + len - 1;
            if ( idx >= len ) idx -= len;
            // idx = idx >= len ? idx - len : idx;
            BinaryStdOut.write(input.charAt(idx));
        }
        BinaryStdOut.close();
    }

    public static void decode() {
        int first = BinaryStdIn.readInt();
        Stack<Character> temp = new Stack<Character>();
        while (!BinaryStdIn.isEmpty()) temp.push(BinaryStdIn.readChar());
     
        // build tail column and head column(sorted)
        char[] t = new char[temp.size()];
        char[] h = new char[temp.size()];
        int c = temp.size()-1;
        while (!temp.isEmpty()) {
            char pop = temp.pop();
            t[c] = pop;
            h[c] = pop;
            --c;
        }
        // Arrays.sort(h);
        sortByKeyIndexCounting(h);

        // map from character to positions. list size would should be 
        ST<Character, ArrayList<Integer> > occuranceMap = 
            new ST<Character, ArrayList<Integer> >();

        for (int i = 0; i < t.length; ++i) {
            if (!occuranceMap.contains(t[i])) {
                ArrayList<Integer> occurances = new ArrayList<Integer>();
                occurances.add(i);
                occuranceMap.put(t[i], occurances);
            }    
            else
            {
                occuranceMap.get(t[i]).add(i);
            }
            
        }

        // build next array
        int[] next = new int[h.length];
        ST<Character, Integer> countMap = new ST<Character, Integer>();
        for (int i = 0; i < h.length; ++i) {
            int occurred = 0;
            if (countMap.contains(h[i])) occurred = countMap.get(h[i]);

            next[i] = occuranceMap.get(h[i]).get(occurred);
         
            ++occurred;
            countMap.put(h[i], occurred);
        }

        // decode the message with t[], first and next[]
        int index = first;
        c = 0;
        do {
            BinaryStdOut.write(h[index]);
            index = next[index];
            ++c;
        } while (c != h.length);
        BinaryStdOut.close();
    }

    private static void sortByKeyIndexCounting(char[] a) {
        int[] count = new int[R+1];
        char[] aux = new char[a.length];
        
        for (int i = 0; i < a.length; ++i) count[a[i]+1]++;
        for (int i = 0; i < R; ++i) count[i+1] += count[i];
        for (int i = 0; i < a.length; ++i) aux[count[a[i]]++] = a[i];
        for (int i = 0; i < a.length; ++i) a[i] = aux[i];
    }

    private static int findIndex(char[] a, char c, int occurance) {
        int count = 0; // count occurance
        for (int i = 0; i < a.length; ++i) {
            if (a[i] == c) ++count;
            if (count == occurance) return i;
        }
        throw new UnsupportedOperationException(
                occurance+"th "+c+" is NOT found in array");
    }

    public static void main(String[] args) {
        if (args == null || args.length == 0) {
            throw new java.lang.NullPointerException("Null input");
        }

        if (args[0].equals("-")) encode();
        else if (args[0].equals("+")) decode();
    }

}
