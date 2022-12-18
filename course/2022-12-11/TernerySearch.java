import java.util.Arrays;

public class TernerySearch implements Searcher {
    private int[] array;

    public TernerySearch(int[] array) {
        this.array = array.clone();
        Arrays.sort(this.array);
    }

    @Override
    public SearchResult findNumber(int target) {
        return findWithinRange(target, 0, this.array.length-1);
    }

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

                // int oneThird = low + (high-low) / 3;
                // if (target == this.array[oneThird]) {
                //     return new SearchResult(target);
                // } else if (target < this.array[oneThird]) {
                //     // 1st third
                //     return findWithinRange(target, low, oneThird);
                // } else {
                //     int twoThird = low + 2* (high-low) / 3;
                //     if (target == this.array[twoThird]) {
                //         return new SearchResult(target);
                //     } else if (target < this.array[twoThird]) {
                //         return findWithinRange(target, oneThird+1, twoThird);
                //     } else {
                //         return findWithinRange(target, twoThird+1, high);
                //     }
                // }

                int segsize = (high-low)/3;
                int oneThird = low + segsize;
                int twoThird = oneThird + segsize;
                if (target == array[oneThird]) {
                    return new SearchResult(target);
                } else if (target == array[twoThird]) {
                    return new SearchResult(target);
                } else if (target < array[oneThird]) { 
                    return findWithinRange(target, low, oneThird);
                } else if (target > array[twoThird]) {
                    return findWithinRange(target, twoThird+1, high);
                } else {
                    return findWithinRange(target, oneThird+1, twoThird);
                }
            }
            
        }
    }
}
