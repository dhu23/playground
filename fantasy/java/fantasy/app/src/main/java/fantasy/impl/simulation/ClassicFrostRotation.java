package fantasy.impl.simulation;

import fantasy.impl.deathknight.DeathKnight;
import fantasy.intf.DeathKnightPlayControl;

public class ClassicFrostRotation extends DeathKnightPlayControl {
    public ClassicFrostRotation(DeathKnight deathKnight) {
        super(deathKnight);
    }

    protected void playRotation() {
        castIcyTouchIfNoFrostFever();
        castPlagueStrikeIfNoBloodPlague();
        spamStrikes();
    }
}
