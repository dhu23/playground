package skillrotation.impl;

import com.google.common.annotations.VisibleForTesting;
import skillrotation.impl.event.Event;
import skillrotation.impl.event.EventQueue;
import skillrotation.impl.event.WorldEvent;

import java.time.Instant;


public class WorldSpaceTime {
    private static final WorldSpaceTime INSTANCE = new WorldSpaceTime();
    private final EventQueue<Event<WorldEvent.EventType, Object>> eventQueue_;

    public static WorldSpaceTime getInstance() {
        return INSTANCE;
    }

    public WorldSpaceTime() {
        this.eventQueue_ = new EventQueue<>(this::onEvent_);
        this.eventQueue_.start();
    }

    public void stop() {
        this.eventQueue_.stop();
    }

    public void pushEvent(Event<WorldEvent.EventType, Object> event) {
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
                    gcd.caster().clearGlobalCoolDown();
                }
            }
        }
    }
}
