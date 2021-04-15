import java.util.Iterator;
import java.util.NoSuchElementException;

import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;
import edu.princeton.cs.algs4.StdRandom;

public class RandomizedQueue<Item> implements Iterable<Item>
{
    private Item[] a_;
    private int numOfElement_;

    public RandomizedQueue()
    {
        init();
    }

    public boolean isEmpty()
    {
        return 0 == numOfElement_;
    }

    public int size()
    {
        return numOfElement_;
    }

    public void enqueue(Item item)
    {
        verifyItem(item);
        
        if(isFull())
        {
            resize(a_.length*2);
        }
        a_[numOfElement_] = item;
        numOfElement_ += 1;
    }

    public Item dequeue()
    {
        if(isEmpty())
        {
            throw new NoSuchElementException("RandomizedQueue is empty");
        }
        StdRandom.shuffle(a_, 0, numOfElement_-1);
        Item item = a_[numOfElement_-1];
        a_[numOfElement_-1] = null;
        numOfElement_ -= 1;

        if(!isEmpty() && numOfElement_ == a_.length/4)
        {
            resize(a_.length/2);
        }

        return item;
    }

    public Item sample()
    {
        if(isEmpty())
        {
            throw new NoSuchElementException("RandomizedQueue is empty");
        }

        StdRandom.shuffle(a_, 0, numOfElement_-1);
        return a_[numOfElement_-1];
    }

    public Iterator<Item> iterator()
    {
        return new RandomizedArrayIterator();
    }

    private class RandomizedArrayIterator implements Iterator<Item> 
    {
        private int[] randomizedPositions_;
        private int n_;

        public RandomizedArrayIterator()
        {
            randomizedPositions_ = new int[numOfElement_];
            for(int i = 0; i < numOfElement_; ++i)
            {
                randomizedPositions_[i] = i;
            }
            StdRandom.shuffle(randomizedPositions_);
            n_ = numOfElement_-1;
        }

        public boolean hasNext()
        {
            return n_ >= 0;
        }

        public void remove()
        {
            throw new UnsupportedOperationException();
        }

        public Item next()
        {
            if(!hasNext())
            {
                throw new NoSuchElementException();
            }
            Item item = a_[randomizedPositions_[n_]];
            n_ -= 1;
            return item;
        }
    }

    private void verifyItem(Item item)
    {
        if(null == item)
        {
            throw new java.lang.NullPointerException("Cannot add null item");
        }
    }

    private void init()
    {
        a_ = (Item[]) new Object[2];
        numOfElement_ = 0;
    }

    private void resize(int capacity)
    {
        assert capacity >= numOfElement_;
        Item[] temp = (Item[]) new Object[capacity];

        for(int i = 0; i < numOfElement_; ++i)
        {
            temp[i] = a_[i];
        }
        a_ = temp;
    }

    private boolean isFull()
    {
        return a_.length == numOfElement_;
    }

    private void printInternal()
    {
        String aInfo = "internal array:";
        for(int i = 0; i < a_.length; ++i)
        {
            if(a_[i] == null)
            {
                aInfo += "(null)";
            }
            else
            {
                aInfo += "("+a_[i]+")";
            }
        }
        StdOut.println(aInfo);
    }

    public static void main(String[] args)
    {
        RandomizedQueue<String> r = new RandomizedQueue<String>();
        while(!StdIn.isEmpty())
        {
            String option = StdIn.readString();
            if(option.equals("+"))
            {
                String item = StdIn.readString();
                StdOut.println("enqueue:"+item);
                r.enqueue(item);
            }
            else if(option.equals("-"))
            {
                StdOut.println("dequeue:"+r.dequeue());
            }
            else if(option.equals("s"))
            {
                StdOut.println("sample:"+r.sample());
            }

            r.printInternal();

            String info = "("+r.size()+") Randomized iterator output:";
            Iterator<String> itr = r.iterator();
            while(itr.hasNext())
            {
                info += "("+itr.next()+")";
            }
            StdOut.println(info);
        }
    }
}
