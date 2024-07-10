package fantasy.impl.deathknight;

import fantasy.LogUtils;
import fantasy.impl.RandomUtils;
import fantasy.impl.WorldSpaceTime;
import fantasy.impl.data.ImmutableIntegerInterval;
import fantasy.impl.data.IntegerInterval;
import fantasy.intf.Character;

import java.time.Duration;
import java.util.Optional;
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
        double base = getBaseDamage().sample(WorldSpaceTime.getInstance().getRandomGenerator());
        double frostFeverBase = getFrostFeverTickDamage();

        // talent bonuses
        Optional<DeathKnightTalentPool.BlackIce> blackIce = deathKnight.getBlackIce();
        if (blackIce.isPresent()) {
            double bonus = 1.0 + blackIce.get().frostAndShadowDamageBonusPercentage();
            base *= bonus;
            frostFeverBase *= bonus;
        }

        // inflict the de-buff on the target immediately
        target.receiveEffect(new FrostFever(deathKnight, target, (int) frostFeverBase));

        Optional<DeathKnightTalentPool.GlacierRot> glacierRot = deathKnight.getGlacierRot();
        if (glacierRot.isPresent() && target.isUnderEffect(FrostFever.FROST_FEVER)) {
            base *= (1.0 + glacierRot.get().damageBonusPercentage() * 0.01);
        }

        double criticalChance = deathKnight.criticalChance();
        Optional<DeathKnightTalentPool.Rime> rime = deathKnight.getRime();
        if (rime.isPresent()) {
            criticalChance += rime.get().criticalStrikePercentageBonus() * 0.01;
        }
        boolean critical = false;
        if (RandomUtils.roll(criticalChance, WorldSpaceTime.getInstance().getRandomGenerator())) {
            base *= 1.5; // spell critically hits for 150% damage
            critical = true;
        }

        int damage = (int) base;
        target.sufferDamage(damage);
        WorldSpaceTime.getInstance().getLog().report(deathKnight, target, LogUtils.EffectType.Damage, this, damage, critical);
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
