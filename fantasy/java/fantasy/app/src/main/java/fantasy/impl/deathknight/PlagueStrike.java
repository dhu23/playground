package fantasy.impl.deathknight;

import fantasy.impl.AbstractTargetSkill;
import fantasy.impl.SkillUtils;
import fantasy.impl.spacetime.WorldSpaceTime;
import fantasy.intf.Character;

import java.util.Optional;
import java.util.TreeMap;

/**
 * <pre>
 * A vicious strike that deals 50% weapon damage plus 62 and
 * infects the target with Blood Plague, a disease dealing Shadow damage over time.
 * </pre>
 */
public class PlagueStrike extends AbstractTargetSkill {
    public static final String PLAGUE_STRIKE = "Plague Strike";

    public static final PlagueStrike LEVEL_1 = new PlagueStrike(1);
    public static final PlagueStrike LEVEL_2 = new PlagueStrike(2);
    public static final PlagueStrike LEVEL_3 = new PlagueStrike(3);
    public static final PlagueStrike LEVEL_4 = new PlagueStrike(4);
    public static final PlagueStrike LEVEL_5 = new PlagueStrike(5);
    public static final PlagueStrike LEVEL_6 = new PlagueStrike(6);

    public PlagueStrike(int level) {
        super(PLAGUE_STRIKE, level, getCost_(), 4000);
    }

    protected static DeathKnightResourceCost getCost_() {
        TreeMap<DeathKnightResourceCost.RuneType, Integer> runes = new TreeMap<>();
        runes.put(DeathKnightResourceCost.RuneType.Unholy, 1);
        return ImmutableDeathKnightResourceCost.of(runes, -10);
    }


    @Override
    protected boolean castOnTarget_(Character caster, Character target) {
        if (caster instanceof DeathKnight deathKnight) {
            // inflict the de-buff on the target
            target.receiveEffect(new BloodPlague(deathKnight, target,
                    getBloodPlagueTickCount(deathKnight), getBloodPlagueTickDamage()));

            SkillUtils.SkillAmount amount = SkillUtils.calculate(
                    caster, target, this, SkillUtils.AmountType.Physical,
                    caster.dealWeaponDamage() * 0.5 + getBonusDamage_(),
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
            case 1 -> 62;
            case 2 -> 76;
            case 3 -> 89;
            case 4 -> 108;
            case 5 -> 157;
            case 6 -> 189;
            default -> throw new IllegalStateException("Unexpected value: " + level());
        };
    }

    protected int getBloodPlagueTickDamage() {
        // TODO add black ice bonus
        return 25;
    }

    protected int getBloodPlagueTickCount(DeathKnight deathKnight) {
        int count = 5;
        Optional<DeathKnightTalentPool.Epidemic> epidemic = deathKnight.getEpidemic();
        if (epidemic.isPresent()) {
            count += epidemic.get().extraDiseaseTickCount();
        }
        return count;
    }

    protected double getMultiplier(DeathKnight deathKnight, Character target) {
        return 1.0;
    }

    protected double getCriticalChance(DeathKnight deathKnight, Character target) {
        double chance = deathKnight.criticalChance();
        Optional<DeathKnightTalentPool.Annihilation> annihilation = deathKnight.getAnnihilation();
        if (annihilation.isPresent()) {
            chance += annihilation.get().criticalStrikePercentage() * 0.01;
        }
        Optional<DeathKnightTalentPool.ViciousStrikes> viciousStrikes = deathKnight.getViciousStrikes();
        if (viciousStrikes.isPresent()) {
            chance += viciousStrikes.get().criticalPercentageBonus() * 0.01;
        }
        return chance;
    }

    protected double getCriticalMultiplier(DeathKnight deathKnight, Character target) {
        double multiplier = 2.0;
        Optional<DeathKnightTalentPool.ViciousStrikes> viciousStrikes = deathKnight.getViciousStrikes();
        if (viciousStrikes.isPresent()) {
            multiplier += viciousStrikes.get().criticalDamageBonusPercentage() * 0.01;
        }
        return multiplier;
    }
}
