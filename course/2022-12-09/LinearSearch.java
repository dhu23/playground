
public class LinearSearch implements Searcher {
    private int[] array;

    public LinearSearch(int[] array) {
        this.array = array;
    }

    @Override
    public SearchResult findNumber(int target) {
        for (int i : array) {
            if (i == target) {
                return new SearchResult(i);
            }
        }
        return new SearchResult();
    }
}
