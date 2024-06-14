package fantasy.impl.simulation;

import fantasy.impl.deathknight.DeathKnight;
import fantasy.impl.deathknight.Obliterate;
import fantasy.intf.DeathKnightPlayControl;

public class ObliterateSpam implements DeathKnightPlayControl {
    private final DeathKnight deathKnight_;

    public ObliterateSpam(DeathKnight deathKnight) {
        deathKnight_ = deathKnight;
    }

    @Override
    public void onRuneCoolDownFinish() {
        playRotation();
    }

    @Override
    public void onSelect() {
//        System.out.println("calling onSelect");
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
    }
}
