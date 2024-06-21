package fantasy.impl.deathknight;

import com.google.common.base.Preconditions;
import fantasy.LogUtils;
import fantasy.impl.AbstractTargetSkill;
import fantasy.intf.Character;

public abstract class AbstractDeathKnightTargetSkill extends AbstractTargetSkill {
    public AbstractDeathKnightTargetSkill(String name, int level, DeathKnightResourceCost cost, int coolDownInMillis) {
        super(name, level, cost, coolDownInMillis);
    }

    @Override
    protected boolean castOnTarget_(Character caster, Character target) {
        DeathKnight deathKnight = (DeathKnight) caster;
        if (deathKnight == null) {
            LogUtils.log("not a Death Knight");
            return false;
        }

        DeathKnightResourceCost cost = (DeathKnightResourceCost) cost();
        if (!deathKnight.hasResource(cost)) {
            LogUtils.log(String.format("%s doesn't have enough energy to cast %s", deathKnight.name(), name()));
            return false;
        }

        LogUtils.log(String.format("%s casts %s", deathKnight.name(), name()));
        Preconditions.checkState(deathKnight.consumeResource(cost), "inconsistent state");

        return castOnTargetByDeathKnight(deathKnight, target);
    }

    protected abstract boolean castOnTargetByDeathKnight(DeathKnight deathKnight, Character target);
}
