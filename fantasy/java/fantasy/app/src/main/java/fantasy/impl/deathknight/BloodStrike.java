package fantasy.impl.deathknight;

import fantasy.impl.AbstractTargetSkill;
import fantasy.impl.SkillUtils;
import fantasy.impl.WorldSpaceTime;
import fantasy.intf.Character;

import java.util.Optional;
import java.util.TreeMap;

/**
 * <pre>
 * Instantly strike the enemy, causing 40% weapon damage plus 104,
 * total damage increased by 12.5% for each of your diseases on the target.
 * </pre>
 */
public class BloodStrike extends AbstractTargetSkill {
    public static final String BLOOD_STRIKE = "Blood Strike";

    public static final BloodStrike LEVEL_1 = new BloodStrike(1);
    public static final BloodStrike LEVEL_2 = new BloodStrike(2);
    public static final BloodStrike LEVEL_3 = new BloodStrike(3);
    public static final BloodStrike LEVEL_4 = new BloodStrike(4);
    public static final BloodStrike LEVEL_5 = new BloodStrike(5);
    public static final BloodStrike LEVEL_6 = new BloodStrike(6);

    protected static DeathKnightResourceCost getCost_() {
        TreeMap<DeathKnightResourceCost.RuneType, Integer> runes = new TreeMap<>();
        runes.put(DeathKnightResourceCost.RuneType.Blood, 1);
        return ImmutableDeathKnightResourceCost.of(runes, -10);
    }

    public BloodStrike(int level) {
        super(BLOOD_STRIKE, level, getCost_(), 0);
    }

    @Override
    protected boolean castOnTarget_(Character caster, Character target) {
        if (caster instanceof DeathKnight deathKnight) {
            SkillUtils.SkillAmount amount = SkillUtils.calculate(
                    caster, target, this,
                    SkillUtils.AmountType.Physical,
                    caster.dealWeaponDamage() * 0.4 + getBonusDamage_(),
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
            case 1 -> 104;
            case 2 -> 118;
            case 3 -> 139;
            case 4 -> 164;
            case 5 -> 250;
            case 6 -> 306;
            default -> throw new IllegalStateException("Unexpected value: " + level());
        };
    }

    protected double getMultiplier(DeathKnight deathKnight, Character target) {
        double diseaseEnhancement = 0.0;
        if (target.isUnderEffect(deathKnight, FrostFever.FROST_FEVER)) {
            diseaseEnhancement += 0.125;
        }
        if (target.isUnderEffect(deathKnight, BloodPlague.BLOOD_PLAGUE)) {
            diseaseEnhancement += 0.125;
        }

        double bonusMultiplier = 1.0 + diseaseEnhancement;
        Optional<DeathKnightTalentPool.BloodOfTheNorth> bloodOfTheNorth = deathKnight.getBloodOfTheNorth();
        if (bloodOfTheNorth.isPresent()) {
            bonusMultiplier *= (1.0 + bloodOfTheNorth.get().damageBonusPercentage() * 0.01);
        }

        return bonusMultiplier;
    }

    protected double getCriticalChance(DeathKnight deathKnight, Character target) {
        double criticalChance = deathKnight.criticalChance();
        Optional<DeathKnightTalentPool.Subversion> subversion = deathKnight.getSubversion();
        if (subversion.isPresent()) {
            criticalChance += subversion.get().criticalChanceBonusPercentage() * 0.01;
        }
        Optional<DeathKnightTalentPool.Annihilation> annihilation = deathKnight.getAnnihilation();
        if (annihilation.isPresent()) {
            criticalChance += annihilation.get().criticalStrikePercentage() * 0.01;
        }
        return criticalChance;
    }

    protected double getCriticalMultiplier(DeathKnight deathKnight, Character target) {
        double multiplier = 2.0;
        Optional<DeathKnightTalentPool.GuileOfGorefiend> guileOfGorefiend = deathKnight.getGuileOfGorefiend();
        if (guileOfGorefiend.isPresent()) {
            multiplier += guileOfGorefiend.get().criticalStrikeDamageBonusPercentage() * 0.01;
        }
        return multiplier;
    }
}
