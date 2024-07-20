package fantasy.impl.spacetime;

import com.google.common.annotations.VisibleForTesting;
import fantasy.impl.deathknight.DeathKnight;
import fantasy.impl.event.*;
import fantasy.intf.Character;
import fantasy.intf.Effect;
import fantasy.intf.Skill;
import fantasy.intf.WorldTime;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.time.Instant;
import java.util.Optional;


public class RealTimeWithSpinning extends AbstractWorldTime {
    private static final Logger logger = LoggerFactory.getLogger(RealTimeWithSpinning.class);

    public RealTimeWithSpinning(SequenceNumber sequenceNumber) {
        super(sequenceNumber);
    }

    protected void receiveEvent(Event<WorldEvent.EventType, Object> event) {
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
                    receiveEvent(event);
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
                    receiveEvent(event);
                } else {
                    logger.info(String.format("%s global cool down is clear", gcd.caster().name()));
                    gcd.caster().clearGlobalCoolDown();
                }
            }
            case SkillCoolDown -> {
                WorldEvent.SkillCoolDown scd = (WorldEvent.SkillCoolDown) obj;
                if (now.isBefore(scd.availableTime())) {
                    receiveEvent(event);
                } else {
                    logger.info(String.format("%s's %s cool down is clear", scd.caster().name(), scd.skill().name()));
                    scd.caster().clearSkillCoolDown(scd.skill().name());
                }
            }
            case RuneCoolDown -> {
                WorldEvent.RuneCoolDown rcd = (WorldEvent.RuneCoolDown) obj;
                if (now.isBefore(rcd.availableTime())) {
                    receiveEvent(event);
                } else {
                    DeathKnight dk = (DeathKnight) rcd.caster();
                    logger.info(String.format("%s rune is ready: %s", rcd.caster().name(), rcd.runeId()));
                    dk.clearRuneCoolDown(rcd.runeId());
                }
            }
            case TickNotice -> {
                WorldEvent.TickNotice tn = (WorldEvent.TickNotice) obj;
                if (now.isBefore(tn.nextTickTime())) {
                    receiveEvent(event);
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
}
