package skillrotation.impl.event;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.function.Consumer;

public class EventQueue<T> {
    private final Consumer<T> eventProcessor_;
    private final BlockingQueue<T> queue_;
    private final Thread thread_;
    private final AtomicBoolean stopFlag_;

    public EventQueue(Consumer<T> eventProcessor) {
        this.eventProcessor_ = eventProcessor;
        this.queue_ = new LinkedBlockingQueue<>();
        this.thread_ = new Thread(this::processQueue_, "event-queue-thread");
        this.stopFlag_ = new AtomicBoolean(false);
    }

    public void start() {
        this.thread_.start();
    }

    public void stop() {
        this.stopFlag_.set(true);
    }

    public void pushEvent(T event) {
        this.queue_.offer(event);
    }

    protected void processQueue_() {
        while (!this.stopFlag_.get()) {
            try {
                T data = queue_.poll(100, TimeUnit.MILLISECONDS);
                if (data != null) {
                    // System.out.println("got something!!!!!!!");
                    eventProcessor_.accept(data);
                } else {
                    // System.out.println("got null");
                }
            } catch (InterruptedException e) {
                break;
            }
        }
    }
}
