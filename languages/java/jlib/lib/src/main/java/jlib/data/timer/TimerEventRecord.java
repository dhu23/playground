package jlib.data.timer;

import org.immutables.value.Value;

import java.util.function.Consumer;

@Value.Immutable
public abstract class TimerEventRecord {
    @Value.Parameter
    public abstract TimerEvent timerEvent();

    @Value.Parameter
    public abstract Consumer<TimerEvent> callback();
}
