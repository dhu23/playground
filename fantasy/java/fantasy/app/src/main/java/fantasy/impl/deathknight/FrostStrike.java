package fantasy.impl.deathknight;

import fantasy.impl.AbstractTargetSkill;
import fantasy.impl.SkillUtils;
import fantasy.impl.spacetime.WorldSpaceTime;
import fantasy.intf.Character;

import java.util.Optional;
import java.util.TreeMap;

/**
 * <pre>
 * Instantly strike the enemy,
 * causing 55% weapon damage plus 48 as Frost damage.
 * </pre>
 */
public class FrostStrike extends AbstractTargetSkill {
    public static final String FROST_STRIKE = "Frost Strike";

    public static final FrostStrike LEVEL_1 = new FrostStrike(1);
    public static final FrostStrike LEVEL_2 = new FrostStrike(2);
    public static final FrostStrike LEVEL_3 = new FrostStrike(3);
    public static final FrostStrike LEVEL_4 = new FrostStrike(4);
    public static final FrostStrike LEVEL_5 = new FrostStrike(5);
    public static final FrostStrike LEVEL_6 = new FrostStrike(6);

    public FrostStrike(int level) {
        super(FROST_STRIKE, level, getCost_(), 0);
    }

    protected static DeathKnightResourceCost getCost_() {
        return ImmutableDeathKnightResourceCost.of(new TreeMap<>(), 40);
    }

    @Override
    protected boolean castOnTarget_(Character caster, Character target) {
        if (caster instanceof DeathKnight deathKnight) {
            SkillUtils.SkillAmount amount = SkillUtils.calculate(
                    caster, target, this, SkillUtils.AmountType.Frost,
                    caster.dealWeaponDamage() * 0.55 + getBonusDamage_(),
                    getMultiplier(deathKnight, target),
                    getCriticalChance(deathKnight, target),
                    getCriticalMultiplier(deathKnight, target),
                    WorldSpaceTime.getInstance().getRandomGenerator());

            target.receive(amount);
            return true;
        } else {
            return false;
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

    protected double getMultiplier(DeathKnight deathKnight, Character target) {
        double multiplier = 1.0;
        Optional<DeathKnightTalentPool.BlackIce> blackIce = deathKnight.getBlackIce();
        if (blackIce.isPresent()) {
            multiplier *= (1.0 + blackIce.get().frostAndShadowDamageBonusPercentage());
        }

        Optional<DeathKnightTalentPool.GlacierRot> glacierRot = deathKnight.getGlacierRot();
        if (glacierRot.isPresent() &&
                (target.isUnderEffect(FrostFever.FROST_FEVER) || target.isUnderEffect(BloodPlague.BLOOD_PLAGUE))) {
            multiplier *= (1.0 + glacierRot.get().damageBonusPercentage() * 0.01);
        }

        Optional<DeathKnightTalentPool.BloodOfTheNorth> bloodOfTheNorth = deathKnight.getBloodOfTheNorth();
        if (bloodOfTheNorth.isPresent()) {
            multiplier *= (1.0 + bloodOfTheNorth.get().damageBonusPercentage() * 0.01);
        }
        return multiplier;
    }

    protected double getCriticalChance(DeathKnight deathKnight, Character target) {
        if (deathKnight.isUnderEffect(KillingMachineEffect.KILLING_MACHINE)) {
            deathKnight.removeEffect(KillingMachineEffect.KILLING_MACHINE);
            return 1.0;
        } else {
            double chance = deathKnight.criticalChance();
            Optional<DeathKnightTalentPool.Annihilation> annihilation = deathKnight.getAnnihilation();
            if (annihilation.isPresent()) {
                chance += annihilation.get().criticalStrikePercentage() * 0.01;
            }
            return chance;
        }
    }

    protected double getCriticalMultiplier(DeathKnight deathKnight, Character target) {
        double multipler = 2.0;
        Optional<DeathKnightTalentPool.GuileOfGorefiend> guileOfGorefiend = deathKnight.getGuileOfGorefiend();
        if (guileOfGorefiend.isPresent()) {
            multipler += guileOfGorefiend.get().criticalStrikeDamageBonusPercentage() * 0.01;
        }
        return multipler;
    }
}
