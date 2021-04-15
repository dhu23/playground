// import java.lang.*;
import edu.princeton.cs.algs4.WeightedQuickUnionUF;

public class Percolation
{
    private int[] m_grid;
    private int m_dimension;

    private WeightedQuickUnionUF m_ufPercolates;
    private WeightedQuickUnionUF m_ufFullness;

    private int getVirtualTopLocation()
    {
        return 0;
    }

    private int getVirtualBottomLocation()
    {
        return m_dimension*m_dimension+1;
    }

    private int getGridLocation(int i, int j)
    {
        if(i <= 0 || i > m_dimension || j <= 0 || j > m_dimension)
        {
            throw new IndexOutOfBoundsException("Bad site position");
        }
        return (i-1)*m_dimension+j;
    }

    private void union(int location, int m, int n)
    {
        try
        {
            int loc = getGridLocation(m, n);
            if(isOpen(m, n))
            {
                m_ufPercolates.union(location, loc);
                m_ufFullness.union(location, loc);
            }
        }
        catch(IndexOutOfBoundsException e)
        {
        }
    }

    // create N-by-N grid with all sites blocked
    public Percolation(int N)
    {
        if(N <= 0)
        {
            throw new IllegalArgumentException("Grid dim less then 1");
        }
        
        m_ufPercolates = new WeightedQuickUnionUF(N*N+2);
        m_ufFullness = new WeightedQuickUnionUF(N*N+1);

        m_dimension = N;
        m_grid = new int[N*N+2];
        // initialize grid to 0, 
        // except postion 0 and N*N+1, which are two virtual sites
        
        m_grid[getVirtualTopLocation()] = 1;
        for(int i = 1; i <= N*N; ++i)
        {
            m_grid[i] = 0;
        }
        m_grid[getVirtualBottomLocation()] = 1;
    }

    // open site(row i, column j) if it is not open already
    public void open(int i, int j)
    {
        if(isOpen(i, j))
        {
            return;
        }
        
        // open the site
        int currentLocation = getGridLocation(i, j);
        m_grid[currentLocation] = 1;

        // update union-find data structure information
        union(currentLocation, i+1, j);
        union(currentLocation, i-1, j);
        union(currentLocation, i, j+1);
        union(currentLocation, i, j-1);

        if(i == 1)
        {
            m_ufPercolates.union(currentLocation, getVirtualTopLocation());
            m_ufFullness.union(currentLocation, getVirtualTopLocation());
        }
        if(i == m_dimension)
        {
            m_ufPercolates.union(currentLocation, getVirtualBottomLocation());
        }
    }

    // is site(row i, column j) open?
    public boolean isOpen(int i, int j)
    {
        int gridLoc = getGridLocation(i, j);
        return 1 == m_grid[gridLoc];
    }

    // is site(row i, column j) full?
    public boolean isFull(int i, int j)
    {
        int gridLoc = getGridLocation(i, j);
        return m_ufFullness.connected(gridLoc, getVirtualTopLocation());
    }

    // does the system percolate?
    public boolean percolates()
    {
        return m_ufPercolates.connected
            (getVirtualTopLocation(), getVirtualBottomLocation());
    }

    // test client
    public static void main(String[] args)
    {
        System.out.println("Percolation.Main");
    }
}
