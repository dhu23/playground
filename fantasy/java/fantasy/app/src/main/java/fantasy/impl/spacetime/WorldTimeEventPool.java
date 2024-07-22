package fantasy.impl.spacetime;

import org.immutables.value.Value;
import fantasy.intf.Character;
import fantasy.intf.Skill;

import java.time.Instant;

public class WorldTimeEventPool {
    public enum EventType {
        AutoAttack,
        GlobalCoolDown,
        SkillCoolDown,
        RuneCoolDown,
        TickNotice
    }

    public interface WorldTimeEvent {
        Instant availableTime();
    }

    @Value.Immutable
    public static abstract class AutoAttack implements WorldTimeEvent {
        @Value.Parameter
        public abstract Character caster();

        @Value.Parameter
        @Override
        public abstract Instant availableTime();

        @Value.Parameter
        public abstract boolean isMainHand();
    }

    @Value.Immutable
    public static abstract class GlobalCoolDown implements WorldTimeEvent {
        @Value.Parameter
        public abstract Character caster();

        @Value.Parameter
        public abstract Skill triggeredBy();

        @Value.Parameter
        @Override
        public abstract Instant availableTime();
    }

    @Value.Immutable
    public static abstract class SkillCoolDown implements WorldTimeEvent {
        @Value.Parameter
        public abstract Character caster();

        @Value.Parameter
        public abstract Skill skill();

        @Value.Parameter
        @Override
        public abstract Instant availableTime();

        @Value.Parameter
        public abstract long id();
    }

    @Value.Immutable
    public static abstract class RuneCoolDown implements WorldTimeEvent{
        @Value.Parameter
        public abstract Character caster();

        @Value.Parameter
        public abstract int runeId();

        @Value.Parameter
        @Override
        public abstract Instant availableTime();

        @Value.Parameter
        public abstract long id();
    }

    @Value.Immutable
    public static abstract class TickNotice implements WorldTimeEvent{
        @Value.Parameter
        public abstract Character target();

        @Value.Parameter
        public abstract String name();

        @Value.Parameter
        public abstract long id();

        @Value.Parameter
        @Override
        public abstract Instant availableTime();
    }
}
