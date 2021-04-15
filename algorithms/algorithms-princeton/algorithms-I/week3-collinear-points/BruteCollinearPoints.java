import java.util.Arrays;

import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

import edu.princeton.cs.algs4.StdDraw;

public class BruteCollinearPoints {

    private LineSegment[] segments_;
    private int numOfSegments_;
    private Point[] points_;

    public BruteCollinearPoints(Point[] points) {
        verifyInputPoints(points);

        // initialization of internal data
        numOfSegments_ = 0;

        // based on the requirement, at most each point can have one 
        // collinear line segment
        int len = points.length;
        segments_ = new LineSegment[2];

        numOfSegments_ = 0;
        for (int i = 0; i < len; ++i) {
            for (int j = i+1; j < len; ++j) {
                double slope12 = points_[i].slopeTo(points_[j]);

                boolean foundTheLongest = false;
                for (int k = len-1; k > j; --k) {
                    double slope23 = points_[j].slopeTo(points_[k]);

                    if (slope12 != slope23) continue;

                    for (int l = j+1; l < k; ++l) {
                        double slope34 = points_[k].slopeTo(points_[l]);
                        
                        // because the slope is calculated based of 
                        // numbers between 0 and 32767
                        if (slope12 == slope23 && slope23 == slope34) {
                            String line = points_[i].toString();
                            line += "|" + points_[j].toString();
                            line += "|" + points_[k].toString();
                            line += "|" + points_[l].toString();
                            line += "|" + slope12;
                            line += "|" + slope23;
                            line += "|" + slope34;

                            // StdOut.println(line);
                            addLineSegment(
                                    new LineSegment(points_[i], points_[k]));
                            foundTheLongest = true;
                            break;
                        }
                    }
                    if (foundTheLongest) break;
                }
            }
        }
    }

    public int numberOfSegments() {
        return numOfSegments_;
    }

    public LineSegment[] segments() {
        LineSegment[] results = new LineSegment[numOfSegments_];
        for (int i = 0; i < numOfSegments_; ++i) {
            results[i] = segments_[i];
        }
        return results;
    }

    private void verifyInputPoints(Point[] points) {
        points_ = new Point[points.length];
        for (int i = 0; i < points.length; ++i) {
            if (points[i] == null) {
                throw new java.lang.NullPointerException("Null input"); 
            }
            points_[i] = points[i];
        }
        Arrays.sort(points_);
        
        for (int i = 1; i < points_.length; ++i) {
            if (points_[i].compareTo(points_[i-1]) == 0) {
                throw new java.lang.IllegalArgumentException("Repeated points");
            }
        }
    }

    private void addLineSegment(LineSegment ls) {
        if (segments_.length == numOfSegments_) {
            LineSegment[] temp = new LineSegment[segments_.length*2];
            for (int i = 0; i < numOfSegments_; ++i) {
                temp[i] = segments_[i];
            }
            segments_ = temp;
        } 
        segments_[numOfSegments_++] = ls;
    }

    private void printInternal() {
        String sInfo = "segment";
        for (int j = 0; j < segments_.length; ++j) {
            if (segments_[j] == null) {
                sInfo += "[null]";
            } else {
                sInfo += "["+segments_[j].toString()+"]";
            }
        }
        StdOut.println(sInfo);
    }

    private static void testSimple() {
        Point[] points = new Point[10];
        points[0] = new Point(1, 1);
        points[1] = new Point(1, 2);
        points[2] = new Point(2, 1);
        points[3] = new Point(2, 2);
        points[4] = new Point(3, 2);
        points[5] = new Point(2, 3);
        points[6] = new Point(3, 3);
        points[7] = new Point(4, 2);
        points[8] = new Point(2, 4);
        points[9] = new Point(4, 4);

        BruteCollinearPoints bcp = new BruteCollinearPoints(points);
        bcp.printInternal();
        StdOut.println(bcp.numberOfSegments());
    }

    public static void main(String[] args) {
        In in = new In(args[0]);
        int count = in.readInt();

        StdDraw.clear();
        StdDraw.setPenRadius(0.001);
        StdDraw.setPenColor(StdDraw.BLUE);
        StdDraw.setXscale(0, 32767);
        StdDraw.setYscale(0, 32767);

        Point[] ps = new Point[count];
        int pos = 0;
        while (!in.isEmpty()) {
            ps[pos++] = new Point(in.readInt(), in.readInt());
        }

        BruteCollinearPoints bcp = new BruteCollinearPoints(ps);
        int numOfSeg = bcp.numberOfSegments();
        LineSegment[] segments = bcp.segments();
        StdOut.println(numOfSeg);
        for (int i = 0; i < numOfSeg; ++i) {
            segments[i].draw();
            StdOut.println(segments[i].toString());
        }
    }
}
