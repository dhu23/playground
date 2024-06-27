package fantasy.intf;

import java.time.Instant;
import java.util.Optional;

public interface Effect {
    Character caster();
    Character target();
    long id();
    String name();
    void addStack();
    boolean isExpired();
    default boolean isActive() {
        return !isExpired();
    }
    void tick();
    Optional<Instant> nextTick();
}
