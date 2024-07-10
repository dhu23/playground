package fantasy.impl.deathknight;

import fantasy.LogUtils;
import fantasy.impl.WorldSpaceTime;
import fantasy.intf.Character;

import java.util.Optional;
import java.util.TreeMap;

/**
 * <pre>
 * Instantly strike the enemy,
 * causing 55% weapon damage plus 48 as Frost damage.
 * </pre>
 */
public class FrostStrike extends AbstractDeathKnightTargetSkill {
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
    protected boolean castOnTargetByDeathKnight(DeathKnight deathKnight, Character target) {
        double base = deathKnight.dealWeaponDamage() * 0.55 + getBonusDamage_();

        // talent bonuses
        Optional<DeathKnightTalentPool.BlackIce> blackIce = deathKnight.getBlackIce();
        if (blackIce.isPresent()) {
            double bonus = 1.0 + blackIce.get().frostAndShadowDamageBonusPercentage();
            base *= bonus;
        }

        Optional<DeathKnightTalentPool.GlacierRot> glacierRot = deathKnight.getGlacierRot();
        if (glacierRot.isPresent() && target.isUnderEffect(FrostFever.FROST_FEVER)) {
            base *= (1.0 + glacierRot.get().damageBonusPercentage() * 0.01);
        }

        Optional<DeathKnightTalentPool.BloodOfTheNorth> bloodOfTheNorth = deathKnight.getBloodOfTheNorth();
        if (bloodOfTheNorth.isPresent()) {
            base *= (1.0 + bloodOfTheNorth.get().damageBonusPercentage() * 0.01);
        }

        // Frost Strike is not affected by target's damage mitigation

        double criticalBonus = 2.0;
        // critical strike bonus
        Optional<DeathKnightTalentPool.GuileOfGorefiend> guileOfGorefiend = deathKnight.getGuileOfGorefiend();
        if (guileOfGorefiend.isPresent()) {
            criticalBonus += guileOfGorefiend.get().criticalStrikeDamageBonusPercentage() * 0.01;
        }

        boolean critical = false;
        if (deathKnight.isUnderEffect(KillingMachineEffect.KILLING_MACHINE)) {
            // TODO make it a coefficient from DeathKnight class
            base *= criticalBonus;
            critical = true;
            deathKnight.removeEffect(KillingMachineEffect.KILLING_MACHINE);
        }

        int damage = (int) base;
        target.sufferDamage(damage);
        WorldSpaceTime.getInstance().getLog().report(deathKnight, target, LogUtils.EffectType.Damage,this, damage, critical);
        return true;
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
