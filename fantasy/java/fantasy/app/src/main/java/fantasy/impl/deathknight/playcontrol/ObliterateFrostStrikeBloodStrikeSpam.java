package fantasy.impl.deathknight.playcontrol;

import fantasy.impl.deathknight.DeathKnight;
import fantasy.impl.deathknight.DeathKnightPlayControl;

public class ObliterateFrostStrikeBloodStrikeSpam extends DeathKnightPlayControl {
    public ObliterateFrostStrikeBloodStrikeSpam(DeathKnight deathKnight) {
        super(deathKnight);
    }

    protected void playRotation() {
        spamStrikes(true);
    }
}
