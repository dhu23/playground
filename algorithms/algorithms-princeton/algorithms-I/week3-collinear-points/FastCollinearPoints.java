import java.util.Arrays;

import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

import edu.princeton.cs.algs4.StdDraw;

public class FastCollinearPoints {

    private LineSegment[] segments_;
    private int numOfSegments_;
    private Point[] points_;

    public FastCollinearPoints(Point[] points) { 
            
        verifyInputPoints(points);

        numOfSegments_ = 0;
        segments_ = new LineSegment[2];

        for (int i = 0; i < points.length; ++i) { 
            // treat i as the origin, sort the points_ array based on slope
            Arrays.sort(points_);
            Arrays.sort(points_, points[i].slopeOrder());
            
            // printPointsInternal();
            // printPointsInternal(points[i]);

            LineSegment[] results = checkSortedAdjacentPoints(points[i]);
            
            for (int j = 0; j < results.length; ++j) {
                // segments_[numOfSegments_++] = results[j];
                addLineSegment(results[j]);
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

    private void printPointsInternal() {
        String info = "Points:";
        for (int i = 0; i < points_.length; ++i) {
            info += points_[i].toString();
        }
        StdOut.println(info);
    }

    private void printPointsInternal(Point p) {
        String info = "Slopes:";
        for (int i = 0; i < points_.length; ++i) {
            info += p.slopeTo(points_[i])+"|";
        }
        StdOut.println(info);
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

    private LineSegment[] checkSortedAdjacentPoints(Point p) {
        int len = points_.length;
        LineSegment[] segments = new LineSegment[(len-1)/3];
        int segPos = 0;
        for (int i = 0; i < len-2; ++i) {
            
            int j = i; 
            double slope = points_[0].slopeTo(points_[j]);
            while (++j < len) {
                if (points_[j].compareTo(points_[j-1]) < 0) {
                    break;
                }
                if (slope != points_[0].slopeTo(points_[j])) {
                    break;
                }
            }

            if (j-i >= 3 && points_[0].compareTo(points_[i]) < 0) {
                /*
                StdOut.println(
                        "***** s="+i+" e="+(j-1)+" o="+points_[0].toString());
                */
                segments[segPos++] = new LineSegment(points_[0], points_[j-1]);
            }
            i = j-1;
        }
        LineSegment[] results = new LineSegment[segPos];
        for (int i = 0; i < segPos; ++i) {
            results[i] = segments[i];
        }
        return results;
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

        FastCollinearPoints fcp = new FastCollinearPoints(points);
        StdOut.println(fcp.numberOfSegments());
        LineSegment[] segments = fcp.segments();
        StdOut.println("Segments:");
        for (int i = 0; i < segments.length; ++i) { 
            StdOut.println(segments[i].toString());
        }
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

        FastCollinearPoints fcp = new FastCollinearPoints(ps);
        int numOfSeg = fcp.numberOfSegments();
        LineSegment[] segments = fcp.segments();
        StdOut.println(numOfSeg);
        for (int i = 0; i < numOfSeg; ++i) {
            segments[i].draw();
            StdOut.println(segments[i].toString());
        }
    }
}
