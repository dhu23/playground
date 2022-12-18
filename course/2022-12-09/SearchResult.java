public class SearchResult {
    public int result;
    public boolean found;

    // constructor
    // overloading = using the same name but different signatures
    
    public SearchResult() {
        result = 0;
        found = false;
    }

    public SearchResult(int val) {
        result = val;
        found = true;
    }

    public String toString() {
        if (found) {
            return String.format("Found(%s)", String.valueOf(result));
        } else {
            return "NotFound";
        }
    }
}
