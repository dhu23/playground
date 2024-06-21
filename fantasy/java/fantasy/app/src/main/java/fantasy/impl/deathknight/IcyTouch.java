package fantasy.impl.deathknight;

import fantasy.LogUtils;
import fantasy.impl.Effect;
import fantasy.impl.WorldSpaceTime;
import fantasy.impl.data.ImmutableIntegerInterval;
import fantasy.impl.data.IntegerInterval;
import fantasy.intf.Character;

import java.time.Duration;
import java.util.TreeMap;

/**
 * <pre>
 * Chills the target for 127 to 137 Frost damage and infects them with Frost Fever,
 * a disease that deals periodic damage and reduces melee and ranged attack speed by 14% for 15 sec.
 * Very high threat when in Frost Presence.
 * </pre>
 */
public class IcyTouch extends AbstractDeathKnightTargetSkill {
    public static final String ICY_TOUCH = "Icy Touch";

    public static final IcyTouch LEVEL_1 = new IcyTouch(1);
    public static final IcyTouch LEVEL_2 = new IcyTouch(2);
    public static final IcyTouch LEVEL_3 = new IcyTouch(3);
    public static final IcyTouch LEVEL_4 = new IcyTouch(4);
    public static final IcyTouch LEVEL_5 = new IcyTouch(5);

    public IcyTouch(int level) {
        super(ICY_TOUCH, level, getCost_(), 6000);
    }

    protected static DeathKnightResourceCost getCost_() {
        TreeMap<DeathKnightResourceCost.RuneType, Integer> runes = new TreeMap<>();
        runes.put(DeathKnightResourceCost.RuneType.Frost, 1);
        return ImmutableDeathKnightResourceCost.of(runes, -10);
    }

    @Override
    protected boolean castOnTargetByDeathKnight(DeathKnight deathKnight, Character target) {
        int base = getBaseDamage().sample(WorldSpaceTime.getInstance().getRandomGenerator());

        // inflict the de-buff on the target
        target.receiveEffect(Effect.FrostFever, Duration.ofSeconds(15));

        // set up damage over time
        WorldSpaceTime.getInstance().pushDamageOverTime(Effect.FrostFever, deathKnight, target,
                getFrostFeverTickDamage(), Duration.ofSeconds(3), 5);

        target.sufferDamage(base);
        WorldSpaceTime.getInstance().getLog().report(deathKnight, target, LogUtils.EffectType.Damage, this, base);
        return true;
    }

    protected IntegerInterval getBaseDamage() {
        return switch (level()) {
            case 1 -> ImmutableIntegerInterval.of(127, 137);
            case 2 -> ImmutableIntegerInterval.of(144, 156);
            case 3 -> ImmutableIntegerInterval.of(161, 173);
            case 4 -> ImmutableIntegerInterval.of(187, 203);
            case 5 -> ImmutableIntegerInterval.of(227, 245);
            default -> throw new IllegalStateException("Unexpected value: " + level());
        };
    }

    protected int getFrostFeverTickDamage() {
        return 21;
    }
}
