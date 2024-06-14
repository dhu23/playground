package skillrotation.impl.deathknight;

import skillrotation.LogUtils;
import skillrotation.impl.AbstractTargetSkill;
import skillrotation.intf.Character;

import java.util.TreeMap;

/**
 * <pre>
 * A brutal instant attack that deals 80% weapon damage plus 198,
 * total damage increased 12.5% per each of your diseases on the target,
 * but consumes the diseases.
 * </pre>
 */
public class Obliterate extends AbstractTargetSkill {
    public static final String OBLITERATE = "Obliterate";

    private static final Obliterate LEVEL_1 = new Obliterate(1);
    private static final Obliterate LEVEL_2 = new Obliterate(2);
    private static final Obliterate LEVEL_3 = new Obliterate(3);
    private static final Obliterate LEVEL_4 = new Obliterate(4);

    public Obliterate(int level) {
        super(OBLITERATE, level, getCost_());
    }

    public static Obliterate getInstance(int level) {
        return switch (level) {
            case 1 -> LEVEL_1;
            case 2 -> LEVEL_2;
            case 3 -> LEVEL_3;
            case 4 -> LEVEL_4;
            default -> throw new IllegalStateException("Unexpected value: " + level);
        };
    }

    protected static DeathKnightResourceCost getCost_() {
        TreeMap<DeathKnightResourceCost.RuneType, Integer> runes = new TreeMap<>();
        runes.put(DeathKnightResourceCost.RuneType.Frost, 1);
        runes.put(DeathKnightResourceCost.RuneType.Unholy, 1);
        return ImmutableDeathKnightResourceCost.of(runes, -15);
    }

    @Override
    public void cast_(Character caster) {
        DeathKnight deathKnight = (DeathKnight) caster;
        if (deathKnight == null) {
            LogUtils.log("not a Death Knight");
            return;
        }
        if (deathKnight.consumeResource((DeathKnightResourceCost) this.cost)) {
            LogUtils.log(String.format("%s casts %s", deathKnight.name(), OBLITERATE));
            deathKnight.triggerGlobalCoolDown(this);
            double base = deathKnight.dealWeaponDamage() * 0.8 + getBonusDamage_();
        }
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
