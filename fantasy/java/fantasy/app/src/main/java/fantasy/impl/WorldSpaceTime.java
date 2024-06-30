package fantasy.impl;

import com.google.common.annotations.VisibleForTesting;
import fantasy.LogUtils;
import fantasy.impl.deathknight.DeathKnight;
import fantasy.impl.event.*;
import fantasy.intf.Character;
import fantasy.intf.Effect;
import fantasy.intf.Skill;

import java.io.IOException;
import java.time.Instant;
import java.util.Optional;
import java.util.Random;
import java.util.concurrent.atomic.AtomicLong;
import java.util.random.RandomGenerator;


public class WorldSpaceTime {
    private static final WorldSpaceTime INSTANCE = new WorldSpaceTime();

    private final LogUtils logUtils_;
    private final AtomicLong counter_;
    private final RandomGenerator randomGenerator_;
    private final EventQueue<Event<WorldEvent.EventType, Object>> eventQueue_;

    public static WorldSpaceTime getInstance() {
        return INSTANCE;
    }

    // TODO move log utils out
    public WorldSpaceTime() {
        try {
            this.logUtils_ = new LogUtils("./logs");
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        counter_ = new AtomicLong(0);
        this.randomGenerator_ = new Random();
        this.eventQueue_ = new EventQueue<>(event -> {
            try {
                onEvent_(event);
            } catch (Exception e) {
                LogUtils.log(String.format("got error %s", e));
            }
        });
        this.eventQueue_.start();
    }

    public long getId() {
        return counter_.getAndIncrement();
    }

    public RandomGenerator getRandomGenerator() {
        return randomGenerator_;
    }

    public LogUtils getLog() {
        return logUtils_;
    }

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
            case Select -> {
                WorldEvent.Select select = (WorldEvent.Select) obj;
                if (select.target().isPresent()) {
                    select.caster().selectTarget(select.target().get());
                } else {
                    select.caster().unSelectTarget();
                }
            }
            case AutoAttack -> {
                WorldEvent.AutoAttack aa = (WorldEvent.AutoAttack) obj;
                if (now.isBefore(aa.nextTime())) {
                    pushEvent(event);
                } else {
                    Character caster = aa.caster();
                    if (aa.caster().isAutoAttacking()) {
                        LogUtils.log(String.format("%s's auto-attack inflicts dmg", caster.name()));
                        if (aa.isMainHand()) {
                            caster.attackWithMainHand();
                            pushMainHandAutoAttack(aa.caster());
                        } else {
                            caster.attackWithOffHand();
                            pushOffHandAutoAttack(aa.caster());
                        }
                    }
                }
            }
            case Cast -> {
                WorldEvent.Cast cast = (WorldEvent.Cast) obj;
                cast.caster().cast(cast.spellName());
            }
            case GlobalCoolDown -> {
                WorldEvent.GlobalCoolDown gcd = (WorldEvent.GlobalCoolDown) obj;
                // System.out.println(gcd);
                if (now.isBefore(gcd.availableTime())) {
                    pushEvent(event);
                } else {
                    LogUtils.log(String.format("%s global cool down is clear", gcd.caster().name()));
                    gcd.caster().clearGlobalCoolDown();
                }
            }
            case SkillCoolDown -> {
                WorldEvent.SkillCoolDown scd = (WorldEvent.SkillCoolDown) obj;
                if (now.isBefore(scd.availableTime())) {
                    pushEvent(event);
                } else {
                    LogUtils.log(String.format("%s's %s cool down is clear", scd.caster().name(), scd.skill().name()));
                    scd.caster().clearSkillCoolDown(scd.skill().name());
                }
            }
            case RuneCoolDown -> {
                WorldEvent.RuneCoolDown rcd = (WorldEvent.RuneCoolDown) obj;
                if (now.isBefore(rcd.availableTime())) {
                    pushEvent(event);
                } else {
                    DeathKnight dk = (DeathKnight) rcd.caster();
                    LogUtils.log(String.format("%s rune is ready: %s", rcd.caster().name(), rcd.runeId()));
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
                                LogUtils.log(String.format("%s's %s effect expired on target %s",
                                        caster.name(), effect.name(), target.name()));
                                target.removeEffect(effect);
                                effect.caster().onEffectExpiration(target, effect);
                            } else {
                                pushTickNotice(effect);
                            }
                        }
                    }
                }
            }
        }
    }

    public void pushGlobalCoolDownEvent(Character caster, Skill skill, long coolDownInMillis) {
        Instant now = Instant.now();
        Event<WorldEvent.EventType, Object> event =
                ImmutableEvent.of(WorldEvent.EventType.GlobalCoolDown,
                        ImmutableGlobalCoolDown.of(caster, skill, now.plusMillis(coolDownInMillis)));
        pushEvent(event);
        LogUtils.log(String.format("%s's global cool down is triggered by casting %s", caster.name(), skill.name()));
    }

    public void pushSkillCoolDownEvent(Character caster, Skill skill) {
        if (skill.coolDownInMillis() < 0) {
            return;
        }
        Instant now = Instant.now();
        Event<WorldEvent.EventType, Object> event =
                ImmutableEvent.of(WorldEvent.EventType.SkillCoolDown,
                        ImmutableSkillCoolDown.of(caster, skill, now.plusMillis(skill.coolDownInMillis()), getId()));
        pushEvent(event);
        LogUtils.log(String.format("%s's skill cool down is triggered by casting %s", caster.name(), skill.name()));
    }

    public void pushRuneCoolDownEvent(DeathKnight dk, int runeId, long coolDownInMillis) {
        Instant now = Instant.now();
        Event<WorldEvent.EventType, Object> event =
                ImmutableEvent.of(WorldEvent.EventType.RuneCoolDown,
                        ImmutableRuneCoolDown.of(dk, runeId, now.plusMillis(coolDownInMillis), getId()));
        pushEvent(event);
        LogUtils.log(String.format("%s's rune is on cool down: %d", dk.name(), runeId));
    }

    public void pushTickNotice(Effect effect) {
        effect.nextTick().ifPresent(nextTick -> {
            Event<WorldEvent.EventType, Object> event =
                    ImmutableEvent.of(WorldEvent.EventType.TickNotice,
                            ImmutableTickNotice.of(effect.target(), effect.name(), effect.id(), nextTick));
            pushEvent(event);
        });
    }

    private void pushAutoAttack(Character caster, long nextInMillis, boolean mainHand) {
        Instant now = Instant.now();
        Event<WorldEvent.EventType, Object> event =
                ImmutableEvent.of(WorldEvent.EventType.AutoAttack,
                        ImmutableAutoAttack.of(caster, now.plusMillis(nextInMillis), mainHand));
        pushEvent(event);
    }

    public void pushMainHandAutoAttack(Character caster) {
        pushAutoAttack(caster, caster.mainHandAttackSpeed().toMillis(), true);
    }

    public void pushOffHandAutoAttack(Character caster) {
        caster.offHandAttackSpeed().ifPresent(offHandSpeed -> {
            pushAutoAttack(caster, offHandSpeed.toMillis(), false);
        });
    }
}
