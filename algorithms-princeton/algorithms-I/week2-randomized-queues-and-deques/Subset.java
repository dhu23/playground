import java.util.Iterator;

import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;

public class Subset
{
    private static void solution1(int k)
    {
        RandomizedQueue<String> q = new RandomizedQueue<String>();

        String input;
        double threshold = 1.0*k/(k+1);
        StdOut.println(threshold);
        while(!StdIn.isEmpty())
        {
            input = StdIn.readString();
            if(StdRandom.bernoulli(threshold))
            {    
                q.dequeue();
                q.enqueue(input);
            }
        }
        Iterator<String> itr = q.iterator();
        if(q.size() < k)
        {
            StdOut.println("Total input size less than "+k);
        }
        while(itr.hasNext())
        {
            StdOut.println(itr.next());
        }
    }

    private static void solution2(int k)
    {
        RandomizedQueue<String> q = new RandomizedQueue<String>();

        while(!StdIn.isEmpty())
        {
            q.enqueue(StdIn.readString());
        }
        while(q.size() > k)
        {
            q.dequeue();
        }
        Iterator<String> itr = q.iterator();
        while(itr.hasNext())
        {
            StdOut.println(itr.next());
        }
    }

    private static int verifyInput(String[] args)
    {
        if(args.length != 1)
        {
            throw new UnsupportedOperationException("Need one integer input");
        }
        return Integer.parseInt(args[0]);
    }

    public static void main(String[] args)
    {
        int k = verifyInput(args);
        solution2(k);
    }
}
