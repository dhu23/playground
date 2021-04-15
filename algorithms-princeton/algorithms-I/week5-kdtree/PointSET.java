import java.util.Iterator;

import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.RectHV;

import edu.princeton.cs.algs4.Stack;
import edu.princeton.cs.algs4.SET;

import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdDraw;

public class PointSET {

    private SET<Point2D> points;

    public PointSET() {
        this.points = new SET<Point2D>();
    }

    public boolean isEmpty() {return 0 == size();}

    public int size() {return this.points.size();}

    public void insert(Point2D p) {
        if (!contains(p)) {this.points.add(p);}
    }

    public boolean contains(Point2D p) {
        verifyPoint2D(p);
        return this.points.contains(p);
    }
    
    public void draw() {
        Iterator<Point2D> itr = iterator();
        while (itr.hasNext()) {
            itr.next().draw();
        }
    }

    public Iterable<Point2D> range(RectHV rect) {
        Iterator<Point2D> itr = iterator();
        Stack<Point2D> r = new Stack<Point2D>();
        while (itr.hasNext()) {
            Point2D pp = itr.next();
            if (rect.contains(pp)) {r.push(pp);}
        }
        return r;
    }

    public Point2D nearest(Point2D p) {
        Iterator<Point2D> itr = iterator();
        double nearestDistance = Double.POSITIVE_INFINITY;
        Point2D nearestPoint = null;
        while (itr.hasNext()) {
            Point2D pp = itr.next();
            double dis = p.distanceTo(pp);
            if (dis < nearestDistance) {
                nearestDistance = dis;
                nearestPoint = pp;
            }
        }
        return nearestPoint;
    }

    private void verifyPoint2D(Point2D p) {
        if (null == p) throw new java.lang.NullPointerException("Null Point2D");
    }

    private Iterator<Point2D> iterator() {return this.points.iterator();}

    public static void main(String[] args) {
        In in = new In(args[0]);

        StdDraw.clear();
        StdDraw.setPenRadius(0.01);
        StdDraw.setPenColor(StdDraw.BLACK);
        StdDraw.setXscale(0, 1.00);
        StdDraw.setYscale(0, 1.00);

        PointSET s = new PointSET();
        while (!in.isEmpty()) {
            s.insert(new Point2D(in.readDouble(), in.readDouble()));
        }
        
        s.draw();
    }
}
