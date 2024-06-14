package fantasy.impl.deathknight;

import fantasy.LogUtils;
import fantasy.impl.AbstractTargetSkill;
import fantasy.impl.WorldSpaceTime;
import fantasy.intf.Character;

import java.util.TreeMap;

/**
 * <pre>
 * Instantly strike the enemy,
 * causing 55% weapon damage plus 48 as Frost damage.
 * </pre>
 */
public class FrostStrike extends AbstractDeathKnightTargetSkill {
    public static final String FROST_STRIKE = "Frost Strike";

    private static final FrostStrike LEVEL_1 = new FrostStrike(1);
    private static final FrostStrike LEVEL_2 = new FrostStrike(2);
    private static final FrostStrike LEVEL_3 = new FrostStrike(3);
    private static final FrostStrike LEVEL_4 = new FrostStrike(4);
    private static final FrostStrike LEVEL_5 = new FrostStrike(5);
    private static final FrostStrike LEVEL_6 = new FrostStrike(6);

    public FrostStrike(int level) {
        super(FROST_STRIKE, level, getCost_());
    }

    public static FrostStrike getInstance(int level) {
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

    protected static DeathKnightResourceCost getCost_() {
        return ImmutableDeathKnightResourceCost.of(new TreeMap<>(), 40);
    }

    @Override
    protected void castOnTargetByDeathKnight(DeathKnight deathKnight, Character target) {
        double base = deathKnight.dealWeaponDamage() * 0.55 + getBonusDamage_();

        // Frost Strike is not affected by target's damage mitigation

        int damage = (int) base;
        target.sufferDamage(damage);
        WorldSpaceTime.getInstance().getLog().report(deathKnight, target, LogUtils.EffectType.Damage,this, damage);
    }

    protected int getBonusDamage_() {
        return switch (level()) {
            case 1 -> 48;
            case 2 -> 57;
            case 3 -> 63;
            case 4 -> 78;
            case 5 -> 111;
            case 6 -> 138;
            default -> throw new IllegalStateException("Unexpected value: " + level());
        };
    }
}
