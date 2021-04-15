import java.util.Iterator;

import edu.princeton.cs.algs4.Point2D;
import edu.princeton.cs.algs4.RectHV;

import edu.princeton.cs.algs4.Stack;
// import edu.princeton.cs.algs4.

import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdDraw;

public class KdTree {

    private static class Node {
        private Point2D point;
        private Node leftOrBottom;
        private Node rightOrTop;
        private RectHV containingRect;
        private boolean vertical;
        private int size;

        public Node(
                Point2D point, Node leftOrBottom, Node rightOrTop,
                RectHV rect, boolean vertical, int size) {
            this.point = point;
            this.leftOrBottom = leftOrBottom;
            this.rightOrTop = rightOrTop;
            this.containingRect = rect;
            this.vertical = vertical;
            this.size = size;
        }
    }

    private Node root;

    public KdTree() {
        this.root = null;
    }

    public boolean isEmpty() {return null == this.root;}

    public int size() {return size(this.root);}

    public void insert(Point2D p) {
        this.root = put(p, this.root, new RectHV(0.0, 0.0, 1.0, 1.0), true);
        // printInternal(this.root);
    }

    public boolean contains(Point2D p) {
        return null != search(p, this.root);
    }

    public void draw() {
        StdDraw.clear();
        StdDraw.setXscale(0, 1.0);
        StdDraw.setYscale(0, 1.0);
        draw(this.root);
    }

    public Iterable<Point2D> range(RectHV rect) {
        return range(this.root, rect);       
    }

    public Point2D nearest(Point2D p) {
        return nearest(p, this.root, Double.POSITIVE_INFINITY);
    }

    private int size(Node n) {
        if (null == n) return 0;
        return n.size;
    }

    private Node search(Point2D p, Node n) {
        if (null == n) return null;
        else {
            Point2D np = n.point;
            if (np.equals(p)) {
                return n;
            } else if (n.vertical) {
                // if it is vertical node, use x-coordinate
                if (p.x() < np.x()) return search(p, n.leftOrBottom);
                else                return search(p, n.rightOrTop);
            } else {
                // if it is horizontal node, use y-coordinate
                if (p.y() < np.y()) return search(p, n.leftOrBottom);
                else                return search(p, n.rightOrTop);
            }
        }
    }

    private Node put(Point2D p, Node n, RectHV rect, boolean isVertical) {
        if (null == n) {
            // StdOut.println("Inserting to an empty tree");
            return new Node(p, null, null, rect, isVertical, 1);
        } else {
            Point2D np = n.point;
            RectHV nrect = n.containingRect;
            if (np.equals(p)) {
                // not inserted, just return the node that has the point p
                // StdOut.println("Item is already there, NOT inserted");
                return n;
            } else if (n.vertical) {
                // if it is vertical node, use x-coordinate
                // StdOut.println("vertical node. Use X-coordinate");
                if (p.x() < np.x()) {
                    RectHV containingRect = new RectHV(
                            nrect.xmin(), nrect.ymin(), np.x(), nrect.ymax());
                    n.leftOrBottom = put(
                            p, n.leftOrBottom, containingRect, !isVertical);
                } else {
                    RectHV containingRect = new RectHV(
                            np.x(), nrect.ymin(), nrect.xmax(), nrect.ymax());
                    n.rightOrTop = put(
                            p, n.rightOrTop, containingRect, !isVertical);
                }
            } else {
                // if it is horizontal node, use y-coordinate
                // StdOut.println("horizontal node. Use Y-coordinate");
                if( p.y() < np.y()) {
                    RectHV containingRect = new RectHV(
                            nrect.xmin(), nrect.ymin(), nrect.xmax(), np.y());
                    n.leftOrBottom = put(
                            p, n.leftOrBottom, containingRect, !isVertical);
                } else {
                    RectHV containingRect = new RectHV(
                            nrect.xmin(), np.y(), nrect.xmax(), nrect.ymax());
                    n.rightOrTop = put(
                            p, n.rightOrTop, containingRect, !isVertical);
                }
            }
        }
        n.size = size(n.leftOrBottom)+size(n.rightOrTop)+1;
        return n;
    }


    private void draw(Node n) {
        StdOut.println("size:"+size());

        if (null == n) return;
        
        // draw the point
        RectHV nrect = n.containingRect;
        Point2D np = n.point;
        if (n.vertical) {
            StdDraw.setPenRadius(0.005);
            StdDraw.setPenColor(StdDraw.RED);
            StdDraw.line(np.x(), nrect.ymin(), np.x(), nrect.ymax()); 
        } else {
            StdDraw.setPenRadius(0.005);
            StdDraw.setPenColor(StdDraw.BLUE);
            StdDraw.line(nrect.xmin(), np.y(), nrect.xmax(), np.y());
        }
        StdDraw.setPenRadius(0.01);
        StdDraw.setPenColor(StdDraw.BLACK);
        StdDraw.point(np.x(), np.y());

        // draw sub-trees
        draw(n.leftOrBottom);
        draw(n.rightOrTop);
    }

    private Iterable<Point2D> range(Node n, RectHV rect) {
        Stack<Point2D> s = new Stack<Point2D>();
        if (null == n) return s;
        if (!rect.intersects(n.containingRect)) return s;

        if (rect.contains(n.point)) s.push(n.point);
        
        for (Point2D p: range(n.leftOrBottom, rect)) s.push(p);
        for (Point2D p: range(n.rightOrTop, rect)) s.push(p);

        return s;
    }

    private Point2D nearest(Point2D p, Node n, double minDis) {
        if (null == n) return null;
        
        if (n.containingRect.distanceTo(p) >= minDis) return null;

        double currMinDis = n.point.distanceTo(p);
        Point2D nearestPoint = n.point;
        
        Point2D nearestLeftOrBottom = nearest(p, n.leftOrBottom, currMinDis);
        if (nearestLeftOrBottom != null) {
            double minDisLeftOrBottom = nearestLeftOrBottom.distanceTo(p);
            if (minDisLeftOrBottom < currMinDis) {
                currMinDis = minDisLeftOrBottom;
                nearestPoint = nearestLeftOrBottom;
            }
        }
        Point2D nearestRightOrTop = nearest(p, n.rightOrTop, currMinDis); 
        if (nearestRightOrTop != null) {
            double minDisRightOrTop = nearestRightOrTop.distanceTo(p);
            if (minDisRightOrTop < currMinDis) {
                currMinDis = minDisRightOrTop;
                nearestPoint = nearestRightOrTop;
            }
        }
        return nearestPoint;
    }

    private void printInternal(Node n) {
        if (null == n) {
            return;
        }

        StdOut.println(n.point);
        printInternal(n.leftOrBottom);
        printInternal(n.rightOrTop);
    }

    public static void main(String[] args) {

    }

}
