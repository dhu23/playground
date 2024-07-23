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
import java.util.concurrent.CountDownLatch;

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

    protected abstract void receiveEvent(Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event);
    protected abstract void onEvent_(Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event);

    protected boolean shouldEnd() {
        return endTime_ != null && clock_.now().isAfter(endTime_);
    }

    protected Instant now() {
        return clock_.now();
    }

    protected void processEvent(Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event) {
        WorldTimeEventPool.EventType type = event.type();
        WorldTimeEventPool.WorldTimeEvent worldTimeEvent = event.data();
        switch (type) {
            case AutoAttack -> {
                if (worldTimeEvent instanceof WorldTimeEventPool.AutoAttack aa) {
                    onAutoAttack(aa);
                }
            }
            case GlobalCoolDown -> {
                if (worldTimeEvent instanceof WorldTimeEventPool.GlobalCoolDown gcd) {
                    onGlobalCoolDown(gcd);
                }
            }
            case SkillCoolDown -> {
                if (worldTimeEvent instanceof WorldTimeEventPool.SkillCoolDown scd) {
                    onSkillCoolDown(scd);
                }
            }
            case RuneCoolDown -> {
                if (worldTimeEvent instanceof WorldTimeEventPool.RuneCoolDown rcd) {
                    onRuneCoolDown(rcd);
                }
            }
            case TickNotice -> {
                if (worldTimeEvent instanceof WorldTimeEventPool.TickNotice tn) {
                    onTickNotice(tn);
                }
            }
        }
    }

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

    @Override
    public WorldClock getClock() {
        return this.clock_;
    }

    @Override
    public void scheduleAutoAttack(Character caster, long nextInMillis, boolean mainHand) {
        Instant now = clock_.now();
        Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event =
                ImmutableEvent.of(WorldTimeEventPool.EventType.AutoAttack,
                        ImmutableAutoAttack.of(caster, now.plusMillis(nextInMillis), mainHand));
        receiveEvent(event);
    }

    @Override
    public void scheduleGlobalCoolDownEvent(Character caster, Skill skill, long coolDownInMillis) {
        Instant now = clock_.now();
        Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event =
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
        Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event =
                ImmutableEvent.of(WorldTimeEventPool.EventType.SkillCoolDown,
                        ImmutableSkillCoolDown.of(caster, skill, now.plusMillis(skill.coolDownInMillis()), sequence_.getId()));
        receiveEvent(event);
        logger.info(String.format("%s's skill cool down is triggered by casting %s", caster.name(), skill.name()));
    }

    @Override
    public void scheduleRuneCoolDownEvent(Character caster, int runeId, long coolDownInMillis) {
        Instant now = clock_.now();
        Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event =
                ImmutableEvent.of(WorldTimeEventPool.EventType.RuneCoolDown,
                        ImmutableRuneCoolDown.of(caster, runeId, now.plusMillis(coolDownInMillis), sequence_.getId()));
        receiveEvent(event);
        logger.info(String.format("%s's rune is on cool down: %d", caster.name(), runeId));
    }

    @Override
    public void scheduleTickNotice(Effect effect) {
        effect.nextTick().ifPresent(nextTick -> {
            Event<WorldTimeEventPool.EventType, WorldTimeEventPool.WorldTimeEvent> event =
                    ImmutableEvent.of(WorldTimeEventPool.EventType.TickNotice,
                            ImmutableTickNotice.of(effect.target(), effect.name(), effect.id(), nextTick));
            receiveEvent(event);
        });
    }
}
