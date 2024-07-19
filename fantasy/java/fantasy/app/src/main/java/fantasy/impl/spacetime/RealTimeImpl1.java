package fantasy.impl.spacetime;

import com.google.common.annotations.VisibleForTesting;
import fantasy.impl.*;
import fantasy.impl.deathknight.DeathKnight;
import fantasy.impl.event.*;
import fantasy.intf.Character;
import fantasy.intf.Effect;
import fantasy.intf.Skill;
import fantasy.intf.WorldTime;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.time.Instant;
import java.util.Optional;
import java.util.Random;
import java.util.random.RandomGenerator;


public class RealTimeImpl1 implements WorldTime {
    private static final Logger logger = LoggerFactory.getLogger(RealTimeImpl1.class);

    private final SequenceNumber sequence_;
    private final EventQueue<Event<WorldEvent.EventType, Object>> eventQueue_;

    // TODO move log utils out
    public RealTimeImpl1(SequenceNumber sequenceNumber) {
        sequence_ = sequenceNumber;
        this.eventQueue_ = new EventQueue<>(event -> {
            try {
                onEvent_(event);
            } catch (Exception e) {
                logger.error(String.format("got error %s", e));
            }
        });
    }

    @Override
    public void start() {
        this.eventQueue_.start();
    }

    @Override
    public void stop() {
        this.eventQueue_.stop();
    }

    protected void pushEvent(Event<WorldEvent.EventType, Object> event) {
        this.eventQueue_.pushEvent(event);
    }

    @VisibleForTesting
    protected void onEvent_(Event<WorldEvent.EventType, Object> event) {
        WorldEvent.EventType type = event.type();
        Object obj = event.data();
        Instant now = Instant.now();
        switch (type) {
            case AutoAttack -> {
                WorldEvent.AutoAttack aa = (WorldEvent.AutoAttack) obj;
                if (now.isBefore(aa.nextTime())) {
                    pushEvent(event);
                } else {
                    Character caster = aa.caster();
                    if (aa.caster().isAutoAttacking()) {
//                        LogUtils.log(String.format("%s's auto-attack inflicts dmg", caster.name()));
                        if (aa.isMainHand()) {
                            caster.attackWithMainHand();
                            scheduleMainHandAutoAttack(aa.caster());
                        } else {
                            caster.attackWithOffHand();
                            scheduleOffHandAutoAttack(aa.caster());
                        }
                    }
                }
            }
            case GlobalCoolDown -> {
                WorldEvent.GlobalCoolDown gcd = (WorldEvent.GlobalCoolDown) obj;
                // System.out.println(gcd);
                if (now.isBefore(gcd.availableTime())) {
                    pushEvent(event);
                } else {
                    logger.info(String.format("%s global cool down is clear", gcd.caster().name()));
                    gcd.caster().clearGlobalCoolDown();
                }
            }
            case SkillCoolDown -> {
                WorldEvent.SkillCoolDown scd = (WorldEvent.SkillCoolDown) obj;
                if (now.isBefore(scd.availableTime())) {
                    pushEvent(event);
                } else {
                    logger.info(String.format("%s's %s cool down is clear", scd.caster().name(), scd.skill().name()));
                    scd.caster().clearSkillCoolDown(scd.skill().name());
                }
            }
            case RuneCoolDown -> {
                WorldEvent.RuneCoolDown rcd = (WorldEvent.RuneCoolDown) obj;
                if (now.isBefore(rcd.availableTime())) {
                    pushEvent(event);
                } else {
                    DeathKnight dk = (DeathKnight) rcd.caster();
                    logger.info(String.format("%s rune is ready: %s", rcd.caster().name(), rcd.runeId()));
                    dk.clearRuneCoolDown(rcd.runeId());
                }
            }
            case TickNotice -> {
                WorldEvent.TickNotice tn = (WorldEvent.TickNotice) obj;
                if (now.isBefore(tn.nextTickTime())) {
                    pushEvent(event);
                } else {
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
            }
        }
    }

    @Override
    public void scheduleAutoAttack(Character caster, long nextInMillis, boolean mainHand) {
        Instant now = Instant.now();
        Event<WorldEvent.EventType, Object> event =
                ImmutableEvent.of(WorldEvent.EventType.AutoAttack,
                        ImmutableAutoAttack.of(caster, now.plusMillis(nextInMillis), mainHand));
        pushEvent(event);
    }

    @Override
    public void scheduleGlobalCoolDownEvent(Character caster, Skill skill, long coolDownInMillis) {
        Instant now = Instant.now();
        Event<WorldEvent.EventType, Object> event =
                ImmutableEvent.of(WorldEvent.EventType.GlobalCoolDown,
                        ImmutableGlobalCoolDown.of(caster, skill, now.plusMillis(coolDownInMillis)));
        pushEvent(event);
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
        pushEvent(event);
        logger.info(String.format("%s's skill cool down is triggered by casting %s", caster.name(), skill.name()));
    }

    @Override
    public void scheduleRuneCoolDownEvent(Character caster, int runeId, long coolDownInMillis) {
        Instant now = Instant.now();
        Event<WorldEvent.EventType, Object> event =
                ImmutableEvent.of(WorldEvent.EventType.RuneCoolDown,
                        ImmutableRuneCoolDown.of(caster, runeId, now.plusMillis(coolDownInMillis), sequence_.getId()));
        pushEvent(event);
        logger.info(String.format("%s's rune is on cool down: %d", caster.name(), runeId));
    }

    @Override
    public void scheduleTickNotice(Effect effect) {
        effect.nextTick().ifPresent(nextTick -> {
            Event<WorldEvent.EventType, Object> event =
                    ImmutableEvent.of(WorldEvent.EventType.TickNotice,
                            ImmutableTickNotice.of(effect.target(), effect.name(), effect.id(), nextTick));
            pushEvent(event);
        });
    }
}
