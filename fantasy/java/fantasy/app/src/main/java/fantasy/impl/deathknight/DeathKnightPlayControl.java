package fantasy.impl.deathknight;

import fantasy.impl.LogUtils;
import fantasy.intf.Character;
import fantasy.intf.Effect;
import fantasy.intf.PlayControl;
import fantasy.intf.Skill;

import java.util.Optional;

public abstract class DeathKnightPlayControl implements PlayControl {
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
        deathKnight_.turnOnAutoAttack();
        playRotation();
    }

    @Override
    public void onUnselect() {

    }

    @Override
    public void onGlobalCoolDownFinish() {
        playRotation();
    }

    @Override
    public void onSkillCoolDownFinish(Skill skill) {
//        LogUtils.log(String.format("PlayControl: %s's %s Cool Down is clear", deathKnight_.name(), skill.name()));
        playRotation();
    }

    @Override
    public void onEffectExpiration(Character target, Effect effect) {
        playRotation();
    }

    @Override
    public void onSuccessfulCast(Skill skill) {
        // no need to act because of gcd
    }

    protected boolean castIcyTouch(boolean ignoreFrostFever) {
        Optional<Character> targetOptional = deathKnight_.getTarget();
        if (targetOptional.isEmpty()) {
            return false;
        }
        Character target = targetOptional.get();
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

    protected void castObliterate(boolean blindly) {
        deathKnight_.cast(Obliterate.OBLITERATE);
    }

    protected void castFrostStrike(boolean blindly) {
        deathKnight_.cast(FrostStrike.FROST_STRIKE);
    }

    protected void castBloodStrike(boolean blindly) {
        deathKnight_.cast(BloodStrike.BLOOD_STRIKE);
    }

    protected void spamStrikes(boolean blindly) {
        Optional<Character> targetOptional = deathKnight_.getTarget();
        if (targetOptional.isEmpty()) {
            return;
        }

        Character target = targetOptional.get();
        boolean hasBloodPlague = target.isUnderEffect(BloodPlague.BLOOD_PLAGUE);
        boolean hasFrostFever = target.isUnderEffect(FrostFever.FROST_FEVER);

        if (deathKnight_.isUnderEffect(KillingMachineEffect.KILLING_MACHINE)) {
            if (blindly || (hasFrostFever || hasBloodPlague)) {
                castFrostStrike(blindly);
            }
        }
        if (deathKnight_.getDeathKnightResource().hasRune(DeathKnightResourceCost.RuneType.Blood)) {
            if (blindly || (hasFrostFever && hasBloodPlague)) {
                castBloodStrike(blindly);
            }
        }
        if (blindly || (hasFrostFever && hasBloodPlague)) {
            castObliterate(blindly);
        }
        if (blindly || (hasFrostFever || hasBloodPlague)) {
            castFrostStrike(blindly);
        }
    }

    protected abstract void playRotation();
}
