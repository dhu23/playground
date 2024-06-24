package fantasy.impl.event;

import fantasy.impl.Effect;
import org.immutables.value.Value;
import fantasy.intf.Character;
import fantasy.intf.Skill;

import java.time.Duration;
import java.time.Instant;
import java.util.Optional;

public class WorldEvent {
    public enum EventType {
        Select,
        AutoAttack,
        Cast,
        GlobalCoolDown,
        SkillCoolDown,
        RuneCoolDown,
        AmountOverTime
    }

    @Value.Immutable
    public static abstract class Select {
        @Value.Parameter
        public abstract Character caster();

        @Value.Parameter
        public abstract Optional<Character> target();
    }

    @Value.Immutable
    public static abstract class AutoAttack {
        @Value.Parameter
        public abstract Character caster();

        @Value.Parameter
        public abstract Instant nextTime();

        @Value.Parameter
        public abstract boolean isMainHand();
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
    public static abstract class SkillCoolDown {
        @Value.Parameter
        public abstract Character caster();

        @Value.Parameter
        public abstract Skill skill();

        @Value.Parameter
        public abstract Instant availableTime();

        @Value.Parameter
        public abstract long id();
    }

    @Value.Immutable
    public static abstract class RuneCoolDown {
        @Value.Parameter
        public abstract Character caster();

        @Value.Parameter
        public abstract int runeId();

        @Value.Parameter
        public abstract Instant availableTime();

        @Value.Parameter
        public abstract long id();
    }

    @Value.Immutable
    public static abstract class AmountOverTime {
        public enum Type {
            HoT,
            DoT
        }
        @Value.Parameter
        public abstract Type type();

        @Value.Parameter
        public abstract Effect name();

        @Value.Parameter
        public abstract Character caster();

        @Value.Parameter
        public abstract Character target();

        @Value.Parameter
        public abstract long id();

        @Value.Parameter
        public abstract int tickAmount();

        @Value.Parameter
        public abstract Duration frequency();

        @Value.Parameter
        public abstract Instant nextTickTime();

        @Value.Parameter
        public abstract int remainingTickCount();
    }
}
