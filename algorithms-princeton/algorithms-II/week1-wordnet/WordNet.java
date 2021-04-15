import edu.princeton.cs.algs4.Digraph;
import edu.princeton.cs.algs4.ST;
import edu.princeton.cs.algs4.SET;

import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;

import java.util.Iterator;

public class WordNet {

    private static class Synset {
        private final int id;
        private final String[] words;
        private final String gloss;

        public Synset(int id, String[] words, String gloss) {
            this.id = id;
            this.words = words;
            this.gloss = gloss;
        }

        public String toString() {
            String s = "id:"+this.id+",words:";
            for (int i = 0; i < this.words.length; ++i) {
                s += this.words[i];
                if (i != this.words.length-1) s += "|";
            }
            s += ",def:"+this.gloss;
            return s;
        }
    }

    private ST<Integer, Synset> synsets;
    private ST<String, SET<Integer> > glossary;
    private Digraph hypernyms;
    private SAP sap;

    public WordNet(String synsets, String hypernyms) {
        readSynsets(synsets);
        readHypernyms(hypernyms);
        sap = new SAP(this.hypernyms);
    }

    public Iterable<String> nouns() {
        return this.glossary.keys();
    }

    public boolean isNoun(String word) {
        return this.glossary.contains(word);
    }

    public int distance(String nounA, String nounB) {
        SET<Integer> idA = this.glossary.get(nounA);
        SET<Integer> idB = this.glossary.get(nounB);
        if (idA != null && idB != null) {
            return this.sap.length(idA, idB);
        } else {
            return -1;
        }
    }

    public String sap(String nounA, String nounB) {
        SET<Integer> idA = this.glossary.get(nounA);
        SET<Integer> idB = this.glossary.get(nounB);
        if (idA != null && idB != null) {
            String[] words =
                this.synsets.get(this.sap.ancestor(idA, idB)).words;
            if (words == null || words.length == 0) {
                return null;
            } else {
                return words[0];
            }
        } else {
            return null;
        } 
    }

    // input data is in the format of id, list of words, definition
    // each id might have multiple words and each word might associate
    // with multiple ids. But each id is mapped to a unique definition
    private void readSynsets(String synsets) {
        if (synsets == null) {
            throw new java.lang.NullPointerException("Synsets NULL file");
        }

        In in = new In(synsets);
        this.synsets = new ST<Integer, Synset>();
        this.glossary = new ST<String, SET<Integer> >();
    
        while (!in.isEmpty()) {
            String line = in.readLine();
            String[] fields = line.split(",", 3);
            if (fields.length < 3) { 
                throw new UnsupportedOperationException("fields count NOT 3");
            }
            int id = Integer.parseInt(fields[0]);

            String[] words = fields[1].split("\\s+");
            for (int i = 0; i < words.length; ++i) {
                SET<Integer> idsFound = this.glossary.get(words[i]);
                if (idsFound == null) {
                    SET<Integer> ids = new SET<Integer>();
                    ids.add(id);
                    this.glossary.put(words[i], ids);
                } else {
                    idsFound.add(id);
                }
            }

            this.synsets.put(id, new Synset(id, words, fields[2]));
        }
    }

    // build graph to represent hypernym relationship
    private void readHypernyms(String hypernyms) {
        if (hypernyms == null) {
            throw new java.lang.NullPointerException("hypernyms NULL file");
        }

        In in = new In(hypernyms);
        if (this.synsets == null) {
            throw new NullPointerException("Synsets is NULL");
        }
        this.hypernyms = new Digraph(this.synsets.size());
        
        while (!in.isEmpty()) {
            String[] numbers = in.readLine().split(",");
            if (numbers.length < 2) continue;

            // check validity of the synset id
            int synsetId = Integer.parseInt(numbers[0]);
            if (!this.synsets.contains(synsetId)) {
                throw new UnsupportedOperationException(
                        "Synsets does NOT contain "+numbers[0]);
            }
             
            for (int i = 1; i < numbers.length; ++i) {
 
                // check validity of the hypernym ids
                int hypernymId = Integer.parseInt(numbers[i]);
                if (!this.synsets.contains(hypernymId)) {
                    throw new UnsupportedOperationException(
                            "Synsets does NOT contain "+numbers[i]);
                }
 
                this.hypernyms.addEdge(synsetId, hypernymId);
            }
        }
    }

    private void printSynsets() {
        Iterable<Integer> keys = this.synsets.keys();
        for (Integer k : keys) StdOut.println(k+" "+this.synsets.get(k));

    }

    private void printGlossary() {
        Iterable<String> words = this.glossary.keys();
        for (String w : words) StdOut.println(w+" "+this.glossary.get(w));
    }

    private void printHypernyms() {
        StdOut.println(this.hypernyms);
    }

    public static void main(String[] args) {
        // throw new UnsupportedOperationException();
        WordNet wn = new WordNet(args[0], args[1]);
        wn.printSynsets();
        wn.printGlossary();
        wn.printHypernyms();
    }
}
