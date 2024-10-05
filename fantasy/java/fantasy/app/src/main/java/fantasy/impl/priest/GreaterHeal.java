package fantasy.impl.priest;

import fantasy.impl.data.ImmutableIntegerInterval;
import fantasy.impl.data.IntegerInterval;
import fantasy.impl.resource.cost.BaseManaCost;
import fantasy.impl.resource.cost.ImmutableBaseManaCost;
import fantasy.impl.skill.AbstractTargetSkill;
import fantasy.impl.skill.SkillUtils;
import fantasy.impl.spacetime.WorldSpaceTime;
import fantasy.intf.Character;

public class GreaterHeal extends AbstractTargetSkill {
    public static final String GREATER_HEAL = "Greater HEAL";

    public GreaterHeal(int level) {
        super(GREATER_HEAL, level, getCost_(), 0, 3000);
    }

    protected static BaseManaCost getCost_() {
        return ImmutableBaseManaCost.of(32);
    }

    @Override
    protected boolean castOnTarget_(Character caster, Character target) {
        if (caster instanceof Priest priest) {
            SkillUtils.SkillAmount amount = SkillUtils.calculate(
                    caster, target, this,
                    SkillUtils.AmountType.Healing,
                    getBaseHealing_(),
                    getMultiplier_(priest, target),
                    getCriticalChance_(priest, target),
                    getCriticalMultiplier_(priest, target),
                    WorldSpaceTime.getInstance().getRandomGenerator());
            target.receive(amount);
            return true;
        } else {
            return false;
        }
    }

    protected IntegerInterval getBaseHealingRange_() {
        return ImmutableIntegerInterval.of(3980, 4621);
    }

    protected int getBaseHealing_() {
        return getBaseHealingRange_().sample(WorldSpaceTime.getInstance().getRandomGenerator());
    }

    protected double getMultiplier_(Priest priest, Character target) {
        return 1.0;
    }

    protected double getCriticalChance_(Priest priest, Character target) {
        return 0.0;
    }

    protected double getCriticalMultiplier_(Priest priest, Character target) {
        return 1.5;
    }
}
