package yieldcpu;

public class StopWatch {
    private long startNanos = 0;


    public StopWatch() {
    }

    public long start() {
        long start = System.nanoTime();
        startNanos = start;
        return start;
    }

    public long check() {
        long now = System.nanoTime();
        System.out.println(String.format("elapsed time from start: %s (ps)", now - startNanos));
        return now - startNanos;
    }
}
