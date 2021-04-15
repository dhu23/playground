import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.StdOut;

public class PercolationStats
{
    private double[] m_pList;
    private double m_mean;
    private double m_sigma;
    private int m_count;

    private int getRandom(int N)
    {
        return StdRandom.uniform(1, N+1);
    }

    private double calculateThreshold(int N)
    {
        Percolation percolation = new Percolation(N);
        int openSiteCount = 0;
        while(!percolation.percolates())
        {
            int i = getRandom(N);
            int j = getRandom(N);
            
            while(percolation.isOpen(i, j))
            {
                i = getRandom(N);
                j = getRandom(N);
            }

            percolation.open(i, j);
            openSiteCount += 1;
        }

        return openSiteCount*1.0/(N*N);
    }

    private void calculateStats()
    {
        double sum2 = 0.0;
        double sum = 0.0;

        int count = m_pList.length;
        for(int i = 0; i < count; ++i)
        {
            double p = m_pList[i];
            sum += p;
            sum2 += p*p;
        }

        m_mean = sum/count;
        m_sigma = Math.sqrt((sum2-sum*sum/count)/(count-1));
    }

    public PercolationStats(int N, int T)
    {
        m_pList = new double[T];

        for(int i = 0; i < T; ++i)
        {
            m_pList[i] = calculateThreshold(N);
        }
        
        calculateStats();
        m_count = T;
    }

    public double mean()
    {
        return m_mean;
    }

    public double stddev()
    {
        return m_sigma;
    }

    public double confidenceLo()
    {
        return m_mean-1.96*m_sigma/Math.sqrt(m_count);
    }

    public double confidenceHi()
    {
        return m_mean+1.96*m_sigma/Math.sqrt(m_count);
    }

    public static void main(String[] args)
    {
        System.out.println("PercolationStats.Main");

        int N = Integer.parseInt(args[0]);
        int T = Integer.parseInt(args[1]);

        PercolationStats ps = new PercolationStats(N, T);

        StdOut.printf("mean\t\t\t\t= %f\n", ps.mean());
        StdOut.printf("stddev\t\t\t\t= %f\n", ps.stddev());
        StdOut.printf
            ("95%% confidence interval\t\t = %f, %f\n", 
             ps.confidenceLo(), ps.confidenceHi());
    }
}
