package skillrotation.impl.event;

import org.immutables.value.Value;
import skillrotation.intf.Character;
import skillrotation.intf.Skill;

import java.time.Instant;
import java.util.Optional;

public class WorldEvent {
    public enum EventType {
        Select,
        Cast,
        GlobalCoolDown
    }

    @Value.Immutable
    public static abstract class Select {
        @Value.Parameter
        public abstract Character caster();

        @Value.Parameter
        public abstract Optional<Character> target();
    }

    @Value.Immutable
    public static abstract class Cast {
        @Value.Parameter
        public abstract Character caster();

        @Value.Parameter
        public abstract String spellName();
    }

    @Value.Immutable
    public static abstract class GlobalCoolDown {
        @Value.Parameter
        public abstract Character caster();

        @Value.Parameter
        public abstract Skill triggeredBy();

        @Value.Parameter
        public abstract Instant availableTime();
    }

    @Value.Immutable
    public static abstract class DirectDamage {
        @Value.Parameter
        public abstract int amount();

        @Value.Parameter
        public abstract Character caster();

        @Value.Parameter
        public abstract Character target();
    }
}
