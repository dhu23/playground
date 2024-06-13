package skillrotation.impl.deathknight;

import skillrotation.impl.AbstractSkill;
import skillrotation.impl.AbstractTargetSkill;
import skillrotation.intf.Character;

import java.time.Instant;
import java.util.TreeMap;
import java.util.random.RandomGenerator;

/**
 * <pre>
 * A brutal instant attack that deals 80% weapon damage plus 198,
 * total damage increased 12.5% per each of your diseases on the target,
 * but consumes the diseases.
 * </pre>
 */
public class Obliterate extends AbstractTargetSkill {
    public static final String OBLITERATE = "Obliterate";

    public Obliterate(int level) {
        super(OBLITERATE, level, getCost_());
    }

    protected static DeathKnightResourceCost getCost_() {
        TreeMap<DeathKnightResourceCost.RuneType, Integer> runes = new TreeMap<>();
        runes.put(DeathKnightResourceCost.RuneType.Frost, 1);
        runes.put(DeathKnightResourceCost.RuneType.Unholy, 1);
        return ImmutableDeathKnightResourceCost.of(runes, -15);
    }

    @Override
    public void cast_(Character caster) {
        System.out.println(String.format("%s: casting Obliterate by: %s", Instant.now(), caster.name()));
        double base = caster.dealWeaponDamage() * 0.8 + getBonusDamage_();
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
