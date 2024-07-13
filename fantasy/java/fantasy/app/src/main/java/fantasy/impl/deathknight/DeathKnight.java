package fantasy.impl.deathknight;

import fantasy.impl.LogUtils;
import fantasy.impl.AbstractCharacter;
import fantasy.impl.RandomUtils;
import fantasy.impl.WorldSpaceTime;
import fantasy.impl.data.ImmutableIntegerInterval;
import fantasy.impl.data.IntegerInterval;
import fantasy.impl.item.Weapon;
import fantasy.intf.*;

import java.time.Instant;
import java.util.*;

public class DeathKnight extends AbstractCharacter {
    private DeathKnightResource resource_;

    public static class DeathKnightRune {
        private DeathKnightResourceCost.RuneType runeType_;
        private Optional<Instant> nextAvailableTime_;
        public boolean isDeath;

        public DeathKnightRune(DeathKnightResourceCost.RuneType initialRuneType) {
            runeType_ = initialRuneType;
            nextAvailableTime_ = Optional.empty();
            isDeath = false;
        }

        public boolean isAvailable() {
            return nextAvailableTime_.isEmpty();
        }

        public void clearCoolDown() {
            nextAvailableTime_ = Optional.empty();
        }

        public void setCoolDown(Instant availableTime) {
            nextAvailableTime_ = Optional.of(availableTime);
            // LogUtils.log(String.format("rune %s is on cool down now", runeType));
        }

        public void flipToDeath() {
            this.isDeath = true;
        }

        public void flipBack() {
            this.isDeath = false;
        }

        public DeathKnightResourceCost.RuneType getRuneType() {
            return this.isDeath
                    ? DeathKnightResourceCost.RuneType.Death
                    : this.runeType_;
        }
    }

    public static class DeathKnightResource implements CharacterResource {
        private DeathKnight.DeathKnightRune rune1_ = new DeathKnight.DeathKnightRune(DeathKnightResourceCost.RuneType.Blood);
        private DeathKnight.DeathKnightRune rune2_ = new DeathKnight.DeathKnightRune(DeathKnightResourceCost.RuneType.Blood);
        private DeathKnight.DeathKnightRune rune3_ = new DeathKnight.DeathKnightRune(DeathKnightResourceCost.RuneType.Frost);
        private DeathKnight.DeathKnightRune rune4_ = new DeathKnight.DeathKnightRune(DeathKnightResourceCost.RuneType.Frost);
        private DeathKnight.DeathKnightRune rune5_ = new DeathKnight.DeathKnightRune(DeathKnightResourceCost.RuneType.Unholy);
        private DeathKnight.DeathKnightRune rune6_ = new DeathKnight.DeathKnightRune(DeathKnightResourceCost.RuneType.Unholy);

        private List<DeathKnight.DeathKnightRune> runes_ = List.of(rune1_, rune2_, rune3_, rune4_, rune5_, rune6_);

        private int runicPowerLevel_;
        private int maxRunicPowerBonus;
        private final int originalMaxRunicPower;

        public DeathKnightResource() {
            runicPowerLevel_ = 0;
            maxRunicPowerBonus = 0;
            originalMaxRunicPower = 100;
        }

        public void setMaxRunicPowerBonus(int bonus) {
            this.maxRunicPowerBonus = bonus;
        }

        public int getMaxRunicPower() {
            return originalMaxRunicPower + maxRunicPowerBonus;
        }

        public int getRunicPower() {
            return this.runicPowerLevel_;
        }

        public boolean hasRune(DeathKnightResourceCost.RuneType runeType) {
            for (DeathKnightRune rune : runes_) {
                if (rune.getRuneType() == runeType) {
                    return true;
                }
            }
            return false;
        }

        public Set<Integer> getAvailableRuneIndices() {
            Set<Integer> ret = new HashSet<>();
            for (int i = 0; i < runes_.size(); ++i) {
                if (runes_.get(i).isAvailable()) {
                    ret.add(i);
                }
            }
            return ret;
        }

        public DeathKnight.DeathKnightRune getRune(int runeId) {
            return runes_.get(runeId);
        }

