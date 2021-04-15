
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;

public class Outcast {

    private WordNet wordnet;

    public Outcast(WordNet wordnet) {
        this.wordnet = wordnet;    
    }

    public String outcast(String[] nouns) {
        if (nouns == null) throw new NullPointerException("NULL input");
        int len = nouns.length;
        int[][] matrix = new int[len][len];
        
        int maxDis = 0;
        String oc = null;
        for (int i = 0; i < len; ++i) {
            int dis = 0;
            for (int j = 0; j < len; ++j) {
                if (j == i) {
                    matrix[i][j] = 0;
                } else if (j < i) {
                    matrix[i][j] = matrix[j][i];
                } else { // when i < j
                    matrix[i][j] = this.wordnet.distance(nouns[i], nouns[j]);
                }

                if (matrix[i][j] == -1) dis = -1;
                if (dis != -1)          dis += matrix[i][j];
            }
            if (maxDis != -1 && (dis == -1 || dis > maxDis)) {
                maxDis = dis;
                oc = nouns[i];
            }
        }
        
        /*
        for (int i = 0; i < len; ++i) {
            String info = new String();
            for (int j = 0; j < len; ++j) {
                info += matrix[i][j]+" ";
            }
            StdOut.println(info);
        }
        */

        return oc;
    }

    public static void main(String[] args) {
        WordNet wordnet = new WordNet(args[0], args[1]);
        Outcast outcast = new Outcast(wordnet);
        for (int t = 2; t < args.length; ++t) {
            In in = new In(args[t]);
            String[] nouns = in.readAllStrings();
            StdOut.println(args[t]+":"+outcast.outcast(nouns));
        }
    }
}
