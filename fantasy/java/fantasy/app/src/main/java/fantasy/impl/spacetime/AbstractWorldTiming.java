package fantasy.impl.spacetime;

import fantasy.impl.deathknight.DeathKnight;
import fantasy.impl.event.Event;
import fantasy.impl.event.ImmutableEvent;
import fantasy.intf.*;
import fantasy.intf.Character;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import javax.annotation.Nullable;
import java.time.Instant;
import java.util.Optional;

public abstract class AbstractWorldTiming implements WorldTiming {
    private static final Logger logger = LoggerFactory.getLogger(AbstractWorldTiming.class);

    protected final WorldClock clock_;
    protected final Instant endTime_;
    protected final SequenceNumber sequence_;

    public AbstractWorldTiming(WorldClock clock, @Nullable Instant endTime,
                               SequenceNumber sequenceNumber) {
        clock_ = clock;
        endTime_ = endTime;
        sequence_ = sequenceNumber;
    }

    protected abstract void receiveEvent(Event<WorldTimeEventPool.EventType, Object> event);
    protected abstract void onEvent_(Event<WorldTimeEventPool.EventType, Object> event);

    protected boolean shouldEnd() {
        return endTime_ != null && clock_.now().isAfter(endTime_);
    }

    protected Instant now() {
        return clock_.now();
    }

    protected void processEvent(Event<WorldTimeEventPool.EventType, Object> event) {
        WorldTimeEventPool.EventType type = event.type();
        Object data = event.data();
        switch (type) {
            // timer events
            case AutoAttack -> {
                if (data instanceof WorldTimeEventPool.AutoAttack aa) {
                    onAutoAttack(aa);
                }
            }
            case GlobalCoolDown -> {
                if (data instanceof WorldTimeEventPool.GlobalCoolDown gcd) {
                    onGlobalCoolDown(gcd);
                }
            }
            case SkillCoolDown -> {
                if (data instanceof WorldTimeEventPool.SkillCoolDown scd) {
                    onSkillCoolDown(scd);
                }
            }
            case RuneCoolDown -> {
                if (data instanceof WorldTimeEventPool.RuneCoolDown rcd) {
                    onRuneCoolDown(rcd);
                }
            }
            case TickNotice -> {
                if (data instanceof WorldTimeEventPool.TickNotice tn) {
                    onTickNotice(tn);
                }
            }
            case CastComplete -> {
                if (data instanceof WorldTimeEventPool.CastComplete cc) {
                    onCastComplete(cc);
                }
            }

            // command events
            case Select -> {
                if (data instanceof WorldTimeEventPool.Select select) {
                    onSelect(select);
                }
            }
        }
    }

    // on-event logic to process the actual event logic
    protected void onAutoAttack(WorldTimeEventPool.AutoAttack aa) {
        Character caster = aa.caster();
        if (aa.caster().isAutoAttacking()) {
            if (aa.isMainHand()) {
                caster.attackWithMainHand();
                scheduleMainHandAutoAttack(aa.caster());
            } else {
                caster.attackWithOffHand();
                scheduleOffHandAutoAttack(aa.caster());
            }
        }
    }

    protected void onSelect(WorldTimeEventPool.Select select) {
        Character caster = select.caster();
        Character target = select.target();
        caster.selectTarget(target);
    }

    protected void onGlobalCoolDown(WorldTimeEventPool.GlobalCoolDown gcd) {
        logger.info(String.format("%s global cool down is clear", gcd.caster().name()));
        gcd.caster().clearGlobalCoolDown();
    }

    protected void onSkillCoolDown(WorldTimeEventPool.SkillCoolDown scd) {
        logger.info(String.format("%s's %s cool down is clear", scd.caster().name(), scd.skill().name()));
        scd.caster().clearSkillCoolDown(scd.skill().name());
    }

    protected void onRuneCoolDown(WorldTimeEventPool.RuneCoolDown rcd) {
        DeathKnight dk = (DeathKnight) rcd.caster();
        logger.info(String.format("%s rune is ready: %s", rcd.caster().name(), rcd.runeId()));
        dk.clearRuneCoolDown(rcd.runeId());
    }

