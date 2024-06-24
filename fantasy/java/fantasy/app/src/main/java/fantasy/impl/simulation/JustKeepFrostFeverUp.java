package fantasy.impl.simulation;

import fantasy.impl.deathknight.DeathKnight;
import fantasy.intf.DeathKnightPlayControl;

public class JustKeepFrostFeverUp extends DeathKnightPlayControl {
    public JustKeepFrostFeverUp(DeathKnight deathKnight) {
        super(deathKnight);
    }

    @Override
    protected void playRotation() {
        castIcyTouch(false);
    }
}
