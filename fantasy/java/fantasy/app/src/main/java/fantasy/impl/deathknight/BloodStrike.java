package fantasy.impl.deathknight;

import fantasy.LogUtils;

import fantasy.impl.WorldSpaceTime;
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

    public static final BloodStrike LEVEL_1 = new BloodStrike(1);
    public static final BloodStrike LEVEL_2 = new BloodStrike(2);
    public static final BloodStrike LEVEL_3 = new BloodStrike(3);
    public static final BloodStrike LEVEL_4 = new BloodStrike(4);
    public static final BloodStrike LEVEL_5 = new BloodStrike(5);
    public static final BloodStrike LEVEL_6 = new BloodStrike(6);

    protected static DeathKnightResourceCost getCost_() {
        TreeMap<DeathKnightResourceCost.RuneType, Integer> runes = new TreeMap<>();
        runes.put(DeathKnightResourceCost.RuneType.Blood, 1);
        return ImmutableDeathKnightResourceCost.of(runes, -10);
    }

    public BloodStrike(int level) {
        super(BLOOD_STRIKE, level, getCost_(), 0);
    }

    @Override
    protected boolean castOnTargetByDeathKnight(DeathKnight deathKnight, Character target) {
        double base = deathKnight.dealWeaponDamage() * 0.4 + getBonusDamage_();

        double diseaseEnhancement = 0.0;
        if (target.isUnderEffect(deathKnight, FrostFever.FROST_FEVER)) {
            diseaseEnhancement += 0.125;
        }
        if (target.isUnderEffect(deathKnight, BloodPlague.BLOOD_PLAGUE)) {
            diseaseEnhancement += 0.125;
        }
        base *= (1.0 + diseaseEnhancement);

        // damage mitigation
        base *= (1.0 - target.damageMitigation());

        int damage = (int) base;
        target.sufferDamage(damage);
        WorldSpaceTime.getInstance().getLog().report(deathKnight, target, LogUtils.EffectType.Damage,this, damage, false);
        return true;
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
