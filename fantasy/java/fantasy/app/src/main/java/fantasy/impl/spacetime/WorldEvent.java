package fantasy.impl.spacetime;

import org.immutables.value.Value;
import fantasy.intf.Character;
import fantasy.intf.Skill;

import java.time.Instant;

public class WorldEvent {
    public enum EventType {
        AutoAttack,
        GlobalCoolDown,
        SkillCoolDown,
        RuneCoolDown,
        TickNotice
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
    public static abstract class TickNotice {
        @Value.Parameter
        public abstract Character target();

        @Value.Parameter
        public abstract String name();

        @Value.Parameter
        public abstract long id();

        @Value.Parameter
        public abstract Instant nextTickTime();
    }
}
