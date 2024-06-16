package fantasy.intf;

import fantasy.impl.deathknight.DeathKnight;

public abstract class DeathKnightPlayControl implements PlayControl{
    protected final DeathKnight deathKnight_;

    public DeathKnightPlayControl(DeathKnight deathKnight) {
        deathKnight_ = deathKnight;
        deathKnight_.setControl(this);
    }

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

    protected abstract void playRotation();
}