        private int selectRune_(Collection<Integer> selectedRunes, DeathKnightResourceCost.RuneType needed) {
            for (int i : selectedRunes) {
                if (getRune(i).getRuneType() == needed) {
                    return i;
                }
            }
            // prefer matching rune type over consuming a death rune
            for (int i : selectedRunes) {
                if (getRune(i).getRuneType() == DeathKnightResourceCost.RuneType.Death) {
                    return i;
                }
            }
            return -1;
        }

        protected Set<Integer> supplyRunes(List<DeathKnightResourceCost.RuneType> neededRuneTypes) {
            Set<Integer> availableRuneIndices = getAvailableRuneIndices();

            Set<Integer> toBeUsed = new HashSet<>();
            for (DeathKnightResourceCost.RuneType needed : neededRuneTypes) {
                int found = selectRune_(availableRuneIndices, needed);
                if (found >= 0) {
                    availableRuneIndices.remove(found);
                    toBeUsed.add(found);
                } else {
                    return new HashSet<>();
                }
            }
            return toBeUsed;
        }

        protected boolean hasRunicPower(int neededRunicPower) {
            return neededRunicPower <= 0 || runicPowerLevel_ >= neededRunicPower;
        }

        public boolean consume(Skill skill, DeathKnight deathKnight) {
            DeathKnightResourceCost cost = (DeathKnightResourceCost) skill.cost();
            if (cost == null) {
                return false;
            }

            Set<Integer> toBeUsed = supplyRunes(cost.getRuneList());
            boolean hasEnoughRunicPower = hasRunicPower(cost.runicPower());
            if (toBeUsed.size() != cost.getRuneList().size() || !hasEnoughRunicPower) {
                return false;
            }

            Optional<DeathKnightTalentPool.BloodOfTheNorth> bloodOfTheNorth = deathKnight.getBloodOfTheNorth();
            final boolean setRunesToDeath;
            if (bloodOfTheNorth.isPresent() && skill.name().equals(BloodStrike.BLOOD_STRIKE)) {
                setRunesToDeath = RandomUtils.roll(bloodOfTheNorth.get().deathRuneChance(),
                        WorldSpaceTime.getInstance().getRandomGenerator());
                if (setRunesToDeath) {
                    LogUtils.log("flip runes to death runes");
                }
            } else {
                setRunesToDeath = false;
            }

            // consume runes
            Instant nextAvailable = Instant.now().plusSeconds(10);
            for (int runeIndex : toBeUsed) {
                DeathKnightRune rune = getRune(runeIndex);
                rune.setCoolDown(nextAvailable);
                if (setRunesToDeath) {
                    rune.flipToDeath();
                } else {
                    rune.flipBack();
                }
                WorldSpaceTime.getInstance().pushRuneCoolDownEvent(deathKnight, runeIndex, 10000);
            }

            // consume runic power
            int runicPowerCost = cost.runicPower();
            Optional<DeathKnightTalentPool.ChillOfTheGrave> chillOfTheGrave = deathKnight.getChillOfTheGrave();
            if (chillOfTheGrave.isPresent() && chillOfTheGrave.get().isAffected(skill.name())) {
                runicPowerCost -= chillOfTheGrave.get().extraRunicPowerGenerated();
            }

            int maxLevel = getMaxRunicPower();
            runicPowerLevel_ -= runicPowerCost;
            if (runicPowerLevel_ < 0) {
                LogUtils.log("really really bad");
            } else if (runicPowerLevel_ > maxLevel){
                runicPowerLevel_ = maxLevel;
            }

            return true;
        }

        private String getRuneSummary() {
            List<String> runeSummarySegments = new ArrayList<>();

            for (int i = 0; i < runes_.size(); ++i) {
                DeathKnight.DeathKnightRune rune = getRune(i);
                if (rune.isAvailable()) {
                    runeSummarySegments.add(String.format("%s(%d)", rune.getRuneType(), i));
                }
            }

            if (runeSummarySegments.isEmpty()) {
                return "No Runes Available";
            } else {
                return String.join(", ", runeSummarySegments);
            }
        }

