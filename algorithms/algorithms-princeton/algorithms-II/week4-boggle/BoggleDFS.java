import edu.princeton.cs.algs4.SET;

import edu.princeton.cs.algs4.StdOut;

public class BoggleDFS {

    public static class Dice {
        public int row;
        public int col;

        public Dice(int r, int c) {
            this.row = r;
            this.col = c;
        }
    }

    private boolean[][] letterMarks;
    private SET<String> words;
    private final BoggleBoard board;
    private final BoggleDictionary dict;

    public BoggleDFS(
            int row, int col, 
            BoggleBoard board, BoggleDictionary dict) {
        int rows = board.rows();
        int cols = board.cols();

        this.letterMarks = new boolean[rows][cols];
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                this.letterMarks[i][j] = false;
            }
        }

        this.words = new SET<String>();
        this.board = board;
        this.dict = dict;

        this.dfs("", new Dice(row, col));
    }

    public Iterable<String> getWords() {return this.words;}

    private void dfs(String prefix, Dice dice) {
        int rows = this.board.rows();
        int cols = this.board.cols();
                
        assert(dice.row >= 0 && dice.row < rows);
        assert(dice.col >= 0 && dice.col < cols);

        // build new prefix. "Qu" cases are covered here:
        // 1) always replace Q as QU.
        // 2) when U follows Q which is replaced by QU, it is not added
        StringBuilder prefixBuilder = new StringBuilder();
        prefixBuilder.append(prefix);

        char letter = this.board.getLetter(dice.row, dice.col);
        if (letter == 'Q') {
            prefixBuilder.append("QU");
        } else if (letter == 'U') {
            String prefixEnding = prefix.length() > 2 
                ? prefix.substring(prefix.length()-2)
                : prefix;
            if (!prefixEnding.equals("QU")) {
                prefixBuilder.append(letter);
            }
        } else {
            prefixBuilder.append(letter);
        }

        String prefixUpdated = prefixBuilder.toString();
        BoggleDictionary.Node p = this.dict.getPrefix(prefixUpdated);
        if (p == null) {
            // this prefix does not exist in the dictionary; terminate dfs
            return;
        } else if (p.isWord && prefixUpdated.length() > 2) {
            this.words.add(prefixUpdated);
        }

        // dfs to search all possible words
        this.markLetter(dice, true);        
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) continue;

                int row = dice.row+i;
                int col = dice.col+j;

                if (row < 0 || row >= this.board.rows()) continue;
                if (col < 0 || col >= this.board.cols()) continue;
                
                if (this.letterMarks[row][col]) continue;
                this.dfs(prefixUpdated, new Dice(row, col));
            }
        }
        this.markLetter(dice, false); // unmark the dice when finished
    }

    private void markLetter(Dice dice, boolean mark) {
        this.letterMarks[dice.row][dice.col] = mark;
    }
}
