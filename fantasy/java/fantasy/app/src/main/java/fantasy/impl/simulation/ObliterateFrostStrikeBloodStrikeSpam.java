package fantasy.impl.simulation;

import fantasy.impl.deathknight.BloodStrike;
import fantasy.impl.deathknight.DeathKnight;
import fantasy.impl.deathknight.FrostStrike;
import fantasy.impl.deathknight.Obliterate;
import fantasy.intf.DeathKnightPlayControl;

public class ObliterateFrostStrikeBloodStrikeSpam extends DeathKnightPlayControl {
    public ObliterateFrostStrikeBloodStrikeSpam(DeathKnight deathKnight) {
        super(deathKnight);
    }

    protected void playRotation() {
        if (deathKnight_.canCast(Obliterate.OBLITERATE)) {
            deathKnight_.cast(Obliterate.OBLITERATE);
        }
        if (deathKnight_.canCast(FrostStrike.FROST_STRIKE)) {
            deathKnight_.cast(FrostStrike.FROST_STRIKE);
        }
        if (deathKnight_.canCast(BloodStrike.BLOOD_STRIKE)) {
            deathKnight_.cast(BloodStrike.BLOOD_STRIKE);
        }
    }
}
