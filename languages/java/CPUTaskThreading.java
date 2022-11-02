import java.util.List;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.CountDownLatch;


public class CPUTaskThreading {

    // public static final List<Integer> FIB_INPUT = new ArrayList<Integer>(Arrays.asList(10, 11, 12, 13, 14, 15, 16, 17));
    // public static final List<Integer> FIB_INPUT = new ArrayList<Integer>(Arrays.asList(20, 21, 22, 23, 24, 25, 26, 27));
    public static final List<Integer> FIB_INPUT = new ArrayList<Integer>(Arrays.asList(30, 31, 32, 33, 34, 35));
    public static final Long BATCH_SLEEP_MILLISECONDS = null;
    // public static final Long BATCH_SLEEP_MILLISECONDS = 100L;

    public static class Result {
        int arg;
        int ans;
        long start;
        long end;

        public Result(int arg, int ans, long start, long end) {
            this.arg = arg;
            this.ans = ans;
            this.start = start;
            this.end = end;
        }

        public String toString() {
            return this.arg + "|" + this.ans + "|" + this.start + "|" + this.end + "|" + (this.end-this.start);
        }
    }

    public static class ResultSummary {
        public long firstStart;
        public long lastEnd;
        public Map<Integer, Integer> ansMap;
        public Map<Integer, List<Long>> startMap;
        public Map<Integer, List<Long>> endMap;
        public Map<Integer, List<Long>> costMap;

        public ResultSummary() {
            this.firstStart = 0L;
            this.lastEnd = 0L;
            this.ansMap = new HashMap<Integer, Integer>();
            this.startMap = new HashMap<Integer, List<Long>>();
            this.endMap = new HashMap<Integer, List<Long>>();
            this.costMap = new HashMap<Integer, List<Long>>();
        }

        public void add(Result ret) {
            if (firstStart == 0L || firstStart > ret.start) {
                firstStart = ret.start;
            }
            if (lastEnd < ret.end) {
                lastEnd = ret.end;
            }
            this.ansMap.put(ret.arg, ret.ans);
            this.startMap.computeIfAbsent(ret.arg, x -> new ArrayList<Long>()).add(ret.start);
            this.endMap.computeIfAbsent(ret.arg, x -> new ArrayList<Long>()).add(ret.end);
            this.costMap.computeIfAbsent(ret.arg, x -> new ArrayList<Long>()).add(ret.end-ret.start);
        }

        public void summarize() {
            costMap.entrySet().stream()
                .sorted(Map.Entry.comparingByKey())
                .forEachOrdered(x -> System.out.printf("%d \t\t | %f \n", x.getKey(), getMean(x.getValue()) / 1000000000.0));
        }
    }

    // x=0, fib=0, cost=1,176 
    // x=1, fib=1, cost=3,215 
    // x=2, fib=1, cost=797 
    // x=3, fib=2, cost=806 
    // x=4, fib=3, cost=1,147 
    // x=5, fib=5, cost=2,052 
    // x=6, fib=8, cost=3,141 
    // x=7, fib=13, cost=4,906 
    // x=8, fib=21, cost=6,726 
    // x=9, fib=34, cost=25,452 
    // x=10, fib=55, cost=9,837 
    // x=11, fib=89, cost=2,416 
    // x=12, fib=144, cost=3,367 
    // x=13, fib=233, cost=4,514 
    // x=14, fib=377, cost=6,728 
    // x=15, fib=610, cost=8,871 
    // x=16, fib=987, cost=22,823 
    // x=17, fib=1597, cost=26,141 
    // x=18, fib=2584, cost=36,428 
    // x=19, fib=4181, cost=35,193 
    // x=20, fib=6765, cost=49,572 
    // x=21, fib=10946, cost=43,359 
    // x=22, fib=17711, cost=113,475 
    // x=23, fib=28657, cost=228,683 
    // x=24, fib=46368, cost=277,984 
    // x=25, fib=75025, cost=561,592 
    // x=26, fib=121393, cost=721,388 
    // x=27, fib=196418, cost=793,620 
    // x=28, fib=317811, cost=1,290,704 
    // x=29, fib=514229, cost=2,113,276 
    // x=30, fib=832040, cost=3,353,879 
    // x=31, fib=1346269, cost=5,912,648 
    // x=32, fib=2178309, cost=8,852,151 
    // x=33, fib=3524578, cost=20,986,988 
    // x=34, fib=5702887, cost=24,795,992 
    // x=35, fib=9227465, cost=42,447,139 
    // x=36, fib=14930352, cost=66,808,080 
    // x=37, fib=24157817, cost=98,758,351 
    // x=38, fib=39088169, cost=160,528,053 
    // x=39, fib=63245986, cost=263,880,842 
    public static int fib(int n) {
        if (n < 2) {
            return n;
        } else {
            return fib(n-1) + fib(n-2);
        }
    }

    public static double getMean(List<Long> ns) {
        long sum = ns.stream().mapToLong(Long::longValue).sum();
        return sum / ns.size();
    }

    public static int backoutFib(int fibVal) {
        int n = 0;
        while (fib(n) < fibVal) {
            ++n;
        }
        return n;
    }

    @FunctionalInterface
    public interface Task {
        int runTask(int n);
    }

    public static Result runTask(Task task, int arg) {
        long startNs = System.nanoTime();
        int ret = task.runTask(arg);
        long endNs = System.nanoTime();
        return new Result(arg, ret, startNs, endNs);
    }

    public static Result runProducerTask(int arg) {
        return runTask(CPUTaskThreading::fib, arg);
    }

