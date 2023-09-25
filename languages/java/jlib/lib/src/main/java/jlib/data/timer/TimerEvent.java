package jlib.data.timer;

import org.immutables.value.Value;

import java.time.Instant;

@Value.Immutable
public abstract class TimerEvent {
    @Value.Parameter
    public abstract String name();

    @Value.Parameter
    public abstract Instant eventTime();
}
