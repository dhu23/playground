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
    public static final String GREATER_HEAL = "Greater Heal";

    public static final GreaterHeal LEVEL_1 = new GreaterHeal(1);
    public static final GreaterHeal LEVEL_2 = new GreaterHeal(2);
    public static final GreaterHeal LEVEL_3 = new GreaterHeal(3);
    public static final GreaterHeal LEVEL_4 = new GreaterHeal(4);
    public static final GreaterHeal LEVEL_5 = new GreaterHeal(5);
    public static final GreaterHeal LEVEL_6 = new GreaterHeal(6);
    public static final GreaterHeal LEVEL_7 = new GreaterHeal(7);
    public static final GreaterHeal LEVEL_8 = new GreaterHeal(8);
    public static final GreaterHeal LEVEL_9 = new GreaterHeal(9);

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
        return switch (level()) {
            case 1 -> ImmutableIntegerInterval.of(924, 1039);
            case 2 -> ImmutableIntegerInterval.of(1178, 1318);
            case 3 -> ImmutableIntegerInterval.of(1470, 1642);
            case 4 -> ImmutableIntegerInterval.of(1835, 2044);
            case 5 -> ImmutableIntegerInterval.of(2006, 2235);
            case 6 -> ImmutableIntegerInterval.of(2107, 2444);
            case 7 -> ImmutableIntegerInterval.of(2433, 2822);
            case 8 -> ImmutableIntegerInterval.of(3447, 3998);
            case 9 -> ImmutableIntegerInterval.of(3980, 4621);
            default -> throw new IllegalStateException("Unexpected value: " + level());
        };
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