    public static Result runConsumerTask(int arg) {
        return runTask(CPUTaskThreading::backoutFib, arg);
    }

    public static void showFibCosts() {
        for (int i = 0; i < 40; ++i) {
            long startNs = System.nanoTime();
            int ret = fib(i);
            long endNs = System.nanoTime();
            System.out.printf("x=%d, fib=%d, cost=%,d %n", i, ret, endNs-startNs);
        }
    }

    public static class RunResult {
        public ResultSummary prets;
        public ResultSummary crets;
        public long cost;

        public RunResult() {
            this.prets = new ResultSummary();
            this.crets = new ResultSummary();
            this.cost = 0L;
        }
    }

    public static class Producer implements Runnable {

        private ResultSummary resultSummary;
        private BlockingQueue<Integer> queue;
        private CountDownLatch latch;
        private int repeatLimit;

        public Producer(
                ResultSummary rsum, BlockingQueue<Integer> queue, CountDownLatch latch, int repeatLimit) {
            this.resultSummary = rsum;
            this.queue = queue;
            this.latch = latch;
            this.repeatLimit = repeatLimit;
        }

        public void run() {
            int repeated = 0;
            while (true) {
                for (int n : FIB_INPUT) {
                    Result ret = runProducerTask(n);
                    try {
                        this.queue.put(ret.ans);
                        resultSummary.add(ret);
                    } catch (InterruptedException e) {
                    } catch (NullPointerException e) {
                    }
                }

                ++repeated;
                if (repeated < repeatLimit) {
                    if (BATCH_SLEEP_MILLISECONDS != null) {
                        try {
                            Thread.sleep(BATCH_SLEEP_MILLISECONDS);
                        } catch (InterruptedException e) {
                        }
                    }
                } else {
                    break;
                }
            }
            latch.countDown();
        }
    }

    public static class Consumer implements Runnable {

        private ResultSummary resultSummary;
        private BlockingQueue<Integer> queue;
        private CountDownLatch latch;

        public Consumer(ResultSummary rsum, BlockingQueue<Integer> queue, CountDownLatch latch) {
            this.resultSummary = rsum;
            this.queue = queue;
            this.latch = latch;
        }

        public void run() {
            while (true) {
                try {
                    Integer fibVal = queue.poll(2, TimeUnit.SECONDS);
                    if (fibVal == null) {
                        if (latch.getCount() > 0) {
                            continue;
                        } else {
                            break;
                        }
                    } else {
                        resultSummary.add(runConsumerTask(fibVal));
                    }
                } catch (InterruptedException e) {
                }
            }
        }
    }

    public static RunResult singleThreadRun(int repeatLimit) throws InterruptedException {
        long startNs = System.nanoTime();
        int repeated = 0;

        RunResult rres = new RunResult();

        while (true) {
            for (int n : FIB_INPUT) {
                Result producerResult = runProducerTask(n);
                // System.out.println(producerResult.toString());
                Result consumerResult = runConsumerTask(producerResult.ans);
                rres.prets.add(producerResult);
                rres.crets.add(consumerResult);
            }
            ++repeated;

            if (repeated < repeatLimit) {
                if (BATCH_SLEEP_MILLISECONDS != null) {
                    Thread.sleep(BATCH_SLEEP_MILLISECONDS);
                }
            } else {
                break;
            }
        }
        long endNs = System.nanoTime();
        rres.cost = endNs-startNs;
        
        System.out.printf("run singlethreaded against %s, repeated %d times, cost=%,d %n", FIB_INPUT, repeatLimit, rres.cost);
        
        return rres;
    }

    public static RunResult multiThreadRun(int repeatLimit) {
        long startNs = System.nanoTime();

        RunResult rres = new RunResult();

        BlockingQueue<Integer> queue = new LinkedBlockingQueue<Integer>(256);
        CountDownLatch latch = new CountDownLatch(1);

        Producer p = new Producer(rres.prets, queue, latch, repeatLimit);
        Consumer c = new Consumer(rres.crets, queue, latch);

        Thread pthread = new Thread(p);
        Thread cthread = new Thread(c);

        pthread.start();
        cthread.start();

        try {
            pthread.join();
            cthread.join();
        } catch (InterruptedException e) {
        }

        long endNs = System.nanoTime();
        rres.cost = endNs-startNs;
        System.out.printf("run multithreaded against %s, repeated %d times, cost=%,d %n", FIB_INPUT, repeatLimit, rres.cost);
        return rres;
    }

    public static void main(String[] args) {
        int repeatLimit = 40;
        try {
            RunResult sret = singleThreadRun(repeatLimit);
            RunResult mret = multiThreadRun(repeatLimit);

            System.out.println("---------------- Single producer -----------------");
            sret.prets.summarize();
            System.out.println("---------------- Single consumer -----------------");
            sret.crets.summarize();
            long sComputeCost = sret.crets.lastEnd-sret.prets.firstStart;
            System.out.printf("computing cost: %,d %n", sComputeCost);
            System.out.printf("total cost: %,d  overhead: %,d %n", sret.cost, (sret.cost-sComputeCost));

            System.out.println("---------------- Multi producer -----------------");
            mret.prets.summarize();
            System.out.println("---------------- Multi consumer -----------------");
            mret.crets.summarize();
            long mComputeCost = mret.crets.lastEnd-mret.prets.firstStart;
            System.out.printf("computing cost: %,d %n", mComputeCost);
            System.out.printf("total cost: %,d  overhead: %,d %n", mret.cost, (mret.cost-mComputeCost));

        } catch (InterruptedException e) {
        }
    }
}
