package fantasy.impl.simulation;

import fantasy.impl.deathknight.DeathKnight;
import fantasy.impl.deathknight.DeathKnightPlayControl;

public class ObliterateSpam extends DeathKnightPlayControl {
    public ObliterateSpam(DeathKnight deathKnight) {
        super(deathKnight);
    }

    protected void playRotation() {
        castObliterate(true);
    }
}
