package fantasy.impl.spacetime;

import fantasy.intf.WorldClock;

import java.time.Duration;
import java.time.Instant;

public class SimulationClock implements WorldClock {
    private Instant now_;

    public SimulationClock(Instant initialInstant) {
        now_ = initialInstant;
    }

    @Override
    public Instant now() {
        return now_;
    }

    public boolean moveBy(Duration duration) {
        Instant movedTo = now_.plus(duration);
        if (movedTo.isAfter(now_)) {
            now_ = movedTo;
            return true;
        }
        return false;
    }

    public boolean moveTo(Instant instant) {
        if (now_.isAfter(instant)) {
            return false;
        }
        now_ = instant;
        return true;
    }
}
