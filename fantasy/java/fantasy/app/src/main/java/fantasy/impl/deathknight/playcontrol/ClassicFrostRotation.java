package fantasy.impl.deathknight.playcontrol;

import fantasy.impl.deathknight.DeathKnight;
import fantasy.impl.deathknight.DeathKnightPlayControl;

public class ClassicFrostRotation extends DeathKnightPlayControl {
    public ClassicFrostRotation(DeathKnight deathKnight) {
        super(deathKnight);
    }

    protected void playRotation() {
        castPlagueStrike(false); // apply Blood Plague for Icy Touch
        castIcyTouch(false);
        spamStrikes(false);
    }
}
