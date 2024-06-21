package fantasy.intf;

import fantasy.LogUtils;
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
        LogUtils.log("PlayControl: Global Cool Down is clear");
        playRotation();
    }

    @Override
    public void onSkillCoolDownFinish(Skill skill) {
        LogUtils.log(String.format("PlayControl: %s's Cool Down is clear", deathKnight_.name()));
        playRotation();
    }

    protected abstract void playRotation();
}
