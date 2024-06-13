package skillrotation.impl.deathknight;

import skillrotation.impl.AbstractTargetSkill;
import skillrotation.intf.Character;
import skillrotation.intf.ResourceCost;

import java.util.TreeMap;

/**
 * <pre>
 * Instantly strike the enemy, causing 40% weapon damage plus 104,
 * total damage increased by 12.5% for each of your diseases on the target.
 * </pre>
 */
public class BloodStrike extends AbstractTargetSkill {
    public static final String NAME = "Blood Strike";
    public static final ResourceCost RESOURCE_COST = getCost_();

    protected static DeathKnightResourceCost getCost_() {
        TreeMap<DeathKnightResourceCost.RuneType, Integer> runes = new TreeMap<>();
        runes.put(DeathKnightResourceCost.RuneType.Blood, 1);
        return ImmutableDeathKnightResourceCost.of(runes, -10);
    }

    public BloodStrike(int level) {
        super(NAME, level, RESOURCE_COST);
    }

    @Override
    public void cast_(Character caster) {
    }

    protected int getBonusDamage_() {
        return switch (level()) {
            case 1 -> 104;
            case 2 -> 118;
            case 3 -> 139;
            case 4 -> 164;
            case 5 -> 250;
            case 6 -> 306;
            default -> throw new IllegalStateException("Unexpected value: " + level());
        };
    }
}
