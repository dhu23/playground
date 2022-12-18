// given a list of unique numbers, check if a number is in the list
// [1, 4, 5, 2, 11, 100, 87]
//
// function signature
import java.lang.Thread;
import java.util.List;
import java.util.ArrayList;

public class SearchMain {

    // signature
    //    input String[],
    //    its name

    //    output void
    public static void main(String[] args) {
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
    }

}
