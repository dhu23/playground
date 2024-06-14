package skillrotation.impl.deathknight;

import skillrotation.LogUtils;
import skillrotation.impl.AbstractTargetSkill;
import skillrotation.intf.Character;

import java.util.TreeMap;

/**
 * <pre>
 * Instantly strike the enemy,
 * causing 55% weapon damage plus 48 as Frost damage.
 * </pre>
 */
public class FrostStrike extends AbstractTargetSkill {
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
    protected void cast_(Character caster) {
        DeathKnight deathKnight = (DeathKnight) caster;
        if (deathKnight == null) {
            LogUtils.log("not a Death Knight");
            return;
        }
        if (deathKnight.consumeResource((DeathKnightResourceCost) this.cost)) {
            LogUtils.log(String.format("%s casts %s", deathKnight.name(), FROST_STRIKE));
            deathKnight.triggerGlobalCoolDown(this);
            // double base = deathKnight.dealWeaponDamage() * 0.8 + getBonusDamage_();
        }
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
