import java.util.Iterator;
import java.util.NoSuchElementException;

import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

public class Deque<Item> implements Iterable<Item>
{
    private Node first_;
    private Node last_;
    private int numOfElement_;

    private class Node
    {
        private Item item_;
        private Node next_;
        private Node prev_;
    }

    public Deque()
    {
        /*
        first_ = new Node();
        last_ = new Node();
        first_.next_ = last_;
        last_.prev_ = first_;
        numOfElement_ = 0;
        */

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

    public void addFirst(Item item)
    {
        verifyItem(item);

        Node itemNode = new Node();
        /*
        itemNode.item_ = item;
        itemNode.next_ = first_.next_;
        first_.next_.prev_ = itemNode;
        itemNode.prev_ = first_;
        first_.next_ = itemNode;
        */
        itemNode.item_ = item;
        
        if(isEmpty())
        {
            itemNode.next_ = null;
            itemNode.prev_ = null;

            first_ = itemNode;
            last_ = itemNode;
        }
        else
        {
            Node oldFirst = first_;
            oldFirst.prev_ = itemNode;
            itemNode.next_ = oldFirst;
            itemNode.prev_ = null;
            first_ = itemNode;
        }
        numOfElement_ += 1;
    }

    public void addLast(Item item)
    {
        Node itemNode = new Node();

        itemNode.item_ = item;
        if(isEmpty())
        {
            itemNode.prev_ = null;
            itemNode.next_ = null;

            first_ = itemNode;
            last_ = itemNode;
        }
        else
        {
            Node oldLast = last_;
            oldLast.next_ = itemNode;
            itemNode.prev_ = oldLast;
            itemNode.next_ = null;
            last_ = itemNode;
        }
        numOfElement_ += 1;
    }

    public Item removeFirst()
    {
        if(isEmpty())
        {
            throw new NoSuchElementException("Deque is empty");
        }

        if(1 == numOfElement_)
        {
            Item item = first_.item_;
            init();
            return item;
        }
        else
        {
            Node oldFirst = first_;
            first_ = first_.next_;
            first_.prev_ = null;
            Item item = oldFirst.item_;
            oldFirst = null;

            numOfElement_ -= 1;
            return item;
        }
    }

    public Item removeLast()
    {
        if(isEmpty())
        {
            throw new NoSuchElementException("Deque is empty");
        }

        if(1 == numOfElement_)
        {
            Item item = last_.item_;
            init();
            return item;
        }
        else
        {
            Node oldLast = last_;
            last_ = last_.prev_;
            last_.next_ = null;
            Item item = oldLast.item_;
            oldLast = null;

            numOfElement_ -= 1;
            return item;
        }
    }

    public Iterator<Item> iterator()
    {
        return new DoubleLinkedListIterator();
    }

    private class DoubleLinkedListIterator implements Iterator<Item>
    {
        private Node current = first_;
        public boolean hasNext()
        {
            return null != current;
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
            Item item = current.item_;
            current = current.next_;
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
        first_ = null;
        last_ = null;
        numOfElement_ = 0;
    }

    private void printInternal()
    {
        Node node = first_;
        String info = "Internal:";
        while(null != node)
        {
            info += "("+node.item_+")";
            node = node.next_;
        }
        StdOut.println(info);
    }

    public static void main(String[] args)
    {
        Deque<String> d = new Deque();
        while(!StdIn.isEmpty())
        {
            String option = StdIn.readString();
            String pos = StdIn.readString();
            if(option.equals("+") && pos.equals("f"))
            {
                String item = StdIn.readString();
                StdOut.println("add first:"+item);
                d.addFirst(item);
            }
            else if(option.equals("+") && pos.equals("l"))
            {
                String item = StdIn.readString();
                StdOut.println("add last:"+item);
                d.addLast(item);
            }
            else if(option.equals("-") && pos.equals("f"))
            {
                StdOut.println("remove first:"+d.removeFirst());
            }
            else if(option.equals("-") && pos.equals("l"))
            {
                StdOut.println("remove last:"+d.removeLast());
            }

            d.printInternal();

            String info = "("+d.size()+") Deque iterator output";
            Iterator<String> itr = d.iterator();
            while(itr.hasNext())
            {
                info += "("+itr.next()+")";
            }
            StdOut.println(info);
        }
    }
}
