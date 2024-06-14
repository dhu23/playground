package fantasy.impl.event;

import org.immutables.value.Value;

@Value.Immutable
public abstract class Event<EVENT_TYPE_ENUM, EVENT_DATA> {
    @Value.Parameter
    public abstract EVENT_TYPE_ENUM type();

    @Value.Parameter
    public abstract EVENT_DATA data();
}
