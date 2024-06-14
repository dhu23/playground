package fantasy.impl;

import com.google.common.annotations.VisibleForTesting;
import fantasy.LogUtils;
import fantasy.impl.deathknight.DeathKnight;
import fantasy.impl.event.*;
import fantasy.intf.Character;
import fantasy.intf.Skill;

import java.io.IOException;
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
                cast.caster().getSkill(cast.spellName()).ifPresent(skill -> {
                    skill.cast(cast.caster());
                });
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
        }
    }

    public void pushGlobalCoolDownEvent(Character caster, Skill skill, long coolDownInMillis) {
        Instant now = Instant.now();
        Event<WorldEvent.EventType, Object> event =
                ImmutableEvent.of(WorldEvent.EventType.GlobalCoolDown,
                        ImmutableGlobalCoolDown.of(caster, skill, now.plusMillis(coolDownInMillis)));
        pushEvent(event);
        LogUtils.log(String.format("%s triggers global cool down by casting %s", caster.name(), skill.name()));
    }

    public void pushRuneCoolDownEvent(DeathKnight dk, int runeId, long coolDownInMillis) {
        Instant now = Instant.now();
        Event<WorldEvent.EventType, Object> event =
                ImmutableEvent.of(WorldEvent.EventType.RuneCoolDown,
                        ImmutableRuneCoolDown.of(dk, runeId, now.plusMillis(10000)));
        pushEvent(event);
        LogUtils.log(String.format("%s triggers cool down for rune %d", dk.name(), runeId));
    }
}
