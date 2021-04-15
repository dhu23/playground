import edu.princeton.cs.algs4.StdOut;

public class CircularSuffixArray {

    private static final int R = 256;

    private static final int CUT_OFF = 15;

    private String original;
    private int[] sortedSuffixIndices;

    public CircularSuffixArray(String s) {
        if (s == null) throw new java.lang.NullPointerException("NULL string");
        
        this.original = s;
        // sort suffix strings by LSD
        // this.sortedSuffixIndices = sortSuffixByLSD(this.original);
        // sort suffix strings by MSD
        // this.sortedSuffixIndices = sortSuffixByMSD(this.original);
        // sort suffix strings by 3-way quicksort
        this.sortedSuffixIndices = sortSuffixBy3WayQuick(this.original);
    }

    public int length() { return this.original.length(); } 

    public int index(int i) {
        if (i >= this.length() || i < 0) {
            throw new java.lang.IndexOutOfBoundsException("Invalid index");
        }
        return this.sortedSuffixIndices[i];
    }

    private static char getSuffixChar(String s, int suffix, int at) {
        int loc = suffix+at;
        while (loc >= s.length()) loc -= s.length();
        return s.charAt(loc);
    }

    private static int getMSDSuffixChar(String s, int suffix, int at)
    {
        int len = s.length();
        if (at >= len) return -1;
    
        return getSuffixChar(s, suffix, at);
    }


    private static int getSuffixChar2(String s, int suffix, int at)
    {
        int len = s.length();
        if (at >= len) return -1;
    
        return getSuffixChar(s, suffix, at);
    }

    private static int[] sortSuffixByLSD(String s) {
        int n = s.length();

        // initialize sorted as the original order; the content id stands for
        // the circular suffix at position i
        int[] sorted = new int[n];
        for (int i = 0; i < n; ++i) sorted[i] = i;

        int[] aux = new int[n];
        
        for (int d = n-1; d >= 0; --d) {
            int[] count = new int[R+1];
            for (int i = 0; i < n; ++i) {
                count[getSuffixChar(s, sorted[i], d)+1]++;
            }
            for (int r = 0; r < R; ++r) count[r+1] += count[r];
            for (int i = 0; i < n; ++i) {
                aux[count[getSuffixChar(s, sorted[i], d)]++] = sorted[i];
            }
            for (int i = 0; i < n; ++i) sorted[i] = aux[i];
        }
        return sorted;
    }

    private static int[] sortSuffixByMSD(String s) {
        int[] sorted = new int[s.length()];
        for (int i = 0; i < s.length(); ++i) sorted[i] = i;

        int[] aux = new int[s.length()];

        sortSuffixByMSD(s, sorted, aux, 0, s.length()-1, 0);
        return sorted;
    }

    private static void sortSuffixByMSD(
            String s, int[] sorted, int[] aux, int lo, int hi, int d) {
        
        if (hi <= lo) return;
        
        int[] count = new int[R+2];
        for (int i = lo; i <= hi; ++i) {
            int c = getMSDSuffixChar(s, sorted[i], d);
            count[c+2]++;
        }

        for (int r = 0; r < R+1; ++r) count[r+1] += count[r];
        for (int i = lo; i <= hi; ++i) {
            int c = getMSDSuffixChar(s, sorted[i], d);
            aux[count[c+1]++] = sorted[i];
        }
        for (int i = lo; i <= hi; ++i) sorted[i] = aux[i-lo];

        for (int r = 0; r < R; ++r) {
            sortSuffixByMSD(s, sorted, aux, lo+count[r], lo+count[r+1]-1, d+1);
        }
    }

    private static int[] sortSuffixBy3WayQuick(String s) {
        int[] sorted = new int[s.length()];
        for (int i = 0; i < s.length(); ++i) sorted[i] = i;

        sortSuffixBy3WayQuick(s, sorted, 0, s.length()-1, 0);
        return sorted;
    }

    private static void exch(int[] sorted, int i, int j) {
        int temp = sorted[i];
        sorted[i] = sorted[j];
        sorted[j] = temp;
    }

    private static boolean less(String s, int p1, int p2, int d) {
        // for loop assertion
        for (int i = 0; i < d; ++i) {
            assert getSuffixChar2(s, p1, i) == getSuffixChar2(s, p2, i);
        }

        for (int i = d; i < s.length(); ++i) {
            char char1 = getSuffixChar(s, p1, i);
            char char2 = getSuffixChar(s, p2, i);

            if (char1 < char2) return true;
            if (char1 > char2) return false;
        }
        return false;
    }

    private static void sortByInsertion(
            String s, int[] sorted, int lo, int hi, int d) {
        for (int i = lo; i <= hi; ++i) 
            for (int j = i; j > lo && less(s, sorted[j], sorted[j-1], d); --j)
                exch(sorted, j, j-1);
    }

    private static void sortSuffixBy3WayQuick(
            String s, int[] sorted, int lo, int hi, int d) {
    
        if (hi <= lo + CUT_OFF) {
            sortByInsertion(s, sorted, lo, hi, d);
            return;
        }
        int lt = lo, gt = hi;
        int v = getSuffixChar2(s, sorted[lo], d);
        int i = lo + 1;
        while (i <= gt) {

            int t = getSuffixChar2(s, sorted[i], d);
            if (t < v) exch(sorted, lt++, i++);
            else if (t > v) exch(sorted, i, gt--);
            else i++;
        }

        sortSuffixBy3WayQuick(s, sorted, lo, lt-1, d);
        if (v >= 0) sortSuffixBy3WayQuick(s, sorted, lt, gt, d+1);
        sortSuffixBy3WayQuick(s, sorted, gt+1, hi, d);       
    }
            


    public static void main(String[] args) {
        // CircularSuffixArray cfa = new CircularSuffixArray("ABRACADABRA!");
        // CircularSuffixArray cfa = new CircularSuffixArray("BBBABAAABB");
        CircularSuffixArray cfa = new CircularSuffixArray("zebra");
        for (int i = 0; i < cfa.length(); ++i) {
            StdOut.println(cfa.index(i));
        }
    }
}
