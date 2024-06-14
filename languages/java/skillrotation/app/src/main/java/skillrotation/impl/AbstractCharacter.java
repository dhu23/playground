package skillrotation.impl;

import skillrotation.intf.Character;
import skillrotation.intf.PlayControl;
import skillrotation.intf.Skill;

import java.time.Instant;
import java.util.Optional;

public abstract class AbstractCharacter implements Character {
    protected String name;
    protected int level;
    protected int hp;
    protected int maxHp;

    protected Optional<Character> target_;
    protected Optional<PlayControl> control_;

    protected boolean onGlobalCoolDown_;

    public AbstractCharacter(String name, int level, int baseHp) {
        this.name = name;
        this.level = level;
        this.hp = baseHp;
        this.maxHp = baseHp;

        this.target_ = Optional.empty();
        this.control_ = Optional.empty();

        this.onGlobalCoolDown_ = false;
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
        return this.maxHp;
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
    public void setControl(PlayControl playControl) {
        control_ = Optional.of(playControl);
    }

    @Override
    public void triggerGlobalCoolDown(Skill skill) {
        onGlobalCoolDown_ = true;
        WorldSpaceTime.getInstance().pushGlobalCoolDownEvent(this, skill, 1500);
    }

    @Override
    public void clearGlobalCoolDown() {
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
}
