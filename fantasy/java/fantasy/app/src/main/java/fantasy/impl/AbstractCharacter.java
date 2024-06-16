package fantasy.impl;

import com.google.common.base.Preconditions;
import fantasy.impl.item.Weapon;
import fantasy.intf.Character;
import fantasy.intf.PlayControl;
import fantasy.intf.Skill;

import java.time.Duration;
import java.time.Instant;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Optional;
import java.util.Set;

public abstract class AbstractCharacter implements Character {
    protected String name;
    protected int level;
    protected int hp;
    protected int maxHp;

    protected HashMap<Effect, Instant> effects_;

    protected Optional<Character> target_;
    protected Optional<PlayControl> control_;

    protected boolean onGlobalCoolDown_;

    protected Optional<Weapon> mainHand;
    protected Optional<Weapon> offHand;

    public AbstractCharacter(String name, int level, int baseHp) {
        this.name = name;
        this.level = level;
        this.hp = baseHp;
        this.maxHp = baseHp;

        this.effects_ = new HashMap<>();

        this.target_ = Optional.empty();
        this.control_ = Optional.empty();

        this.onGlobalCoolDown_ = false;

        this.mainHand = Optional.empty();
        this.offHand = Optional.empty();
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
    public int modifyHp(int amount) {
        this.hp += amount;
        if (hp() > maxHp()) {
            this.hp = maxHp();
        } else if (hp() < 0) {
            this.hp = 0;
        }
        return hp();
    }

    @Override
    public int sufferDamage(int amount) {
        Preconditions.checkState(amount > 0);
        return modifyHp(-amount);
    }

    @Override
    public int receiveHealing(int amount) {
        Preconditions.checkState(amount > 0);
        return modifyHp(amount);
    }

    @Override
    public Instant receiveEffect(Effect effect, Duration duration) {
        Instant now = Instant.now();
        Instant expirationTime = now.plus(duration);
        effects_.put(effect, expirationTime);
        return expirationTime;
    }

    @Override
    public void removeEffect(Effect effect) {
        effects_.remove(effect);
    }

    @Override
    public boolean isUnderEffect(Effect effect) {
        return effects_.containsKey(effect);
    }

    @Override
    public Optional<Duration> remainingDuration(Effect effect) {
        Instant expiration = effects_.get(effect);
        if (expiration == null) {
            return Optional.empty();
        } else {
            Instant now = Instant.now();
            return Optional.of(Duration.between(now, expiration));
        }
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
