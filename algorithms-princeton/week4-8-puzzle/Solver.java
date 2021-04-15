import edu.princeton.cs.algs4.MinPQ;
import edu.princeton.cs.algs4.Stack;

import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;

public class Solver {

    private class SearchNode implements Comparable<SearchNode> {
        private final Board board_;
        private final int moves_;
        private final SearchNode prev_;

        public SearchNode(Board b, int moves, SearchNode prev) {
            this.board_ = b;
            this.moves_ = moves;
            this.prev_ = prev;
        }

        public SearchNode(Board b) {
            this.board_ = b;
            this.moves_ = 0; 
            this.prev_ = null;
        }

        public int compareTo(SearchNode that) {
            int thisValue = this.board_.manhattan()+this.moves_;
            int thatValue = that.board_.manhattan()+that.moves_;
            if (thisValue < thatValue) return -1;
            else if (thisValue > thatValue) return 1;
            else return 0;
        }

        public boolean equals(Object other) {
            if (other == this) return true;
            if (other == null) return false;
            if (other.getClass() != this.getClass()) return false;
            SearchNode that = (SearchNode) other;
            return this.board_.equals(that.board_);
        }
    }

    private final MinPQ<SearchNode> pq_;
    private final SearchNode achieved_;
    private final Stack<Board> solution_;

    public Solver(Board initial) {
        if (initial == null) {
            throw new java.lang.NullPointerException();
        }
        
        this.pq_ = new MinPQ<SearchNode>();
        
        SearchNode achieved = null;
        this.pq_.insert(new SearchNode(initial));
        this.pq_.insert(new SearchNode(initial.twin()));
        while (true) {
            if (this.pq_.isEmpty()) break;

            SearchNode m = this.pq_.delMin();

            if (m.board_.isGoal()) {
                achieved = m;
                
                // StdOut.println("GOAL achieved!!!!!");
                break;
            }

            for (Board b : m.board_.neighbors()) {
                if (m.prev_ != null && b.equals(m.prev_.board_)) continue;
                if (b.equals(initial)) continue;
                pq_.insert(new SearchNode(b, m.moves_+1, m));
            }
        }
        if (achieved != null) {
            this.solution_ = createSolution(achieved, initial);
            this.achieved_ = this.solution_ != null ? achieved : null;
        } else {
            this.solution_ = null;
            this.achieved_ = null;
        }
    }

    public boolean isSolvable() {
        return solution_ != null;
    }

    public int moves() {
        if (isSolvable()) return achieved_.moves_;
        else return -1;
    }

    public Iterable<Board> solution() {
        return this.solution_;
    }

    private Stack<Board> createSolution(SearchNode achieved, Board initial) {
        assert achieved != null;
        Stack<Board> s = new Stack<Board>();   
        SearchNode node = achieved;
        while (node != null) {
            s.push(node.board_);
            node = node.prev_;
        }
        if (s.peek().equals(initial)) return s;
        else return null;
    }

    public static void main(String[] args) {
        In in = new In(args[0]);
        int N = in.readInt();
        int[][] blocks = new int[N][N];
        for (int i = 0; i < N; ++i) 
            for (int j = 0; j < N; ++j)
                blocks[i][j] = in.readInt();
        Board initial = new Board(blocks);

        Solver solver = new Solver(initial);

        if(!solver.isSolvable())
            StdOut.println("No solution possible");
        else {
            StdOut.println("Minimum number of moves = " + solver.moves());
            for (Board board : solver.solution())
                StdOut.println(board);
        }   
    }
}