        @Override
        public String summary() {
            return String.format("%s, %d runic power", getRuneSummary(), runicPowerLevel_);
        }
    }

    public DeathKnight(String name, int level) {
        super(name, level, 100);

        this.resource_ = new DeathKnightResource();
    }

    @Override
    public IntegerInterval emptyHandedDamage() {
        return ImmutableIntegerInterval.of(3, 5);
    }

    @Override
    public IntegerInterval weaponDamage() {
        return this.mainHand.map(Weapon::weaponDamage).orElse(emptyHandedDamage());
    }

    @Override
    public int armor() {
        return 5000;
    }

    @Override
    public double damageMitigation() {
        return 0;
    }

    @Override
    public double criticalChance() {
        return 0.03;
    }

    @Override
    public int dealWeaponDamage() {
        return weaponDamage().sample(WorldSpaceTime.getInstance().getRandomGenerator());
    }

    @Override
    public boolean equipWeapon(Weapon item) {
        switch (item.style()) {
            case TwoHandedWeapon -> {
                switch (item.category()) {
                    case Axe, Sword -> {
                        this.mainHand = Optional.of(item);
                        return true;
                    }
                }
            }
        }
        return false;
    }

    public DeathKnightResource getDeathKnightResource() {
        return this.resource_;
    }

    public boolean consumeResource(Skill skill) {
        LogUtils.log(String.format("%s has %s", name(), resource_.summary()));
        boolean ok = resource_.consume(skill, this);
        if (ok) {
            LogUtils.log(String.format("%s has %s", name(), resource_.summary()));
        }
        return ok;
    }

    public void clearRuneCoolDown(int runeId) {
        resource_.getRune(runeId).clearCoolDown();
        control_.ifPresent(control -> ((DeathKnightPlayControl) control).onRuneCoolDownFinish());
    }

    @Override
    protected void onAttackWithMainHand_() {
        Optional<DeathKnightTalentPool.KillingMachine> talent = getKillingMachine();
        if (talent.isEmpty()) {
            return;
        }
        double attacksIn1Min = 60000.0 / mainHandAttackSpeed().toMillis();
        if (offHandAttackSpeed().isPresent()) {
            attacksIn1Min += 60000.0 / offHandAttackSpeed().get().toMillis();
        }

        double chance = talent.get().procRatePerMinute() / attacksIn1Min;
        LogUtils.log(String.format("%s's KM proc chance is %s", name(), chance));

        if (RandomUtils.roll(chance, WorldSpaceTime.getInstance().getRandomGenerator())) {
            LogUtils.log(String.format("%s's %s is triggered", name(), talent.get().name()));
            receiveEffect(new KillingMachineEffect(this));
        }
    }

    @Override
    protected void onAttackWithOffHand_() {

    }

    public void setSubversion(DeathKnightTalentPool.Subversion talent) {
        setTalent(talent);
    }

    public Optional<DeathKnightTalentPool.Subversion> getSubversion() {
        return getTalent(DeathKnightTalentPool.SUBVERSION)
                .map(talent -> (DeathKnightTalentPool.Subversion) talent);
    }

    public void setTwoHandedWeaponSpecialization(DeathKnightTalentPool.TwoHandedWeaponSpecialization talent) {
        setTalent(talent);
    }

    public Optional<DeathKnightTalentPool.TwoHandedWeaponSpecialization> getTwoHandedWeaponSpecialization() {
        return getTalent(DeathKnightTalentPool.TWO_HANDED_WEAPON_SPECIALIZATION)
                .map(talent -> (DeathKnightTalentPool.TwoHandedWeaponSpecialization) talent);
    }

    public void setRunicPowerMastery(DeathKnightTalentPool.RunicPowerMastery talent) {
        setTalent(talent);
        resource_.setMaxRunicPowerBonus(talent.runicPowerBonus());
    }

    public Optional<DeathKnightTalentPool.RunicPowerMastery> getRunicPowerMastery() {
        return getTalent(DeathKnightTalentPool.RUNIC_POWER_MASTERY)
                .map(talent -> (DeathKnightTalentPool.RunicPowerMastery) talent);
    }

