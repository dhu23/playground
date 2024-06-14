package fantasy.impl.deathknight;

import com.google.common.base.Preconditions;
import fantasy.LogUtils;
import fantasy.impl.AbstractTargetSkill;
import fantasy.intf.Character;
import fantasy.intf.ResourceCost;

public abstract class AbstractDeathKnightTargetSkill extends AbstractTargetSkill {
    public AbstractDeathKnightTargetSkill(String name, int level, DeathKnightResourceCost cost) {
        super(name, level, cost);
    }

    @Override
    protected void castOnTarget_(Character caster, Character target) {
        DeathKnight deathKnight = (DeathKnight) caster;
        if (deathKnight == null) {
            LogUtils.log("not a Death Knight");
            return;
        }

        DeathKnightResourceCost cost = (DeathKnightResourceCost) cost();
        if (!deathKnight.hasResource(cost)) {
            LogUtils.log(String.format("%s doesn't have enough energy to cast %s", deathKnight.name(), name()));
            return;
        }

        LogUtils.log(String.format("%s casts %s", deathKnight.name(), name()));
        Preconditions.checkState(deathKnight.consumeResource(cost), "inconsistent state");
        deathKnight.triggerGlobalCoolDown(this);

        castOnTargetByDeathKnight(deathKnight, target);
    }

    protected abstract void castOnTargetByDeathKnight(DeathKnight deathKnight, Character target);
}
