package fantasy.impl.simulation;

import fantasy.impl.deathknight.DeathKnight;
import fantasy.impl.deathknight.Obliterate;
import fantasy.intf.DeathKnightPlayControl;

public class ObliterateSpam extends DeathKnightPlayControl {
    public ObliterateSpam(DeathKnight deathKnight) {
        super(deathKnight);
    }

    protected void playRotation() {
        castObliterate();
    }
}
