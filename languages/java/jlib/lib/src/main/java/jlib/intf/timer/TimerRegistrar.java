package jlib.intf.timer;

import jlib.data.timer.TimerEvent;

import java.util.function.Consumer;

public interface TimerRegistrar {
    boolean register(TimerEvent timerEvent, Consumer<TimerEvent> callback);

    boolean register(long delayInMillis, String name, Consumer<TimerEvent> callback);
}
