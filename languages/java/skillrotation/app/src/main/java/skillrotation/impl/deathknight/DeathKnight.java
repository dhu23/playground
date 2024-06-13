package skillrotation.impl.deathknight;

import skillrotation.impl.WorldSpaceTime;
import skillrotation.impl.data.IntegerInterval;
import skillrotation.impl.event.Event;
import skillrotation.impl.event.ImmutableEvent;
import skillrotation.impl.event.ImmutableGlobalCoolDown;
import skillrotation.impl.event.WorldEvent;
import skillrotation.intf.Character;
import skillrotation.intf.PlayControl;
import skillrotation.intf.Skill;

import java.time.Instant;
import java.util.HashMap;
import java.util.Optional;

public class DeathKnight implements Character {
    private String name;
    private int level;
    private int hp;

    private Optional<Character> target_;
    private Optional<PlayControl> control_;
    private HashMap<String, Skill> skills_;

    private boolean onGlobalCoolDown_;

    public DeathKnight(String name, int level) {
        this.name = name;
        this.level = level;
        this.hp = 100;
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
            triggerGlobalCoolDown(skill);
            // trigger global cooldown
        });
    }

    @Override
    public void triggerGlobalCoolDown(Skill skill) {
        onGlobalCoolDown_ = true;
        Instant now = Instant.now();
        Event<WorldEvent.EventType, Object> event =
                ImmutableEvent.of(WorldEvent.EventType.GlobalCoolDown, ImmutableGlobalCoolDown.of(this, skill, now.plusMillis(1500)));
        WorldSpaceTime.getInstance().pushEvent(event);
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
