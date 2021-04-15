import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;

import edu.princeton.cs.algs4.SET;

public class BoggleSolver {

    private final BoggleDictionary dictionary;

    public BoggleSolver(String[] dictionary) {
        this.dictionary = new BoggleDictionary(dictionary);
    }

    public Iterable<String> getAllValidWords(BoggleBoard board) {
        SET<String> words = new SET<String>();
        
        for (int i = 0; i < board.rows(); ++i) {
            for (int j = 0; j < board.rows(); ++j) {
                BoggleDFS solver = 
                    new BoggleDFS(i, j, board, this.dictionary);
                for (String w : solver.getWords()) {
                    if (!words.contains(w)) words.add(w);
                }
            }
        }
        return words;
    }

    public int scoreOf(String word) {
        if (word == null) {
            throw new NullPointerException("NULL input word");
        }
        BoggleDictionary.Node w = this.dictionary.getPrefix(word);
        if (w == null) {
            throw new IllegalArgumentException(word+" NOT in dict");
        } else if (!w.isWord) {
            throw new IllegalArgumentException(word+" NOT legit word");
        } else {
            int wordLen = word.length();
            if (wordLen <= 2)          return 0;
            else if (wordLen <= 4)     return 1;
            else if (wordLen == 5)     return 2;
            else if (wordLen == 6)     return 3;
            else if (wordLen == 7)     return 5;
            else                       return 11;
        } 
    }

    public static void main(String[] args) {
        In in = new In(args[0]);
        String[] dictionary = in.readAllStrings();
        BoggleSolver solver = new BoggleSolver(dictionary);
        BoggleBoard board = new BoggleBoard(args[1]);
        int score = 0;
        for(String word : solver.getAllValidWords(board)) {
            StdOut.println(word);
            score += solver.scoreOf(word);
        }
        StdOut.println("Score = " + score);
    }
}
