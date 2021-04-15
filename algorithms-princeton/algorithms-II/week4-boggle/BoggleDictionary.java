
public class BoggleDictionary {

    private static final int R = 26;

    public static class Node {
        public boolean isWord;
        private Node[] next = new Node[R];
    }

    private Node root = new Node();

    public BoggleDictionary(String[] dictionary) {
        
        // add dictionary words into the 26-way Trie
        for (String word : dictionary) {
            this.root = this.put(this.root, word, 0);
        }
    }
 
    // put the d-th character of string key 'under/below' node x
    private Node put(Node x, String key, int d) {
        if (x == null) x = new Node();
        if (d == key.length()) {
            x.isWord = true;
            return x;
        }
        char c = key.charAt(d);
        x.next[c-'A'] = this.put(x.next[c-'A'], key, d+1);
        return x;
    }
    // put(root, "SEA", 0);  --> creates Node-S
    // put(Node-S, "SEA", 1); --> creates Node-E
    // put(Node-E, "SEA", 2); --> creates Node-A
    // put(Node-A", SEA", 3); --> market Node-A as isWord

    // get the d-th character of string key at Node x
    private Node get(Node x, String key, int d) {
        if (x == null) return null;
        if (d == key.length()) return x;
        char c = key.charAt(d);
        return get(x.next[c-'A'], key, d+1);
    }

    public Node get(Node x, char c) {
        if (x == null) return null;
        return x.next[c-'A'];
    }

    public Node getPrefix(String word) {
        return this.get(this.root, word, 0);
    }
}
