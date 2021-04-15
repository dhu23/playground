import java.awt.Color;
import edu.princeton.cs.algs4.Picture;

import edu.princeton.cs.algs4.StdOut;
// import edu.princeton.cs.algs4.Stack;


public class SeamCarver {

    private Picture picture;

    public SeamCarver(Picture picture) {
        if (picture == null) {
            throw new java.lang.NullPointerException("NULL picture input");
        }
        this.picture = new Picture(picture);
    }

    public Picture picture() {return this.picture;}

    public int width() {return this.picture.width();}

    public int height() {return this.picture.height();}

    public double energy(int x, int y) {
        if (x < 0 || x >= this.width() || y < 0 || y >= this.height()) {
            throw new java.lang.IndexOutOfBoundsException(
                    "input is OUT of boundary");
        }

        if (x == 0 || x == this.width()-1 || y == 0 || y == this.height()-1) {
            return 1000.0;
        }

        int deltaSquare = 
            calculateDeltaSquare(
                    this.picture.get(x-1, y), this.picture.get(x+1, y)) +
            calculateDeltaSquare(
                    this.picture.get(x, y-1), this.picture.get(x, y+1));
        return Math.sqrt(deltaSquare);
    }

    public int[] findHorizontalSeam() {
        // stores lowest energy from the left to (i, j)
        double[][] energies = new double[this.height()][this.width()];
        // stores the parent pixel from the last row to (i, j)
        int[][] parents = new int[this.height()][this.width()];

        for (int j = 0; j < this.width(); ++j) {
            for (int i = 0; i < this.height(); ++i) {
                double e = this.energy(j, i);
                // initialize the left column
                if (j == 0) {
                    energies[i][j] = e;
                    parents[i][j] = -999;
                } else {
                    // compute lowest energy up to this point from 
                    // (i-1, j-1), (i, j-1), (i+1, j-1)
                    double eMin = energies[i][j-1];
                    int posMin = i;
                    // when top left point exist
                    if (i > 0 && energies[i-1][j-1] < eMin) {
                        eMin = energies[i-1][j-1];
                        posMin = i-1;
                    }
                    // when bottom left point exist
                    if (i < this.height()-1 && energies[i+1][j-1] < eMin) {
                        eMin = energies[i+1][j-1];
                        posMin = i+1;
                    }

                    energies[i][j] = e+eMin;
                    parents[i][j] = posMin;
                }
            }
        }

        // go through the last row of the energies matrix and find the min
        double lowest = energies[0][this.width()-1];
        int lowestPos = 0;
        for (int i = 1; i < this.height()-1; ++i) {
            double e = energies[i][this.width()-1];
            if (e < lowest) {
                lowest = e;
                lowestPos = i;
            }
        }

        int[] seam = new int[this.width()];
        seam[this.width()-1] = lowestPos;
        for (int j = this.width()-1; j > 0; --j) {
            seam[j-1] = parents[seam[j]][j];
        }

        return seam;
    }

    public int[] findVerticalSeam() {
        // stores lowest energy from the top to (i, j)
        double[][] energies = new double[this.height()][this.width()];
        // stores the parent pixel from the last row to (i, j)
        int[][] parents = new int[this.height()][this.width()];

        for (int i = 0; i < this.height(); ++i) {
            for (int j = 0; j < this.width(); ++j) {
                double e = this.energy(j, i); // (x, y) <-> (j, i)
                // initialize the top row
                if (i == 0) {
                    energies[i][j] = e; 
                    parents[i][j] = -999;
                } else {
                    // compute lowest energy up to this point from
                    // (i-1, j-1), (i-1, j) and (i-1, j+1)
                    double eMin = energies[i-1][j];
                    int posMin = j;
                    // when top left point exist
                    if (j > 0 && energies[i-1][j-1] < eMin) {
                        eMin = energies[i-1][j-1];
                        posMin = j-1;
                    }
                    // when top right point exist
                    if (j < this.width()-1 && energies[i-1][j+1] < eMin) {
                        eMin = energies[i-1][j+1];
                        posMin = j+1;
                    }

                    energies[i][j] = e+eMin;
                    parents[i][j] = posMin;
                }
            }
        }

        // go through the last row of the energies matrix and find the min
        double lowest = energies[this.height()-1][0];
        int lowestPos = 0;
        for (int j = 1; j < this.width(); ++j) {
            double e = energies[this.height()-1][j];
            if (e < lowest) {
                lowest = e; 
                lowestPos = j;
            }
        }

        int[] seam = new int[this.height()];
        seam[this.height()-1] = lowestPos;
        for (int i = this.height()-1; i > 0; --i) {
            seam[i-1] = parents[i][seam[i]];
        }

        return seam;
    }

    public void removeHorizontalSeam(int[] seam) {
        if (seam == null) {
            throw new java.lang.NullPointerException("NULL horizontal seam");
        }

        Picture p = new Picture(this.width(), this.height()-1);
        for (int i = 0; i < width(); ++i) {
            int pos = 0;
            for (int j = 0; j < height(); ++j) {
                if (j == seam[i]) continue;
                p.set(i, pos++, this.picture.get(i, j));
            }
        }
        this.picture = p;
    }

    public void removeVerticalSeam(int[] seam) {
        if (seam == null) {
            throw new java.lang.NullPointerException("NULL vertical seam");
        }

        Picture p = new Picture(this.width()-1, this.height());
        for (int j = 0; j < height(); ++j) {
            int pos = 0;
            for (int i = 0; i < width(); ++i) {
                if (i == seam[j]) continue;
                p.set(pos++, j, this.picture.get(i, j));
            }
        }
        this.picture = p;
    }

    private int calculateDeltaSquare(Color c1, Color c2) {
        int deltaR = c1.getRed()-c2.getRed();
        int deltaG = c1.getGreen()-c2.getGreen();
        int deltaB = c1.getBlue()-c2.getBlue();

        return deltaR*deltaR+deltaG*deltaG+deltaB*deltaB;
    }
}
