package skillrotation.impl.deathknight;

import skillrotation.LogUtils;
import skillrotation.impl.WorldSpaceTime;
import skillrotation.impl.data.IntegerInterval;
import skillrotation.impl.event.*;
import skillrotation.intf.Character;
import skillrotation.intf.CharacterResource;
import skillrotation.intf.PlayControl;
import skillrotation.intf.Skill;

import java.time.Instant;
import java.util.*;

public class DeathKnight implements Character {
    private String name;
    private int level;
    private int hp;
    private DeathKnightResource resource_;

    private Optional<Character> target_;
    private Optional<PlayControl> control_;
    private HashMap<String, Skill> skills_;

    private boolean onGlobalCoolDown_;

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
            LogUtils.log(String.format("rune %s is on cool down now", runeType));
        }
    }

    public static class DeathKnightResource implements CharacterResource {
        private DeathKnightRune rune1_ = new DeathKnightRune(DeathKnightResourceCost.RuneType.Blood);
        private DeathKnightRune rune2_ = new DeathKnightRune(DeathKnightResourceCost.RuneType.Blood);
        private DeathKnightRune rune3_ = new DeathKnightRune(DeathKnightResourceCost.RuneType.Frost);
        private DeathKnightRune rune4_ = new DeathKnightRune(DeathKnightResourceCost.RuneType.Frost);
        private DeathKnightRune rune5_ = new DeathKnightRune(DeathKnightResourceCost.RuneType.Unholy);
        private DeathKnightRune rune6_ = new DeathKnightRune(DeathKnightResourceCost.RuneType.Unholy);

        private List<DeathKnightRune> runes_ = List.of(rune1_, rune2_, rune3_, rune4_, rune5_, rune6_);

        private int runicPowerLevel_ = 0;

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

        public boolean hasRunes(List<DeathKnightResourceCost.RuneType> neededRuneTypes) {
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

        public boolean hasRunicPower(int neededRunicPower) {
            return neededRunicPower <= 0 || runicPowerLevel_ >= neededRunicPower;
        }

        public boolean consume(List<DeathKnightResourceCost.RuneType> runeTypes, int runicPower,
                               DeathKnight deathKnight) {
            if (hasRunes(runeTypes) && hasRunicPower(runicPower)) {
                // consume runes
                Set<Integer> availableRuneIndices = getAvailableRuneIndices();

                Instant nextAvailable = Instant.now().plusSeconds(10);
                for (DeathKnightResourceCost.RuneType needed : runeTypes) {
                    int found = selectRune_(availableRuneIndices, needed);
                    if (found >= 0) {
                        availableRuneIndices.remove(found);
                        getRune(found).setCoolDown(nextAvailable);

                        Instant now = Instant.now();
                        Event<WorldEvent.EventType, Object> event =
                                ImmutableEvent.of(WorldEvent.EventType.RuneCoolDown,
                                        ImmutableRuneCoolDown.of(deathKnight, found, now.plusMillis(10000)));
                        WorldSpaceTime.getInstance().pushEvent(event);
                    } else {
                        LogUtils.log("really bad");
                        return false;
                    }
                }
                // consume runic power
                // TODO add runic power max, right now it goes up forever
                runicPowerLevel_ -= runicPower;
                return true;
            } else {
                return false;
            }
        }

        @Override
        public String summary() {
            return "to be implemented";
        }
    }


    public DeathKnight(String name, int level) {
        this.name = name;
        this.level = level;
        this.hp = 100;
        this.resource_ = new DeathKnightResource();
        target_ = Optional.empty();
        control_ = Optional.empty();
        skills_ = new HashMap<>();
        this.onGlobalCoolDown_ = false;
    }

    public void setControl(PlayControl playControl) {
        control_ = Optional.of(playControl);
    }

    public void setSkill(Skill skill) {
        skills_.put(skill.name(), skill);
    }

    @Override
    public long uniqueId() {
        return 0;
    }

    @Override
    public int level() {
        return this.level;
    }

    @Override
    public int hp() {
        return this.hp;
    }

    @Override
    public int maxHp() {
        return 100;
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
    public Optional<Character> getTarget() {
        return target_;
    }

    @Override
    public void selectTarget(Character character) {
        System.out.println(String.format("%s: Selecting %s", Instant.now(), character.name()));
        target_ = Optional.ofNullable(character);
        control_.ifPresent(PlayControl::onSelect);
    }

    @Override
    public void unSelectTarget() {
        target_ = Optional.empty();
        control_.ifPresent(PlayControl::onUnselect);
    }

    @Override
    public Optional<Skill> getSkill(String name) {
        return Optional.ofNullable(skills_.get(name));
    }

    @Override
    public void cast(String name) {
        Optional<Skill> skillOptional = getSkill(name);
        if (skillOptional.isEmpty()) {
            System.out.println(String.format("%s: found no skill %s", Instant.now(), name));
        } else {
            System.out.println(String.format("%s: found skill %s", Instant.now(), skillOptional.get().name()));
        }
        skillOptional.ifPresent(skill -> {
            skill.cast(this);
            // triggerGlobalCoolDown(skill);
            // trigger global cooldown
        });
    }

    public boolean consumeResource(DeathKnightResourceCost resourceCost) {
        List<DeathKnightResourceCost.RuneType> neededRuneList = new ArrayList<>();
        resourceCost.runes().forEach(((runeType, count) -> {
            for (int i = 0; i < count; ++i) {
                neededRuneList.add(runeType);
            }
        }));

        // TODO maybe move the message part to DK level
        return resource_.consume(neededRuneList, resourceCost.runicPower(), this);
    }

    @Override
    public void triggerGlobalCoolDown(Skill skill) {
        onGlobalCoolDown_ = true;
        Instant now = Instant.now();
        Event<WorldEvent.EventType, Object> event =
                ImmutableEvent.of(WorldEvent.EventType.GlobalCoolDown, ImmutableGlobalCoolDown.of(this, skill, now.plusMillis(1500)));
        WorldSpaceTime.getInstance().pushEvent(event);
    }

    public void clearRuneCoolDown(int runeId) {
        resource_.getRune(runeId).clearCoolDown();
        control_.ifPresent(PlayControl::onRuneCoolDownFinish);
    }

    @Override
    public void clearGlobalCoolDown() {
        System.out.println(String.format("cleared global cd"));
        onGlobalCoolDown_ = false;
        control_.ifPresent(PlayControl::onGlobalCoolDownFinish);
    }

    @Override
    public boolean onGlobalCoolDown() {
        return this.onGlobalCoolDown_;
    }

    @Override
    public String name() {
        return this.name;
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