    protected void onTickNotice(WorldTimeEventPool.TickNotice tn) {
        Character target = tn.target();
        Optional<Effect> effectOptional = target.getEffect(tn.name());
        if (effectOptional.isPresent()) {
            Effect effect = effectOptional.get();
            if (effect.id() == tn.id()) {
                if (effect.isActive()) {
                    effect.tick();
                }

                Character caster = effect.caster();
                if (effect.isExpired()) {
                    logger.info(String.format("%s's %s effect expired on target %s",
                            caster.name(), effect.name(), target.name()));
                    target.removeEffect(effect);
                    effect.caster().onEffectExpiration(target, effect);
                } else {
                    scheduleTickNotice(effect);
                }
            }
        }
    }

    protected void onCastComplete(WorldTimeEventPool.CastComplete cc) {
        Character caster = cc.caster();
        caster.onCastComplete(cc.skill().name());
    }

    @Override
    public WorldClock getClock() {
        return this.clock_;
    }

    @Override
    public void scheduleAutoAttack(Character caster, long nextInMillis, boolean mainHand) {
        Instant now = clock_.now();
        Event<WorldTimeEventPool.EventType, Object> event =
                ImmutableEvent.of(WorldTimeEventPool.EventType.AutoAttack,
                        ImmutableAutoAttack.of(caster, now.plusMillis(nextInMillis), mainHand));
        logger.info("schedule next auto attack {}", event);
        logger.info("clock is {}, now is {}, instant.now is {}", clock_, now, Instant.now());
        receiveEvent(event);
    }

    @Override
    public void scheduleGlobalCoolDownEvent(Character caster, Skill skill, long coolDownInMillis) {
        Instant now = clock_.now();
        Event<WorldTimeEventPool.EventType, Object> event =
                ImmutableEvent.of(WorldTimeEventPool.EventType.GlobalCoolDown,
                        ImmutableGlobalCoolDown.of(caster, skill, now.plusMillis(coolDownInMillis)));
        receiveEvent(event);
        logger.info(String.format("%s's global cool down is triggered by casting %s", caster.name(), skill.name()));
    }

    @Override
    public void scheduleSkillCoolDownEvent(Character caster, Skill skill) {
        if (skill.coolDownInMillis() < 0) {
            return;
        }
        Instant now = clock_.now();
        Event<WorldTimeEventPool.EventType, Object> event =
                ImmutableEvent.of(WorldTimeEventPool.EventType.SkillCoolDown,
                        ImmutableSkillCoolDown.of(caster, skill, now.plusMillis(skill.coolDownInMillis()), sequence_.getId()));
        receiveEvent(event);
        logger.info(String.format("%s's skill cool down of %s is triggered", caster.name(), skill.name()));
    }

    @Override
    public void scheduleRuneCoolDownEvent(Character caster, int runeId, long coolDownInMillis) {
        Instant now = clock_.now();
        Event<WorldTimeEventPool.EventType, Object> event =
                ImmutableEvent.of(WorldTimeEventPool.EventType.RuneCoolDown,
                        ImmutableRuneCoolDown.of(caster, runeId, now.plusMillis(coolDownInMillis), sequence_.getId()));
        receiveEvent(event);
        logger.info(String.format("%s's rune is on cool down: %d", caster.name(), runeId));
    }

    @Override
    public void scheduleTickNotice(Effect effect) {
        effect.nextTick().ifPresent(nextTick -> {
            Event<WorldTimeEventPool.EventType, Object> event =
                    ImmutableEvent.of(WorldTimeEventPool.EventType.TickNotice,
                            ImmutableTickNotice.of(effect.target(), effect.name(), effect.id(), nextTick));
            receiveEvent(event);
        });
    }

    @Override
    public void scheduleCastComplete(Character caster, Skill skill) {
        logger.info("scheduling skill cast complete...");
        Instant now = clock_.now();
        Event<WorldTimeEventPool.EventType, Object> event =
                ImmutableEvent.of(WorldTimeEventPool.EventType.CastComplete,
                        ImmutableCastComplete.of(caster, skill, now.plusMillis(skill.castTimeInMillis())));
        receiveEvent(event);
    }

    @Override
    public void select(Character caster, Character target) {
        Event<WorldTimeEventPool.EventType, Object> event =
                ImmutableEvent.of(WorldTimeEventPool.EventType.Select,
                        ImmutableSelect.of(caster, target));
        receiveEvent(event);
    }
}
