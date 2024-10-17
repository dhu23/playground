package fantasy.impl.deathknight;

import fantasy.impl.skill.AbstractSkill;
import fantasy.impl.skill.SkillUtils;
import fantasy.impl.data.ImmutableIntegerInterval;
import fantasy.impl.data.IntegerInterval;
import fantasy.impl.spacetime.WorldSpaceTime;
import fantasy.intf.Character;

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
        super(ICY_TOUCH, level, getCost_(), true,6000, 0);
    }

    protected static DeathKnightResourceCost getCost_() {
        TreeMap<DeathKnightResourceCost.RuneType, Integer> runes = new TreeMap<>();
        runes.put(DeathKnightResourceCost.RuneType.Frost, 1);
        return ImmutableDeathKnightResourceCost.of(runes, -10);
    }

    @Override
    protected boolean castOnTarget_(DeathKnight deathKnight, Character target) {
        target.receiveEffect(new FrostFever(deathKnight, target,
                getFrostFeverTickCount(deathKnight), getFrostFeverTickDamage(deathKnight, target)));

        SkillUtils.SkillAmount amount = SkillUtils.calculate(
                deathKnight, target, this, SkillUtils.AmountType.Frost,
                getBaseDamage().sample(WorldSpaceTime.getInstance().getRandomGenerator()),
                getMultiplier(deathKnight, target),
                getCriticalChance(deathKnight, target),
                1.5,
                WorldSpaceTime.getInstance().getRandomGenerator());

        target.receive(amount);
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

    protected int getFrostFeverTickDamage(DeathKnight deathKnight, Character target) {
        double base = 21;
        Optional<Double> blackIceMultiplier = getBlackIceMultiplier(deathKnight, target);
        return (int) (base * blackIceMultiplier.orElse(1.0));
    }

    protected int getFrostFeverTickCount(DeathKnight deathKnight) {
        int count = 5;
        Optional<DeathKnightTalentPool.Epidemic> epidemic = deathKnight.getEpidemic();
        if (epidemic.isPresent()) {
            count += epidemic.get().extraDiseaseTickCount();
        }
        return count;
    }

    protected Optional<Double> getBlackIceMultiplier(DeathKnight deathKnight, Character character) {
        return deathKnight.getBlackIce().map(blackIce -> 1.0 + blackIce.frostAndShadowDamageBonusPercentage());
    }

    protected double getMultiplier(DeathKnight deathKnight, Character target) {
        double multiplier = getBlackIceMultiplier(deathKnight, target).orElse(1.0);

        Optional<DeathKnightTalentPool.GlacierRot> glacierRot = deathKnight.getGlacierRot();
        if (glacierRot.isPresent() &&
                (target.isUnderEffect(FrostFever.FROST_FEVER) || target.isUnderEffect(BloodPlague.BLOOD_PLAGUE))) {
            multiplier *= (1.0 + glacierRot.get().damageBonusPercentage() * 0.01);
        }
        return multiplier;
    }

    protected double getCriticalChance(DeathKnight deathKnight, Character target) {
        double chance = deathKnight.criticalChance();

        Optional<DeathKnightTalentPool.Rime> rime = deathKnight.getRime();
        if (rime.isPresent()) {
            chance += rime.get().criticalStrikePercentageBonus() * 0.01;
        }
        return chance;
    }
}
