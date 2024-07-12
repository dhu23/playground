package fantasy.impl.simulation;

import fantasy.impl.deathknight.DeathKnight;
import fantasy.impl.deathknight.DeathKnightPlayControl;

// the main goal is to test cool-down overwrite
public class IcyTouchSpam extends DeathKnightPlayControl {
    private int counter_;
    private int limit_;

    public IcyTouchSpam(DeathKnight deathKnight, int limit) {
        super(deathKnight);
        this.counter_ = 0;
        this.limit_ = limit;
    }

    @Override
    protected void playRotation() {
        // spam it on every cool down
        if (this.counter_ < this.limit_) {
            if (castIcyTouch(true)) {
                ++this.counter_;
            }
        }
    }
}
