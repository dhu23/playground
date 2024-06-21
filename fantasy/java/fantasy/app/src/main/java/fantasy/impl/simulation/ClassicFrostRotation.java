package fantasy.impl.simulation;

import fantasy.impl.deathknight.DeathKnight;
import fantasy.intf.DeathKnightPlayControl;
import fantasy.intf.Skill;

public class ClassicFrostRotation extends DeathKnightPlayControl {
    public ClassicFrostRotation(DeathKnight deathKnight) {
        super(deathKnight);
    }

    protected void playRotation() {
        castIcyTouchIfNoDot();
        spamStrikes();
    }
}
