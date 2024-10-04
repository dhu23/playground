import java.util.concurrent.*;
import java.util.Collections;
import java.time.Instant;
import java.time.Duration;

public class ExecutorServiceTest {

    public static void main(String[] args) {
        ConcurrentLinkedQueue<Long> delayList = new ConcurrentLinkedQueue<>();

        ScheduledExecutorService service = new ScheduledThreadPoolExecutor(1);
        for (int i = 0; i < 50; ++i) {
            for (int j = 0; j < 50; ++j) {
                final int i2 = i;
                final int j2 = j;
                Instant start = Instant.now();
                service.schedule(
                        () -> {
                            Instant now = Instant.now();
                            Duration delay = Duration.between(start, now);
                            System.out.println("scheduled time: " + start + ", now: " + now + ", delay: " + delay.toMillis() + ", i=" + i2 + ",j=" + j2);
                            delayList.add(delay.toMillis());
                        }, 
                        10000, TimeUnit.MILLISECONDS
                );
            }
            try {
                Thread.sleep(500);
            } catch (Exception e) {
                break;
            }
        }

        service.shutdown();
        try {
            service.awaitTermination(60000, TimeUnit.MILLISECONDS);
        } catch (Exception e) {
        }
        System.out.println("largest delay: " + Collections.max(delayList));
        System.out.println("size: " + delayList.size());
    }
}
