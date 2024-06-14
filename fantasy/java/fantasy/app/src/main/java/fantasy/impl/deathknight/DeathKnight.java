package fantasy.impl.deathknight;

import fantasy.LogUtils;
import fantasy.impl.AbstractCharacter;
import fantasy.impl.WorldSpaceTime;
import fantasy.impl.data.IntegerInterval;
import fantasy.intf.*;

import java.time.Instant;
import java.util.*;

public class DeathKnight extends AbstractCharacter {
    private DeathKnightResource resource_;

    private HashMap<String, Skill> skills_;

    public static class DeathKnightRune {
        public DeathKnightResourceCost.RuneType runeType;
        public Optional<Instant> nextAvailableTime;

        public DeathKnightRune(DeathKnightResourceCost.RuneType initialRuneType) {
            runeType = initialRuneType;
            nextAvailableTime = Optional.empty();
        }

        public boolean isAvailable() {
            return nextAvailableTime.isEmpty();
        }

        public void clearCoolDown() {
            nextAvailableTime = Optional.empty();
        }

        public void setCoolDown(Instant availableTime) {
            nextAvailableTime = Optional.of(availableTime);
            // LogUtils.log(String.format("rune %s is on cool down now", runeType));
        }
    }

    public class DeathKnightResource implements CharacterResource {
        private DeathKnightRune rune1_ = new DeathKnightRune(DeathKnightResourceCost.RuneType.Blood);
        private DeathKnightRune rune2_ = new DeathKnightRune(DeathKnightResourceCost.RuneType.Blood);
        private DeathKnightRune rune3_ = new DeathKnightRune(DeathKnightResourceCost.RuneType.Frost);
        private DeathKnightRune rune4_ = new DeathKnightRune(DeathKnightResourceCost.RuneType.Frost);
        private DeathKnightRune rune5_ = new DeathKnightRune(DeathKnightResourceCost.RuneType.Unholy);
        private DeathKnightRune rune6_ = new DeathKnightRune(DeathKnightResourceCost.RuneType.Unholy);

        private List<DeathKnightRune> runes_ = List.of(rune1_, rune2_, rune3_, rune4_, rune5_, rune6_);

        private int runicPowerLevel_ = 0;
        private int maxRunicPower = 100;

        public Set<Integer> getAvailableRuneIndices() {
            Set<Integer> ret = new HashSet<>();
            for (int i = 0; i < runes_.size(); ++i) {
                if (runes_.get(i).isAvailable()) {
                    ret.add(i);
                }
            }
            return ret;
        }

        public DeathKnightRune getRune(int runeId) {
            return runes_.get(runeId);
        }

        private int selectRune_(Collection<Integer> selectedRunes, DeathKnightResourceCost.RuneType needed) {
            for (int i : selectedRunes) {
                if (getRune(i).runeType == needed) {
                    return i;
                }
            }
            return -1;
        }

        protected boolean hasRunes(List<DeathKnightResourceCost.RuneType> neededRuneTypes) {
            Set<Integer> availableRuneIndices = getAvailableRuneIndices();

            for (DeathKnightResourceCost.RuneType needed : neededRuneTypes) {
                int found = selectRune_(availableRuneIndices, needed);
                if (found >= 0) {
                    availableRuneIndices.remove(found);
                } else {
                    return false;
                }
            }
            return true;
        }

        protected boolean hasRunicPower(int neededRunicPower) {
            return neededRunicPower <= 0 || runicPowerLevel_ >= neededRunicPower;
        }

        public boolean hasResource(DeathKnightResourceCost resourceCost) {
            return hasRunes(resourceCost.getRuneList()) && hasRunicPower(resourceCost.runicPower());
        }

        public boolean consume(DeathKnightResourceCost resourceCost,
                               DeathKnight deathKnight) {
            if (hasRunes(resourceCost.getRuneList()) && hasRunicPower(resourceCost.runicPower())) {
                // consume runes
                Set<Integer> availableRuneIndices = getAvailableRuneIndices();

                Instant nextAvailable = Instant.now().plusSeconds(10);
                for (DeathKnightResourceCost.RuneType needed : resourceCost.getRuneList()) {
                    int found = selectRune_(availableRuneIndices, needed);
                    if (found >= 0) {
                        availableRuneIndices.remove(found);
                        getRune(found).setCoolDown(nextAvailable);
                        WorldSpaceTime.getInstance().pushRuneCoolDownEvent(deathKnight, found, 10000);
                    } else {
                        LogUtils.log("really bad");
                        return false;
                    }
                }
                // consume runic power
                runicPowerLevel_ -= resourceCost.runicPower();
                if (runicPowerLevel_ < 0) {
                    LogUtils.log("really really bad");
                } else if (runicPowerLevel_ > maxRunicPower){
                    runicPowerLevel_ = maxRunicPower;
                }
                return true;
            } else {
                return false;
            }
        }

        private String getRuneSummary() {
            List<String> runeSummarySegments = new ArrayList<>();

            for (int i = 0; i < runes_.size(); ++i) {
                DeathKnightRune rune = getRune(i);
                if (rune.isAvailable()) {
                    runeSummarySegments.add(String.format("%s(%d)", rune.runeType, i));
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
        skills_ = new HashMap<>();
    }

    public void setSkill(Skill skill) {
        skills_.put(skill.name(), skill);
    }

    @Override
    public long uniqueId() {
        return 0;
    }

    @Override
    public IntegerInterval weaponDamage() {
        return null;
    }

    @Override
    public int armor() {
        return 5000;
    }

    @Override
    public int dealWeaponDamage() {
        return 0;
    }

    @Override
    public Optional<Skill> getSkill(String name) {
        Optional<Skill> skill = Optional.ofNullable(skills_.get(name));
        if (skill.isEmpty()) {
            LogUtils.log(String.format("%s has no skill: %s", name(), name));
        }
        return skill;
    }

    @Override
    public void cast(String name) {
        getSkill(name).ifPresent(skill -> skill.cast(this));
    }

    @Override
    public boolean hasResourceToCast(String name) {
        Optional<Skill> skillOptional = getSkill(name);
        if (skillOptional.isEmpty()) {
            return false;
        }
        DeathKnightResourceCost cost = (DeathKnightResourceCost) skillOptional.get().cost();
        return hasResource(cost);
    }

    @Override
    public boolean isCoolDownReady(String name) {
        // TODO need to add skill specific cooldown check
        return !onGlobalCoolDown_;
    }

    protected boolean hasResource(DeathKnightResourceCost resourceCost) {
        return resource_.hasResource(resourceCost);
    }

    public boolean consumeResource(DeathKnightResourceCost resourceCost) {
        boolean ok = resource_.consume(resourceCost, this);
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
    public String description() {
        return String.format("%s (Level %s Death Knight)", name(), level());
    }

    @Override
    public String shortDescription() {
        return description();
    }
}
