package jlib.impl.timer;

import com.google.common.annotations.VisibleForTesting;
import com.google.common.base.Preconditions;
import jlib.data.timer.ImmutableTimerEvent;
import jlib.data.timer.TimerEvent;
import jlib.data.timer.TimerEventRecord;
import jlib.intf.timer.TimerRegistrar;

import java.time.Clock;
import java.time.Instant;
import java.util.Comparator;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.function.Consumer;

public class TimerRegistrarImpl1 implements TimerRegistrar {
    private static final int INIT_CAPACITY = 1024;
    // manages the event by the timestamp
    private final Clock clock;
    private final Timer timer;
    private final ScheduledExecutorService scheduledExecutorService;

    @VisibleForTesting
    protected static class TimeEventRecordComparator implements Comparator<TimerEventRecord> {

        @Override
        public int compare(TimerEventRecord left, TimerEventRecord right) {
            return left.timerEvent().eventTime().compareTo(right.timerEvent().eventTime());
        }
    }

    public TimerRegistrarImpl1(ScheduledExecutorService scheduledExecutorService) {
        clock = Clock.systemDefaultZone();
        timer = new Timer();
        this.scheduledExecutorService = Preconditions.checkNotNull(scheduledExecutorService);
    }

    @Override
    public boolean register(TimerEvent timerEvent, Consumer<TimerEvent> callback) {
        Instant now = clock.instant();
        Instant eventTime = timerEvent.eventTime();
        if (eventTime.isBefore(now)) {
            return false;
        } else {
            long delay = eventTime.toEpochMilli() - now.toEpochMilli();
            scheduledExecutorService.schedule(new Runnable() {
                @Override
                public void run() {
                    callback.accept(timerEvent);
                }
            }, delay, TimeUnit.MILLISECONDS);
            return true;
        }
    }

    @Override
    public boolean register(long delayInMillis, String name, Consumer<TimerEvent> callback) {
        return register(ImmutableTimerEvent.of(name, clock.instant().plusMillis(delayInMillis)), callback);
    }
}