    public void setBlackIce(DeathKnightTalentPool.BlackIce talent) {
        setTalent(talent);
    }

    public Optional<DeathKnightTalentPool.BlackIce> getBlackIce() {
        return getTalent(DeathKnightTalentPool.BLACK_ICE)
                .map(talent -> (DeathKnightTalentPool.BlackIce) talent);
    }

    public void setAnnihilation(DeathKnightTalentPool.Annihilation talent) {
        setTalent(talent);
    }

    public Optional<DeathKnightTalentPool.Annihilation> getAnnihilation() {
        return getTalent(DeathKnightTalentPool.ANNIHILATION)
                .map(talent -> (DeathKnightTalentPool.Annihilation) talent);
    }

    public void setKillingMachine(DeathKnightTalentPool.KillingMachine talent) {
        setTalent(talent);
    }

    public Optional<DeathKnightTalentPool.KillingMachine> getKillingMachine() {
        return getTalent(DeathKnightTalentPool.KILLING_MACHINE)
                .map(talent -> (DeathKnightTalentPool.KillingMachine) talent);
    }

    public void setChillOfTheGrave(DeathKnightTalentPool.ChillOfTheGrave talent) {
        setTalent(talent);
    }

    public Optional<DeathKnightTalentPool.ChillOfTheGrave> getChillOfTheGrave() {
        return getTalent(DeathKnightTalentPool.CHILL_OF_THE_GRAVE)
                .map(talent -> (DeathKnightTalentPool.ChillOfTheGrave) talent);
    }

    public void setGlacierRot(DeathKnightTalentPool.GlacierRot talent) {
        setTalent(talent);
    }

    public Optional<DeathKnightTalentPool.GlacierRot> getGlacierRot() {
        return getTalent(DeathKnightTalentPool.GLACIER_ROT)
                .map(talent -> (DeathKnightTalentPool.GlacierRot) talent);
    }

    public void setRime(DeathKnightTalentPool.Rime talent) {
        setTalent(talent);
    }

    public Optional<DeathKnightTalentPool.Rime> getRime() {
        return getTalent(DeathKnightTalentPool.RIME)
                .map(talent -> (DeathKnightTalentPool.Rime) talent);
    }

    public void setBloodOfTheNorth(DeathKnightTalentPool.BloodOfTheNorth talent) {
        setTalent(talent);
    }

    public Optional<DeathKnightTalentPool.BloodOfTheNorth> getBloodOfTheNorth() {
        return getTalent(DeathKnightTalentPool.BLOOD_OF_THE_NORTH)
                .map(talent -> (DeathKnightTalentPool.BloodOfTheNorth) talent);
    }

    public void setGuileOfGorefield(DeathKnightTalentPool.GuileOfGorefiend talent) {
        setTalent(talent);
    }

    public Optional<DeathKnightTalentPool.GuileOfGorefiend> getGuileOfGorefiend() {
        return getTalent(DeathKnightTalentPool.GUILE_OF_GOREFIEND)
                .map(talent -> (DeathKnightTalentPool.GuileOfGorefiend) talent);
    }

    public void setViciousStrikes(DeathKnightTalentPool.ViciousStrikes talent) {
        setTalent(talent);
    }

    public Optional<DeathKnightTalentPool.ViciousStrikes> getViciousStrikes() {
        return getTalent(DeathKnightTalentPool.VICIOUS_STRIKES)
                .map(talent -> (DeathKnightTalentPool.ViciousStrikes) talent);
    }

    public void setEpidemic(DeathKnightTalentPool.Epidemic talent) {
        setTalent(talent);
    }

    public Optional<DeathKnightTalentPool.Epidemic> getEpidemic() {
        return getTalent(DeathKnightTalentPool.EPIDEMIC)
                .map(talent -> (DeathKnightTalentPool.Epidemic) talent);
    }

    @Override
    public String description() {
        return String.format("%s (Level %s Death Knight)", name(), level());
    }

    @Override
    public String shortDescription() {
        return description();
    }
}
