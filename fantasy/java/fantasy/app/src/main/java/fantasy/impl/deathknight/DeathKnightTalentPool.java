package fantasy.impl.deathknight;

import fantasy.impl.AbstractTalent;

import java.util.Set;

public class DeathKnightTalentPool {
    public static final String SUBVERSION = "Subversion";

    /**
     * Increases the critical strike chance of Blood Strike, Scourge Strike, Heart Strike and Obliterate by 3/6/9%,
     * and reduces threat generated while in Blood or Unholy Presence by 8/16/25%.
     */
    public static class Subversion extends AbstractTalent {
        public static final Subversion LEVEL_1 = new Subversion(1);
        public static final Subversion LEVEL_2 = new Subversion(2);
        public static final Subversion LEVEL_3 = new Subversion(3);

        public Subversion(int rank) {
            super(SUBVERSION, rank);
        }

        public int criticalChanceBonusPercentage() {
            return 3 * rank();
        }
    }

    public static final String TWO_HANDED_WEAPON_SPECIALIZATION = "Two-Handed Weapon Specialization";

    // TODO check weapon type to apply talent
    /**
     * Increases the damage you deal with two-handed melee weapons by 2/4%.
     */
    public static class TwoHandedWeaponSpecialization extends AbstractTalent {
        public static final TwoHandedWeaponSpecialization LEVEL_1 = new TwoHandedWeaponSpecialization(1);
        public static final TwoHandedWeaponSpecialization LEVEL_2 = new TwoHandedWeaponSpecialization(2);

        public TwoHandedWeaponSpecialization(int rank) {
            super(TWO_HANDED_WEAPON_SPECIALIZATION, rank);
        }

        public int bonusDamage() {
            return 2 * rank();
        }
    }

    public static final String RUNIC_POWER_MASTERY = "Runic Power Mastery";

    /**
     * Increases your maximum Runic Power by 15/30.
     */
    public static class RunicPowerMastery extends AbstractTalent {
        public static final RunicPowerMastery LEVEL_1 = new RunicPowerMastery(1);
        public static final RunicPowerMastery LEVEL_2 = new RunicPowerMastery(2);

        public RunicPowerMastery(int rank) {
            super(RUNIC_POWER_MASTERY, rank);
        }

        public int runicPowerBonus() {
            return rank() * 15;
        }
    }

    public static final String BLACK_ICE = "Black Ice";

    /**
     * Increases your Frost and Shadow damage by 2/4/6/8/10%.
     */
    public static class BlackIce extends AbstractTalent {
        public static final BlackIce LEVEL_1 = new BlackIce(1);
        public static final BlackIce LEVEL_2 = new BlackIce(2);
        public static final BlackIce LEVEL_3 = new BlackIce(3);
        public static final BlackIce LEVEL_4 = new BlackIce(4);
        public static final BlackIce LEVEL_5 = new BlackIce(5);

        public BlackIce(int rank) {
            super(BLACK_ICE, rank);
        }

        public double frostAndShadowDamageBonusPercentage() {
            return rank() * 0.02;
        }
    }

    public static final String ANNIHILATION = "Annihilation";

    /**
     * Increases the critical strike chance of your melee special abilities by 1/2/3%.
     * In addition, there is a 33/66/100% chance that your Obliterate will do its damage without consuming diseases.
     */
    public static class Annihilation extends AbstractTalent {
        public static final Annihilation LEVEL_1 = new Annihilation(1);
        public static final Annihilation LEVEL_2 = new Annihilation(2);
        public static final Annihilation LEVEL_3 = new Annihilation(3);

        public Annihilation(int rank) {
            super(ANNIHILATION, rank);
        }

        public int criticalStrikePercentage() {
            return rank();
        }

        public int percentageOfLeavingDiseasesAlong() {
            return switch (rank()) {
                case 1 -> 33;
                case 2 -> 66;
                case 3 -> 100;
                default -> throw new IllegalStateException("Unexpected value: " + rank());
            };
        }

        public double chanceToRemoveDiseases() {
            return (100 - percentageOfLeavingDiseasesAlong()) * 0.01;
        }
    }

    public static final String KILLING_MACHINE = "Killing Machine";

    /**
     * Your melee attacks have a chance to make your next Icy Touch, Howling Blast or Frost Strike a critical strike.
     * Effect occurs more often than Killing Machine (Rank 2).
     */
    public static class KillingMachine extends AbstractTalent {
        public static final KillingMachine LEVEL_1 = new KillingMachine(1);
        public static final KillingMachine LEVEL_2 = new KillingMachine(2);
        public static final KillingMachine LEVEL_3 = new KillingMachine(3);
        public static final KillingMachine LEVEL_4 = new KillingMachine(4);
        public static final KillingMachine LEVEL_5 = new KillingMachine(5);

        protected KillingMachine(int rank) {
            super(KILLING_MACHINE, rank);
        }

        public int procRatePerMinute() {
            return rank();
        }
    }

    public static final String CHILL_OF_THE_GRAVE = "Chill of the Grave";

    /**
     * Your Chains of Ice, Howling Blast, Icy Touch and Obliterate
     * generate 2.5/5 additional runic power.
     */
    public static class ChillOfTheGrave extends AbstractTalent {
        public static final ChillOfTheGrave LEVEL_1 = new ChillOfTheGrave(1);
        public static final ChillOfTheGrave LEVEL_2 = new ChillOfTheGrave(2);

        // TODO: Howling blast and chain of ice should also be here
        protected static final Set<String> AFFECTED_SKILLS = Set.of(IcyTouch.ICY_TOUCH, Obliterate.OBLITERATE);

