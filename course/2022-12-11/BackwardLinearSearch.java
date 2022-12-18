public class BackwardLinearSearch implements Searcher {
    private int[] array;

    public BackwardLinearSearch(int[] array) {
        this.array = array;
    }

    @Override
    public SearchResult findNumber(int target) {
        
        // System.out.println("Searching....");
        for (int i = array.length-1; i >= 0; --i) {
            // System.out.println(i);
            if (array[i] == target) {
                return new SearchResult(array[i]);
            }
        }
        return new SearchResult();
    }
}
