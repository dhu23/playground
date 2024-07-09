package fantasy.intf;

import fantasy.LogUtils;
import fantasy.impl.deathknight.*;

import java.util.Optional;

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
//        LogUtils.log("PlayControl: Global Cool Down is clear");
        playRotation();
    }

    @Override
    public void onSkillCoolDownFinish(Skill skill) {
        LogUtils.log(String.format("PlayControl: %s's %s Cool Down is clear", deathKnight_.name(), skill.name()));
        playRotation();
    }

    @Override
    public void onEffectExpiration(Character target, Effect effect) {
        playRotation();
    }

    protected boolean castIcyTouch(boolean ignoreFrostFever) {
        Optional<Character> targetOptional = deathKnight_.getTarget();
        if (targetOptional.isEmpty()) {
            return false;
        }
        Character target = targetOptional.get();
//        LogUtils.log(String.format("%s is under frost fever: %s", target.name(), target.isUnderEffect(FrostFever.FROST_FEVER)));
        if (ignoreFrostFever || !target.isUnderEffect(FrostFever.FROST_FEVER)) {
            return deathKnight_.cast(IcyTouch.ICY_TOUCH);
        }
        return false;
    }

    protected boolean castPlagueStrike(boolean ignoreBloodPlague) {
        Optional<Character> targetOptional = deathKnight_.getTarget();
        if (targetOptional.isEmpty()) {
            return false;
        }
        Character target = targetOptional.get();
        if (ignoreBloodPlague || !target.isUnderEffect(BloodPlague.BLOOD_PLAGUE)) {
            deathKnight_.cast(PlagueStrike.PLAGUE_STRIKE);
            return true;
        }
        return false;
    }

    protected void castObliterate() {
        deathKnight_.cast(Obliterate.OBLITERATE);
    }

    protected void castFrostStrike() {
        deathKnight_.cast(FrostStrike.FROST_STRIKE);
    }

    protected void castBloodStrike() {
        deathKnight_.cast(BloodStrike.BLOOD_STRIKE);
    }

    protected void spamStrikes() {
        if (deathKnight_.isUnderEffect(KillingMachineEffect.KILLING_MACHINE)) {
            castFrostStrike();
        }
        if (deathKnight_.getDeathKnightResource().hasRune(DeathKnightResourceCost.RuneType.Blood)) {
            castBloodStrike();
        }
        castObliterate();
        castFrostStrike();
    }

    protected abstract void playRotation();
}
