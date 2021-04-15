import edu.princeton.cs.algs4.FlowEdge;
import edu.princeton.cs.algs4.FlowNetwork;
import edu.princeton.cs.algs4.FordFulkerson;
import edu.princeton.cs.algs4.ST;
import edu.princeton.cs.algs4.Stack;

import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;

public class BaseballElimination {

    private final ST<String, Integer> teams;
    private final int[] wins;
    private final int[] losses;
    private final int[] remaining;
    private final int[][] againstMatrix;

    private class FordFulkersonInfo {
        private FordFulkerson fordFulkerson;
        private FlowNetwork flowNetwork;
        private int source;
        private int sink;

        public FordFulkersonInfo(FlowNetwork fn, int s, int t) {
            this.flowNetwork = fn;
            this.source = s;
            this.sink = t;
            this.fordFulkerson = new FordFulkerson(fn, s, t);
        }
    }

    public BaseballElimination(String filename) {
        In in = new In(filename);

        int count = in.readInt();
        this.teams = new ST<String, Integer>();
        this.wins = new int[count];
        this.losses = new int[count];
        this.remaining = new int[count];
        this.againstMatrix = new int[count][count];

        int id = 0;
        while (!in.isEmpty()) {
            this.teams.put(in.readString(), id);
            this.wins[id] = in.readInt();
            this.losses[id] = in.readInt();
            this.remaining[id] = in.readInt();
            for (int i = 0; i < count; ++i) {
                this.againstMatrix[id][i] = in.readInt();
            }
            ++id;
        }
    }

    public int numberOfTeams() {
        return this.teams.size();
    }

    public Iterable<String> teams() {
        return this.teams.keys();
    }

    public int wins(String team) {
        return this.wins[this.id(team)];
    }

    public int losses(String team) {
        return this.losses[this.id(team)];
    }

    public int remaining(String team) {
        return this.remaining[this.id(team)];
    }

    public int against(String team1, String team2) {
        return this.againstMatrix[this.id(team1)][this.id(team2)];
    }

    public boolean isEliminated(String team) {
        if (!this.checkTrivialElimination(team).isEmpty()) {
            return true;
        }

        FordFulkersonInfo ffi = this.doFordFulkerson(team);
        // if all edges coming out from the source are full, then 
        // the team is not eliminated
        for (FlowEdge e : ffi.flowNetwork.adj(ffi.source)) {
            if (e.flow() < e.capacity()) return true;
        }
        return false;
    }

    public Iterable<String> certificateOfElimination(String team) {
        Stack<String> s = this.checkTrivialElimination(team);
        if (!s.isEmpty()) {
            StdOut.println("Trivial Elimination");
            return s;
        }

        FordFulkersonInfo ffi = this.doFordFulkerson(team);
        for (String t : this.teams()) {
            if (t == team) continue;
            if (ffi.fordFulkerson.inCut(this.id(t))) s.push(t);
        }
        return s;
    }

    private Stack<String> checkTrivialElimination(String team) {
        // check if the team is eliminated trivially 
        Stack<String> s = new Stack<String>();
        for (String opponent : this.teams()) {
            if (opponent == team) continue;
            if (this.wins(team)+this.remaining(team) < this.wins(opponent)) {
                s.push(opponent);
            }
        }
        return s;
    }

    private int id(String team) {
        if (team == null || !this.teams.contains(team)) {
            throw new java.lang.IllegalArgumentException("NO such team");
        }
        return this.teams.get(team);
    }
    
    private FordFulkersonInfo doFordFulkerson(String team) {
        int n = this.numberOfTeams();
        // 1+1+(n*n-n)/2-(n-1)+(n-1)
        int vertexCount = 1 + 1 + (n*n-n)/2;
        FlowNetwork fn = new FlowNetwork(vertexCount);

        // construction of the edges in the network. The process is game
        // driven. Teams are from 0 to (n-1), where teamId is used for
        // s. t is n. Games start from n+1 and ends with vertexCount-1
        int teamId = this.id(team);
        int s = teamId;
        int t = n;
        int gameId = n+1;
        int potentialWins = this.wins(team)+this.remaining(team);
        for (int i = 0; i < n; ++i) {
            
            if (i == teamId) continue;
            
            for (int j = i+1; j < n; ++j) {
                if (j == teamId) continue;
                // this is game between team i and j
                fn.addEdge(new FlowEdge(s, gameId, this.againstMatrix[i][j]));
                fn.addEdge(new FlowEdge(gameId, i, Double.POSITIVE_INFINITY));
                fn.addEdge(new FlowEdge(gameId, j, Double.POSITIVE_INFINITY));
                ++gameId;
            }
            fn.addEdge(new FlowEdge(i, t, potentialWins-this.wins[i]));
        }
        return new FordFulkersonInfo(fn, s, t);
    }
    
    public static void main(String[] args) {
        BaseballElimination division = new BaseballElimination(args[0]);
        
        /*
        for (String team : division.teams()) {
            StdOut.println(
                    team + " " + division.wins(team) + " " + 
                    division.losses(team) + " " + division.remaining(team));
            for (String t : division.teams()) {
                StdOut.println(
                        "against " + t + " " + division.against(team, t));
            }
        }
        */
        // StdOut.println(division.doFordFulkerson("Columbia").flowNetwork);
        
        for (String team : division.teams()) {
            if (division.isEliminated(team)) {
                StdOut.print(team + " is eliminated by the subset R = { ");
                for (String t : division.certificateOfElimination(team)) {
                    StdOut.print(t + " ");
                }
                StdOut.println("}");
            } else {
                // StdOut.println(team + " is not eliminated");
            }
        }
    }
}
