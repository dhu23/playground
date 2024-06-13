package skillrotation.impl.simulation;

import skillrotation.impl.deathknight.DeathKnight;
import skillrotation.intf.DeathKnightPlayControl;
import skillrotation.intf.PlayControl;

public class ObliterateSpam implements DeathKnightPlayControl {
    private final DeathKnight deathKnight_;

    public ObliterateSpam(DeathKnight deathKnight) {
        deathKnight_ = deathKnight;
    }

    @Override
    public void onSelect() {
        System.out.println("calling onSelect");
        deathKnight_.cast("Obliterate");
    }

    @Override
    public void onUnselect() {

    }

    @Override
    public void onGlobalCoolDownFinish() {
        playRotation();
    }

    @Override
    public void onRuneCoolDownFinish() {
        playRotation();
    }

    private void playRotation() {
        deathKnight_.cast("Obliterate");
    }
}
