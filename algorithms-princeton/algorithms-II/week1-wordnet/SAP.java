import edu.princeton.cs.algs4.Digraph;
import edu.princeton.cs.algs4.BreadthFirstDirectedPaths;

import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdIn;

public class SAP {

    private Digraph g;

    public SAP(Digraph G) {
        if (null == G) {
            throw new java.lang.NullPointerException("NULL graph input");
        }
        this.g = G;
    }

    public int length(int v, int w) {
        int[] paths = getAncestralPaths(v, w);
        return analyzePaths(paths, false);
    }

    public int ancestor(int v, int w) {
        int[] paths = getAncestralPaths(v, w);
        return analyzePaths(paths, true);
    }

    public int length(Iterable<Integer> v, Iterable<Integer> w) {
        int[] paths = getAncestralPaths(v, w);
        return analyzePaths(paths, false);
    }

    public int ancestor(Iterable<Integer> v, Iterable<Integer> w) {
        int[] paths = getAncestralPaths(v, w);
        return analyzePaths(paths, true);
    }

    private int[] getAncestralPaths(int v, int w) {
        int[] paths = new int[this.g.V()];

        BreadthFirstDirectedPaths bfdpV = 
            new BreadthFirstDirectedPaths(this.g, v);
        for (int i = 0; i < this.g.V(); ++i) {
            if (bfdpV.hasPathTo(i)) {
                paths[i] = bfdpV.distTo(i);
            } else {
                paths[i] = -1;
            }
        }

        BreadthFirstDirectedPaths bfdpW = 
            new BreadthFirstDirectedPaths(this.g, w);
        for (int i = 0; i < this.g.V(); ++i) {
            if (bfdpW.hasPathTo(i)) {
                paths[i] = paths[i] == -1 ? -1 : paths[i]+bfdpW.distTo(i);
            } else {
                paths[i] = -1;
            }
        }
        return paths;
    }

    private int[] getAncestralPaths(Iterable<Integer> v, Iterable<Integer> w) {
        int[] paths = new int[this.g.V()];

        BreadthFirstDirectedPaths bfdpV = 
            new BreadthFirstDirectedPaths(this.g, v);
        for (int i = 0; i < this.g.V(); ++i) {
            if (bfdpV.hasPathTo(i)) {
                paths[i] = bfdpV.distTo(i);
            } else {
                paths[i] = -1;
            }
        }

        BreadthFirstDirectedPaths bfdpW = 
            new BreadthFirstDirectedPaths(this.g, w);
        for (int i = 0; i < this.g.V(); ++i) {
            if (bfdpW.hasPathTo(i)) {
                paths[i] = paths[i] == -1 ? -1 : paths[i]+bfdpW.distTo(i);
            } else {
                paths[i] = -1;
            }
        }
        return paths;
    }

    private int analyzePaths(int[] paths, boolean isAncestor) {
        int minLength = -1;
        int anc = -1;
        for (int i = 0; i < paths.length; ++i) {
            if ((minLength == -1 || minLength > paths[i]) && paths[i] >= 0) {
                minLength = paths[i];
                anc = i;
            }
        }
        if (isAncestor) return anc;
        else            return minLength;
    }

    public static void main(String[] args) {
        In in = new In(args[0]);
        Digraph G = new Digraph(in);
        SAP sap = new SAP(G);
        while (!StdIn.isEmpty()) {
            int v = StdIn.readInt();
            int w = StdIn.readInt();
            int length = sap.length(v, w);
            int ancestor = sap.ancestor(v, w);
            StdOut.printf("length = %d, ancestor = %d\n", length, ancestor);
        }
    }
}
