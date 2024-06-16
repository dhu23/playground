package fantasy.impl.simulation;

import fantasy.impl.Effect;
import fantasy.impl.deathknight.DeathKnight;
import fantasy.impl.deathknight.IcyTouch;
import fantasy.intf.DeathKnightPlayControl;

public class JustKeepFrostFeverUp extends DeathKnightPlayControl {
    public JustKeepFrostFeverUp(DeathKnight deathKnight) {
        super(deathKnight);
    }

    @Override
    protected void playRotation() {
        deathKnight_.getTarget().ifPresent(target -> {
            if (!target.isUnderEffect(Effect.FrostFever)) {
                deathKnight_.cast(IcyTouch.ICY_TOUCH);
            }
        });
    }
}
