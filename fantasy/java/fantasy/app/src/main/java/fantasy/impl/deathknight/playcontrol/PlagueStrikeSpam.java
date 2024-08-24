package fantasy.impl.deathknight.playcontrol;

import fantasy.impl.deathknight.DeathKnight;
import fantasy.impl.deathknight.DeathKnightPlayControl;

public class PlagueStrikeSpam extends DeathKnightPlayControl {
    public PlagueStrikeSpam(DeathKnight deathKnight) {
        super(deathKnight);
    }

    @Override
    protected void playRotation() {
        castPlagueStrike(true);
    }
}