        public ChillOfTheGrave(int rank) {
            super(CHILL_OF_THE_GRAVE, rank);
        }

        public int extraRunicPowerGenerated() {
            return switch (rank()) {
                case 1 -> 2;
                case 2 -> 5;
                default -> throw new IllegalStateException("Unexpected value: " + rank());
            };
        }

        public boolean isAffected(String skillName) {
            return AFFECTED_SKILLS.contains(skillName);
        }
    }

    public static final String GLACIER_ROT = "Glacier Rot";

    /**
     * Diseased enemies take 7/13/20% more damage from your Icy Touch, Howling Blast and Frost Strike.
     */
    public static class GlacierRot extends AbstractTalent {
        public static final GlacierRot LEVEL_1 = new GlacierRot(1);
        public static final GlacierRot LEVEL_2 = new GlacierRot(2);
        public static final GlacierRot LEVEL_3 = new GlacierRot(3);

        public GlacierRot(int rank) {
            super(GLACIER_ROT, rank);
        }

        public int damageBonusPercentage() {
            return switch (rank()) {
                case 1 -> 7;
                case 2 -> 13;
                case 3 -> 20;
                default -> throw new IllegalStateException("Unexpected value: " + rank());
            };
        }
    }

    public static final String RIME = "Rime";

    /**
     * Increases the critical strike chance of your Icy Touch and Obliterate by 5/10/15%
     * and casting Obliterate has a 5/10/15% chance to reset the cooldown on Howling Blast
     * and cause your next Howling Blast to consume no runes.
     */
    public static class Rime extends AbstractTalent {
        public static final Rime LEVEL_1 = new Rime(1);
        public static final Rime LEVEL_2 = new Rime(2);
        public static final Rime LEVEL_3 = new Rime(3);

        public Rime(int rank) {
            super(RIME, rank);
        }

        public int criticalStrikePercentageBonus() {
            return 5 * rank();
        }

        public int resetChancePercentage() {
            return 5 * rank();
        }
    }

    public static final String BLOOD_OF_THE_NORTH = "Blood of the North";

    /**
     * Increases Blood Strike and Frost Strike damage by 3/6/10%.
     * In addition, whenever you hit with Blood Strike or Pestilence there is a 30/60/100% chance
     * that the Blood Rune will become a Death Rune when it activates.
     * Death Runes count as a Blood, Frost or Unholy Rune.
     */
    public static class BloodOfTheNorth extends AbstractTalent {
        public static final BloodOfTheNorth LEVEL_1 = new BloodOfTheNorth(1);
        public static final BloodOfTheNorth LEVEL_2 = new BloodOfTheNorth(2);
        public static final BloodOfTheNorth LEVEL_3 = new BloodOfTheNorth(3);

        public BloodOfTheNorth(int rank) {
            super(BLOOD_OF_THE_NORTH, rank);
        }

        public int damageBonusPercentage() {
            return switch (rank()) {
                case 1 -> 3;
                case 2 -> 6;
                case 3 -> 10;
                default -> throw new IllegalStateException("Unexpected value: " + rank());
            };
        }

        public double deathRuneChance() {
            return switch (rank()) {
                case 1 -> 0.3;
                case 2 -> 0.6;
                case 3 -> 1.0;
                default -> throw new IllegalStateException("Unexpected value: " + rank());
            };
        }
    }

    public static final String GUILE_OF_GOREFIEND = "Guile of Gorefield";

    /**
     * Increases the critical strike damage bonus of your Blood Strike, Frost Strike, Howling Blast and Obliterate abilities by 15/30/45%,
     * and increases the duration of your Icebound Fortitude by 2/4/6 secs.
     */
    public static class GuileOfGorefiend extends AbstractTalent {
        public static final GuileOfGorefiend LEVEL_1 = new GuileOfGorefiend(1);
        public static final GuileOfGorefiend LEVEL_2 = new GuileOfGorefiend(2);
        public static final GuileOfGorefiend LEVEL_3 = new GuileOfGorefiend(3);

        public GuileOfGorefiend(int rank) {
            super(GUILE_OF_GOREFIEND, rank);
        }

        public int criticalStrikeDamageBonusPercentage() {
            return 15 * rank();
        }
    }

    public static final String VICIOUS_STRIKES = "Vicious Strikes";

    /**
     * Increases the critical strike chance by 3/6% and critical strike damage bonus by 15/30%
     * of your Plague Strike and Scourge Strike.
     */
    public static class ViciousStrikes extends AbstractTalent {
        public static final ViciousStrikes LEVEL_1 = new ViciousStrikes(1);
        public static final ViciousStrikes LEVEL_2 = new ViciousStrikes(2);

        public ViciousStrikes(int rank) {
            super(VICIOUS_STRIKES, rank);
        }

        public int criticalPercentageBonus() {
            return 3 * rank();
        }

        public int criticalDamageBonusPercentage() {
            return 15 * rank();
        }
    }

    public static final String EPIDEMIC = "Epidemic";

    /**
     * Increases the duration of Blood Plague and Frost Fever by 3/6 sec.
     */
    public static class Epidemic extends AbstractTalent {
        public static final Epidemic LEVEL_1 = new Epidemic(1);
        public static final Epidemic LEVEL_2 = new Epidemic(2);

        public Epidemic(int rank) {
            super(EPIDEMIC, rank);
        }

        public int extraDiseaseTickCount() {
            return rank();
        }
    }
}
