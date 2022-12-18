// given a list of unique numbers, check if a number is in the list
// [1, 4, 5, 2, 11, 100, 87]
//
// function signature
import java.lang.Thread;
import java.util.List;
import java.util.ArrayList;
import java.time.Instant;
import java.time.Duration;

public class SearchMain {

    // signature
    //    input String[],
    //    its name

    //    output void
    public static void main(String[] args) {
        compare();
    }

    public static void test() {
        int[] array = new int[]{1, 4, 5, 2, 11, 100, 87};

        Searcher ls = new LinearSearch(array);
        SearchResult result = ls.findNumber(11);
        System.out.println(result);

        SearchResult result2 = ls.findNumber(20);
        System.out.println(result2);

        Searcher bls = new BackwardLinearSearch(array);
        SearchResult result3 = bls.findNumber(11);
        System.out.println(result3);

        SearchResult result4 = bls.findNumber(20);
        System.out.println(result4);
        
        System.out.println("==============================");

        Searcher bs = new BinarySearch(array);
        for (int i = -10; i <= 110; ++i) {
            SearchResult resultx = bs.findNumber(i);
            System.out.println(resultx);
        }
    }

    public static void compare() {
        int[] bigArray = new int[1000000];
        for (int i = 0; i < 1000000; ++i) {
            bigArray[i] = i;
        }

        // search -10, 100, 1000, 2000000
        // use LinearSearch
        // int[] targets = new int[]{-10, 100, 1000, 500000, 600000, 700000, 800000, 2000000};

        int[] targets = new int[10000];
        for (int i = 0; i < 10000; ++i) {
            targets[i] = i*10000;
        }
        
        // checkSearchPerformance(targets, new LinearSearch(bigArray));
        // checkSearchPerformance(targets, new BackwardLinearSearch(bigArray));
        checkSearchPerformance(targets, new BinarySearch(bigArray));
        checkSearchPerformance(targets, new TernerySearch(bigArray));
    }

    private static void checkSearchPerformance(int[] targets, Searcher searcher) {
        System.out.println("---------------------------------");
        Instant start0 = Instant.now();
        for (int target : targets) {
            // Instant start = Instant.now();
            searcher.findNumber(target);
            // System.out.println(String.format("finding %d, result=%s", target, searcher.findNumber(target)));
            // Instant end = Instant.now();
            // Duration cost = Duration.between(start, end);
            // System.out.println(String.format("costs %,d nanoseconds", cost.toNanos()));
        }
        Instant end0 = Instant.now();
        System.out.println(String.format("costs %,d nanoseconds", Duration.between(start0, end0).toNanos()));
    }

    // List and its implementation, ArrayList, LinkedList
    // List<String> strings = new ArrayList<String>();
    // Map and its implementation, HashMap, HashTable
}
