package fantasy.impl.simulation;

import fantasy.impl.deathknight.DeathKnight;
import fantasy.intf.DeathKnightPlayControl;

public class ObliterateFrostStrikeBloodStrikeSpam extends DeathKnightPlayControl {
    public ObliterateFrostStrikeBloodStrikeSpam(DeathKnight deathKnight) {
        super(deathKnight);
    }

    protected void playRotation() {
        spamStrikes();
    }
}
