package fantasy.impl.deathknight;

import fantasy.impl.AbstractTargetSkill;
import fantasy.impl.RandomUtils;
import fantasy.impl.SkillUtils;
import fantasy.impl.spacetime.RealTimeImpl1;
import fantasy.impl.spacetime.WorldSpaceTime;
import fantasy.intf.Character;

import java.util.Optional;
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

    public static final Obliterate LEVEL_1 = new Obliterate(1);
    public static final Obliterate LEVEL_2 = new Obliterate(2);
    public static final Obliterate LEVEL_3 = new Obliterate(3);
    public static final Obliterate LEVEL_4 = new Obliterate(4);

    public Obliterate(int level) {
        super(OBLITERATE, level, getCost_(), 0);
    }

    protected static DeathKnightResourceCost getCost_() {
        TreeMap<DeathKnightResourceCost.RuneType, Integer> runes = new TreeMap<>();
        runes.put(DeathKnightResourceCost.RuneType.Frost, 1);
        runes.put(DeathKnightResourceCost.RuneType.Unholy, 1);
        return ImmutableDeathKnightResourceCost.of(runes, -15);
    }


    @Override
    protected boolean castOnTarget_(Character caster, Character target) {
        if (caster instanceof DeathKnight deathKnight) {
            SkillUtils.SkillAmount amount = SkillUtils.calculate(
                    caster, target, this, SkillUtils.AmountType.Physical,
                    caster.dealWeaponDamage() * 0.8 + getBonusDamage_(),
                    getMultiplier(deathKnight, target),
                    getCriticalChance(deathKnight, target),
                    getCriticalMultiplier(deathKnight, target),
                    WorldSpaceTime.getInstance().getRandomGenerator());

            target.receive(amount);

            double chanceToRemoveDiseases = deathKnight.getAnnihilation()
                    .map(DeathKnightTalentPool.Annihilation::chanceToRemoveDiseases).orElse(1.0);
            if (RandomUtils.roll(chanceToRemoveDiseases, WorldSpaceTime.getInstance().getRandomGenerator())) {
                target.removeEffect(FrostFever.FROST_FEVER);
                target.removeEffect(BloodPlague.BLOOD_PLAGUE);
            }

            return true;
        } else {
            return false;
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

    protected double getMultiplier(DeathKnight deathKnight, Character target) {
        // TODO add effect bonuses
        double diseaseEnhancement = 0.0;
        if (target.isUnderEffect(deathKnight, FrostFever.FROST_FEVER)) {
            diseaseEnhancement += 0.125;
        }
        if (target.isUnderEffect(deathKnight, BloodPlague.BLOOD_PLAGUE)) {
            diseaseEnhancement += 0.125;
        }
        return 1.0 + diseaseEnhancement;
    }

    protected double getCriticalChance(DeathKnight deathKnight, Character target) {
        double criticalChance = deathKnight.criticalChance();
        Optional<DeathKnightTalentPool.Rime> rime = deathKnight.getRime();
        if (rime.isPresent()) {
            criticalChance += rime.get().criticalStrikePercentageBonus() * 0.01;
        }
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
        double criticalBonus = 2.0;
        // critical strike bonus
        Optional<DeathKnightTalentPool.GuileOfGorefiend> guileOfGorefiend = deathKnight.getGuileOfGorefiend();
        if (guileOfGorefiend.isPresent()) {
            criticalBonus += guileOfGorefiend.get().criticalStrikeDamageBonusPercentage() * 0.01;
        }
        return criticalBonus;
    }
}
