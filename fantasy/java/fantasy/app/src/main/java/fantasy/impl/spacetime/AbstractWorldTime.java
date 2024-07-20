package fantasy.impl.spacetime;

import fantasy.impl.event.Event;
import fantasy.impl.event.EventQueue;
import fantasy.impl.event.ImmutableEvent;
import fantasy.intf.Character;
import fantasy.intf.Effect;
import fantasy.intf.Skill;
import fantasy.intf.WorldTime;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.time.Instant;

public abstract class AbstractWorldTime implements WorldTime {
    private static final Logger logger = LoggerFactory.getLogger(AbstractWorldTime.class);

    protected final SequenceNumber sequence_;
    protected final EventQueue<Event<WorldEvent.EventType, Object>> eventQueue_;

    public AbstractWorldTime(SequenceNumber sequenceNumber) {
        sequence_ = sequenceNumber;
        this.eventQueue_ = new EventQueue<>(event -> {
            try {
                onEvent_(event);
            } catch (Exception e) {
                logger.error(String.format("got error %s", e));
            }
        });
        this.eventQueue_.start();
    }

    protected abstract void receiveEvent(Event<WorldEvent.EventType, Object> event);
    protected abstract void onEvent_(Event<WorldEvent.EventType, Object> event);

    //protected void void onAutoAttack()

    @Override
    public void stop() {
        this.eventQueue_.stop();
    }

    @Override
    public void scheduleAutoAttack(Character caster, long nextInMillis, boolean mainHand) {
        Instant now = Instant.now();
        Event<WorldEvent.EventType, Object> event =
                ImmutableEvent.of(WorldEvent.EventType.AutoAttack,
                        ImmutableAutoAttack.of(caster, now.plusMillis(nextInMillis), mainHand));
        receiveEvent(event);
    }

    @Override
    public void scheduleGlobalCoolDownEvent(Character caster, Skill skill, long coolDownInMillis) {
        Instant now = Instant.now();
        Event<WorldEvent.EventType, Object> event =
                ImmutableEvent.of(WorldEvent.EventType.GlobalCoolDown,
                        ImmutableGlobalCoolDown.of(caster, skill, now.plusMillis(coolDownInMillis)));
        receiveEvent(event);
        logger.info(String.format("%s's global cool down is triggered by casting %s", caster.name(), skill.name()));
    }

    @Override
    public void scheduleSkillCoolDownEvent(Character caster, Skill skill) {
        if (skill.coolDownInMillis() < 0) {
            return;
        }
        Instant now = Instant.now();
        Event<WorldEvent.EventType, Object> event =
                ImmutableEvent.of(WorldEvent.EventType.SkillCoolDown,
                        ImmutableSkillCoolDown.of(caster, skill, now.plusMillis(skill.coolDownInMillis()), sequence_.getId()));
        receiveEvent(event);
        logger.info(String.format("%s's skill cool down is triggered by casting %s", caster.name(), skill.name()));
    }

    @Override
    public void scheduleRuneCoolDownEvent(Character caster, int runeId, long coolDownInMillis) {
        Instant now = Instant.now();
        Event<WorldEvent.EventType, Object> event =
                ImmutableEvent.of(WorldEvent.EventType.RuneCoolDown,
                        ImmutableRuneCoolDown.of(caster, runeId, now.plusMillis(coolDownInMillis), sequence_.getId()));
        receiveEvent(event);
        logger.info(String.format("%s's rune is on cool down: %d", caster.name(), runeId));
    }

    @Override
    public void scheduleTickNotice(Effect effect) {
        effect.nextTick().ifPresent(nextTick -> {
            Event<WorldEvent.EventType, Object> event =
                    ImmutableEvent.of(WorldEvent.EventType.TickNotice,
                            ImmutableTickNotice.of(effect.target(), effect.name(), effect.id(), nextTick));
            receiveEvent(event);
        });
    }
}
