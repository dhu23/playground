package fantasy.impl.deathknight;

import fantasy.LogUtils;
import fantasy.impl.WorldSpaceTime;
import fantasy.intf.Character;

import java.util.TreeMap;

/**
 * <pre>
 * A brutal instant attack that deals 80% weapon damage plus 198,
 * total damage increased 12.5% per each of your diseases on the target,
 * but consumes the diseases.
 * </pre>
 */
public class Obliterate extends AbstractDeathKnightTargetSkill {
    public static final String OBLITERATE = "Obliterate";

    public static final Obliterate LEVEL_1 = new Obliterate(1);
    public static final Obliterate LEVEL_2 = new Obliterate(2);
    public static final Obliterate LEVEL_3 = new Obliterate(3);
    public static final Obliterate LEVEL_4 = new Obliterate(4);

    public Obliterate(int level) {
        super(OBLITERATE, level, getCost_(), 0);
    }

    protected static DeathKnightResourceCost getCost_() {
        TreeMap<DeathKnightResourceCost.RuneType, Integer> runes = new TreeMap<>();
        runes.put(DeathKnightResourceCost.RuneType.Frost, 1);
        runes.put(DeathKnightResourceCost.RuneType.Unholy, 1);
        return ImmutableDeathKnightResourceCost.of(runes, -15);
    }

    @Override
    protected boolean castOnTargetByDeathKnight(DeathKnight deathKnight, Character target) {
        double base = deathKnight.dealWeaponDamage() * 0.8 + getBonusDamage_();
        // TODO add effect bonuses
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
            case 1 -> 198;
            case 2 -> 244;
            case 3 -> 382;
            case 4 -> 467;
            default -> throw new IllegalStateException("Unexpected value: " + level());
        };
    }
}
