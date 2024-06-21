package fantasy.impl;

import com.google.common.annotations.VisibleForTesting;
import fantasy.LogUtils;
import fantasy.impl.deathknight.DeathKnight;
import fantasy.impl.event.*;
import fantasy.intf.Character;
import fantasy.intf.Skill;

import java.io.IOException;
import java.time.Duration;
import java.time.Instant;
import java.util.Random;
import java.util.random.RandomGenerator;


public class WorldSpaceTime {
    private static final WorldSpaceTime INSTANCE = new WorldSpaceTime();

    private final LogUtils logUtils_;
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
                // System.out.println(select);
                if (select.target().isPresent()) {
                    select.caster().selectTarget(select.target().get());
                } else {
                    select.caster().unSelectTarget();
                }
            }
            case Cast -> {
                WorldEvent.Cast cast = (WorldEvent.Cast) obj;
                // System.out.println(cast);
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
            case AmountOverTime -> {
                WorldEvent.AmountOverTime aot = (WorldEvent.AmountOverTime) obj;
                if (now.isBefore(aot.nextTickTime())) {
                    pushEvent(event);
                } else {
                    Character caster = aot.caster();
                    Character target = aot.target();
                    switch (aot.type()) {
                        case DoT -> {
                            target.sufferDamage(aot.tickAmount());
                            LogUtils.log(String.format("%s suffers %d damage from %s's %s damage",
                                    aot.target().name(), aot.tickAmount(), aot.caster().name(), aot.name()));
                        }
                        case HoT -> {
                            target.receiveHealing(aot.tickAmount());
                            LogUtils.log(String.format("%s receives %d healing from %s's %s healing",
                                    aot.target().name(), aot.tickAmount(), aot.caster().name(), aot.name()));
                        }
                    }
                    if (aot.remainingTickCount() == 1) {
                        LogUtils.log(String.format("%s's %s fades from %s",
                                aot.caster().name(), aot.name(), aot.target().name()));
                        aot.target().removeEffect(aot.name());
                    } else {
                        pushEffectOverTime(aot.type(), aot.name(), aot.caster(), aot.target(), aot.tickAmount(),
                                aot.frequency(), aot.remainingTickCount() - 1);
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
        Instant now = Instant.now();
        Event<WorldEvent.EventType, Object> event =
                ImmutableEvent.of(WorldEvent.EventType.SkillCoolDown,
                        ImmutableSkillCoolDown.of(caster, skill, now.plusMillis(skill.coolDownInMillis())));
        pushEvent(event);
        LogUtils.log(String.format("%s's skill cool down is triggered by casting %s", caster.name(), skill.name()));
    }

    public void pushRuneCoolDownEvent(DeathKnight dk, int runeId, long coolDownInMillis) {
        Instant now = Instant.now();
        Event<WorldEvent.EventType, Object> event =
                ImmutableEvent.of(WorldEvent.EventType.RuneCoolDown,
                        ImmutableRuneCoolDown.of(dk, runeId, now.plusMillis(coolDownInMillis)));
        pushEvent(event);
        LogUtils.log(String.format("%s's rune is on cool down: %d", dk.name(), runeId));
    }

    private void pushEffectOverTime(WorldEvent.AmountOverTime.Type type, Effect effect, Character caster, Character target,
                                   int tickAmount, Duration frequency, int tickCount) {
        Instant now = Instant.now();
        Event<WorldEvent.EventType, Object> event =
                ImmutableEvent.of(WorldEvent.EventType.AmountOverTime,
                        ImmutableAmountOverTime.of(type, effect, caster, target, tickAmount, frequency, now.plus(frequency), tickCount));
        pushEvent(event);
        LogUtils.log(String.format("%s casts %s (effect-over-time) on %s", caster.name(), effect, target.name()));
    }

    public void pushDamageOverTime(Effect effect, Character caster, Character target,
                                   int tickDamage, Duration frequency, int tickCount) {
        pushEffectOverTime(WorldEvent.AmountOverTime.Type.DoT, effect, caster, target,
                tickDamage, frequency, tickCount);
    }

    public void pushHealingOverTime(Effect effect, Character caster, Character target,
                                   int tickHealing, Duration frequency, int tickCount) {
        pushEffectOverTime(WorldEvent.AmountOverTime.Type.HoT, effect, caster, target,
                tickHealing, frequency, tickCount);
    }
}
