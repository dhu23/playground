package fantasy.impl.deathknight;

import fantasy.LogUtils;
import fantasy.impl.WorldSpaceTime;
import fantasy.intf.Character;

import java.util.TreeMap;

/**
 * <pre>
 * A vicious strike that deals 50% weapon damage plus 62 and
 * infects the target with Blood Plague, a disease dealing Shadow damage over time.
 * </pre>
 */
public class PlagueStrike extends AbstractDeathKnightTargetSkill {
    public static final String PLAGUE_STRIKE = "Plague Strike";

    public static final PlagueStrike LEVEL_1 = new PlagueStrike(1);
    public static final PlagueStrike LEVEL_2 = new PlagueStrike(2);
    public static final PlagueStrike LEVEL_3 = new PlagueStrike(3);
    public static final PlagueStrike LEVEL_4 = new PlagueStrike(4);
    public static final PlagueStrike LEVEL_5 = new PlagueStrike(5);
    public static final PlagueStrike LEVEL_6 = new PlagueStrike(6);

    public PlagueStrike(int level) {
        super(PLAGUE_STRIKE, level, getCost_(), 4000);
    }

    protected static DeathKnightResourceCost getCost_() {
        TreeMap<DeathKnightResourceCost.RuneType, Integer> runes = new TreeMap<>();
        runes.put(DeathKnightResourceCost.RuneType.Unholy, 1);
        return ImmutableDeathKnightResourceCost.of(runes, -10);
    }

    @Override
    protected boolean castOnTargetByDeathKnight(DeathKnight deathKnight, Character target) {
        double base = deathKnight.dealWeaponDamage() * 0.5 + getBonusDamage_();

        // damage mitigation
        base *= (1.0 - target.damageMitigation());

        int damage = (int) base;
        target.sufferDamage(damage);
        WorldSpaceTime.getInstance().getLog().report(deathKnight, target, LogUtils.EffectType.Damage,this, damage);
        return true;
    }

    protected int getBonusDamage_() {
        return switch (level()) {
            case 1 -> 62;
            case 2 -> 76;
            case 3 -> 89;
            case 4 -> 108;
            case 5 -> 157;
            case 6 -> 189;
            default -> throw new IllegalStateException("Unexpected value: " + level());
        };
    }

    protected int getBloodPlagueTickDamage() {
        return 1;
    }
}
