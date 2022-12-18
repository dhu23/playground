import java.util.Arrays;

public class BinarySearch implements Searcher {
    private int[] array;

    public BinarySearch(int[] array) {
        this.array = array.clone();
        Arrays.sort(this.array);
    }

    @Override
    public SearchResult findNumber(int target) {
        return findWithinRange(target, 0, this.array.length-1);
    }

    // f(x) = x
    // g(x) = x^2
    // h(x) = sqrt(x) 
    // when x >> 0
    // (1, 1)
    //
    // binary log2 N ( N size of array)
    // log3 N

    // 1st compare --> 1/2
    // 2nd compare --> 1/4
    // 3rd --> 1/8
    // 4th --> 1/16
    //
    // first 2 compares --> 1/3, 1/3*1+ 1/3*1 +1/3*2 = 4/3 compares -> 1/3
    // first 4 compares --> 1/9
    //
    // 3 compares --> 1/4
    
    // it is a recurisve process to find (recursion)
    // 0                                                  63
    // 0                        31 32                     63
    // 0           15 16        31 32        47 48        63
    //

    // _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    // low                         high
    // Pseudocode:
    // goal: try to see if the target is within array[low --> high]
    // if target > high or target < low: not found (no recursion)
    // else
    //     if within first half: tell me what it is
    //     else find within the second half

    private SearchResult findWithinRange(int target, int low, int high) {
        if (low > high) {
            return new SearchResult();
        } else if (low == high) {
            if (target == this.array[low]) {
                return new SearchResult(target);
            } else {
                return new SearchResult();
            }
        } else { // low < high
            if (target > this.array[high] || target < this.array[low]) {
                return new SearchResult();
            } else if (target == this.array[high] || target == this.array[low]) {
                return new SearchResult(target);
            } else {
                int mid = (low + high) / 2;
                if (target == this.array[mid]) {
                    return new SearchResult(target);
                } else if (target < this.array[mid]) {
                    // 1st half
                    return findWithinRange(target, low, mid);
                } else {
                    // 2nd half
                    return findWithinRange(target, mid+1, high);
                }
            }
        }
    }
}

// ?logN
// idx :   0   1    2    3    4
//         5  20  100   200   400
// target: 99
