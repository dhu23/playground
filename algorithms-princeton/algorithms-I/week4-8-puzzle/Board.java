import edu.princeton.cs.algs4.Stack;

public class Board {
    
    private final int dimension_;
    private final char[] blocks_;

    public Board(int[][] blocks) {
        if (blocks.length < 2) {
            throw new UnsupportedOperationException("Block dim less than 2");
        }

        dimension_ = blocks.length;
        assert dimension_ == blocks[0].length;
        blocks_ = new char[dimension_*blocks[0].length];

        int index = 0;
        for (int i = 0; i < dimension_; ++i) {
            assert dimension_ == blocks[i].length;
            for (int j = 0; j < blocks[i].length; ++j) {
                blocks_[index++] = (char)blocks[i][j];
            }
        }
    }
    
    // private constructor for internal use
    private Board(char[] blocks, int dim) {
        this.blocks_ = blocks;
        this.dimension_ = dim;
    }

    public int dimension() {
        return this.dimension_;
    }

    public int hamming() {
        int hamming = 0;
        for (int i = 0; i < blocks_.length; ++i) {
            if (blocks_[i] != 0 && blocks_[i] != i+1) {
                ++hamming;
            }
        }
        return hamming;
    }

    public int manhattan() {
        int manhattan = 0;
        for (int i = 0; i < blocks_.length; ++i) {
            if (blocks_[i] != 0 && blocks_[i] != i+1) {
                int col = col(i, dimension_);
                int row = row(i, dimension_);

                int targetCol = col(blocks_[i]-1, dimension_);
                int targetRow = row(blocks_[i]-1, dimension_);

                manhattan += col > targetCol ? col-targetCol : targetCol-col;
                manhattan += row > targetRow ? row-targetRow : targetRow-row;
            }
        }
        return manhattan;
    }

    public boolean isGoal() {
        return this.hamming() == 0;
    }

    // give a twin
    public Board twin() {
        int i = 0;
        while (blocks_[i] == 0) ++i;
        int j = i+1;
        while (blocks_[j] == 0) ++j;
        
        char[] temp = swapBlock(i, j);
        return new Board(temp, dimension_);
    }

    public boolean equals(Object y) {
        if (y == this) return true;
        if (y == null) return false;
        if (y.getClass() != this.getClass()) return false;
        Board that = (Board) y;
        if (that.dimension_ != this.dimension_) return false;
        
        // this.printInternal();
        // that.printInternal();
    
        int len = this.blocks_.length;
        for (int i = 0; i < len; ++i) {
            if (that.blocks_[i] != this.blocks_[i]) return false;
        }
        return true;
    }

    public Iterable<Board> neighbors() {
        int zeroPos = 0;
        while (blocks_[zeroPos] != 0) ++zeroPos;
        
        int zeroCol = col(zeroPos, dimension_);
        int zeroRow = row(zeroPos, dimension_);

        Stack<Board> s = new Stack<Board>();
        try {
            int swapPos = pos(zeroRow+1, zeroCol, dimension_);
            s.push(new Board(swapBlock(swapPos, zeroPos), dimension_));
        } catch (IndexOutOfBoundsException e) {
            // do nothing
        }

        try {
            int swapPos = pos(zeroRow-1, zeroCol, dimension_);
            s.push(new Board(swapBlock(swapPos, zeroPos), dimension_));
        } catch (IndexOutOfBoundsException e) {
            // do nothing
        }

        try {
            int swapPos = pos(zeroRow, zeroCol+1, dimension_);
            s.push(new Board(swapBlock(swapPos, zeroPos), dimension_));
        } catch (IndexOutOfBoundsException e) {
            // do nothing
        }

        try {
            int swapPos = pos(zeroRow, zeroCol-1, dimension_);
            s.push(new Board(swapBlock(swapPos, zeroPos), dimension_));
        } catch (IndexOutOfBoundsException e) {
            // do nothing
        }
        return s;
    }

    public String toString() {
        StringBuilder s = new StringBuilder();
        s.append(dimension_+"\n");
        for (int i = 0; i < blocks_.length; ++i) {
            s.append(String.format("%2d ", (int)(this.blocks_[i])));
            if ((i+1)%dimension_ == 0) {
                s.append("\n");
            }
        }
        return s.toString();
    }

    private static int row(int arrayPos, int dim) {
        return arrayPos/dim; // from 0 to dim-1
    }

    private static int col(int arrayPos, int dim) {
        return arrayPos%dim; // from 0 to dim-1
    }

    // row is between 0 to dim-1, col is between 0 to dim-1
    private static int pos(int row, int col, int dim) {
        if (row < 0 || row >= dim || col < 0 || col >= dim) {
            throw new IndexOutOfBoundsException();
        }
        return row*dim+col;
    }

    private char[] swapBlock(int pos1, int pos2) {
        int len = blocks_.length;
        if (pos1 < 0 || pos1 >= len || pos2 < 0 || pos2 >= len) {
            throw new IndexOutOfBoundsException();
        }

        char[] temp = new char[len];
        for (int i = 0; i < len; ++i) {
            if (i == pos1) temp[i] = blocks_[pos2];
            else if (i == pos2) temp[i] = blocks_[pos1];
            else temp[i] = blocks_[i];
        }
        return temp;
    }

    private void printInternal() {
        String info = "Internal:";
        for (int i = 0; i < blocks_.length; ++i) {
            info += (int)(blocks_[i]);
        }
        System.out.println(info);
    }

    private static void test1() {
        int[][] table = {{8, 1, 3}, {4, 2, 0}, {7, 6, 5}};
        Board b = new Board(table);
        System.out.println(b.toString());
        System.out.println(b.hamming());
        System.out.println(b.manhattan());

        Board bt = b.twin();
        System.out.println(bt.toString());

        for (Board i : b.neighbors()) System.out.println(i);
    }

    public static void main(String[] args) {
        test1();
    }

}
