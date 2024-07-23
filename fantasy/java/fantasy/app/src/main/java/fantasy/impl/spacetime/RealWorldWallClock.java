package fantasy.impl.spacetime;

import fantasy.intf.WorldClock;

import java.time.Instant;

public class RealWorldWallClock implements WorldClock {
    public RealWorldWallClock() {

    }

    @Override
    public Instant now() {
        return Instant.now();
    }
}
