package fantasy.impl.deathknight;

import fantasy.LogUtils;
import fantasy.impl.AbstractTargetSkill;
import fantasy.intf.Character;

import java.util.TreeMap;

/**
 * <pre>
 * Instantly strike the enemy, causing 40% weapon damage plus 104,
 * total damage increased by 12.5% for each of your diseases on the target.
 * </pre>
 */
public class BloodStrike extends AbstractDeathKnightTargetSkill {
    public static final String BLOOD_STRIKE = "Blood Strike";

    private static final BloodStrike LEVEL_1 = new BloodStrike(1);
    private static final BloodStrike LEVEL_2 = new BloodStrike(2);
    private static final BloodStrike LEVEL_3 = new BloodStrike(3);
    private static final BloodStrike LEVEL_4 = new BloodStrike(4);
    private static final BloodStrike LEVEL_5 = new BloodStrike(5);
    private static final BloodStrike LEVEL_6 = new BloodStrike(6);

    protected static DeathKnightResourceCost getCost_() {
        TreeMap<DeathKnightResourceCost.RuneType, Integer> runes = new TreeMap<>();
        runes.put(DeathKnightResourceCost.RuneType.Blood, 1);
        return ImmutableDeathKnightResourceCost.of(runes, -10);
    }

    public BloodStrike(int level) {
        super(BLOOD_STRIKE, level, getCost_());
    }

    public static BloodStrike getInstance(int level) {
        return switch (level) {
            case 1 -> LEVEL_1;
            case 2 -> LEVEL_2;
            case 3 -> LEVEL_3;
            case 4 -> LEVEL_4;
            case 5 -> LEVEL_5;
            case 6 -> LEVEL_6;
            default -> throw new IllegalStateException("Unexpected value: " + level);
        };
    }

    @Override
    protected void castOnTargetByDeathKnight(DeathKnight deathKnight, Character target) {
        double base = deathKnight.dealWeaponDamage() * 0.8 + getBonusDamage_();
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
