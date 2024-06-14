package skillrotation.impl.simulation;

import skillrotation.impl.deathknight.BloodStrike;
import skillrotation.impl.deathknight.DeathKnight;
import skillrotation.impl.deathknight.FrostStrike;
import skillrotation.impl.deathknight.Obliterate;
import skillrotation.intf.DeathKnightPlayControl;

public class ObliterateFrostStrikeBloodStrikeSpam implements DeathKnightPlayControl {
    private final DeathKnight deathKnight_;

    public ObliterateFrostStrikeBloodStrikeSpam(DeathKnight deathKnight) {
        deathKnight_ = deathKnight;
    }

    @Override
    public void onRuneCoolDownFinish() {
        playRotation();
    }

    @Override
    public void onSelect() {
        playRotation();
    }

    @Override
    public void onUnselect() {

    }

    @Override
    public void onGlobalCoolDownFinish() {
        playRotation();
    }

    private void playRotation() {
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
