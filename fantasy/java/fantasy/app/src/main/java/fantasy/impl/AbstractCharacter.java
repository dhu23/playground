package fantasy.impl;

import com.google.common.base.Preconditions;
import fantasy.LogUtils;
import fantasy.impl.item.Weapon;
import fantasy.intf.Character;
import fantasy.intf.PlayControl;
import fantasy.intf.Skill;

import java.time.Duration;
import java.time.Instant;
import java.util.HashMap;
import java.util.Optional;

public abstract class AbstractCharacter implements Character {
    protected String name;
    protected int level;
    protected int hp;
    protected int maxHp;

    protected boolean autoAttackFlag_;

    private HashMap<String, CharacterSkill> skills_;

    protected HashMap<Effect, Instant> effects_;

    protected Optional<Character> target_;
    protected Optional<PlayControl> control_;

    protected boolean onGlobalCoolDown_;

    protected Optional<Weapon> mainHand;
    protected Optional<Weapon> offHand;

    public static class CharacterSkill {
        private final Skill skill;
        private Optional<Instant> lastCastTime;

        public CharacterSkill(Skill skill) {
            this.skill = skill;
            lastCastTime = Optional.empty();
        }

        public Skill get() {
            return this.skill;
        }

        public void setCoolDown() {
            lastCastTime = Optional.of(Instant.now());
        }

        public void clearCoolDown() {
            lastCastTime = Optional.empty();
        }

        public Instant getNextAvailableTime() {
            if (lastCastTime.isEmpty()) {
                return Instant.now();
            } else {
                return lastCastTime.get().plusMillis(this.skill.coolDownInMillis());
            }
        }

        public boolean isUnderCoolDown() {
            if (this.skill.coolDownInMillis() == 0 || lastCastTime.isEmpty()) {
                return false;
            } else {
                Instant next = lastCastTime.get().plusMillis(this.skill.coolDownInMillis());
                return Instant.now().isBefore(next);
            }
        }
    }

    public AbstractCharacter(String name, int level, int baseHp) {
        this.name = name;
        this.level = level;
        this.hp = baseHp;
        this.maxHp = baseHp;

        this.autoAttackFlag_ = false;

        skills_ = new HashMap<>();
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
    public Duration mainHandAttackSpeed() {
        if (mainHand.isEmpty()) {
            return Duration.ofSeconds(2);
        } else {
            return Duration.ofMillis(mainHand.get().attackSpeedInMillis());
        }
    }

    @Override
    public Optional<Duration> offHandAttackSpeed() {
        return offHand.map(weapon -> Duration.ofMillis(weapon.attackSpeedInMillis()));
    }

    public void setSkill(Skill skill) {
        skills_.put(skill.name(), new CharacterSkill(skill));
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
    public void onEffectExpiration(Character target, Effect effect) {
        control_.ifPresent(playControl -> playControl.onEffectExpiration(target, effect));
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
        if (autoAttackFlag_) {
            autoAttackFlag_ = false;
        }
        control_.ifPresent(PlayControl::onUnselect);
    }

    @Override
    public void setControl(PlayControl playControl) {
        control_ = Optional.of(playControl);
    }

    @Override
    public void triggerGlobalCoolDown(Skill skill) {
        if (!onGlobalCoolDown_) {
            onGlobalCoolDown_ = true;
            WorldSpaceTime.getInstance().pushGlobalCoolDownEvent(this, skill, 1500);
        }
    }

    @Override
    public void clearGlobalCoolDown() {
        if (onGlobalCoolDown_) {
            onGlobalCoolDown_ = false;
            control_.ifPresent(PlayControl::onGlobalCoolDownFinish);
        }
    }

    @Override
    public boolean isUnderGlobalCoolDown() {
        return this.onGlobalCoolDown_;
    }

    @Override
    public void triggerSkillCoolDown(String name) {
        getSkill(name).ifPresent(characterSkill -> {
            if (characterSkill.get().coolDownInMillis() > 0) {
                characterSkill.setCoolDown();
                WorldSpaceTime.getInstance().pushSkillCoolDownEvent(this, characterSkill.get());
            }
        });
    }

    @Override
    public void clearSkillCoolDown(String name) {
        getSkill(name).ifPresent(characterSkill -> {
            characterSkill.clearCoolDown();
            control_.ifPresent(playControl -> playControl.onSkillCoolDownFinish(characterSkill.get()));
        });
    }

    @Override
    public boolean isSkillUnderCoolDown(String name) {
        Optional<CharacterSkill> characterSkillOptional = getSkill(name);
        if (characterSkillOptional.isEmpty()) {
            return false;
        }
        return characterSkillOptional.get().isUnderCoolDown();
    }

    @Override
    public boolean isCoolDownReady(String name) {
        return !isUnderGlobalCoolDown() && !isSkillUnderCoolDown(name);
    }

    protected Optional<CharacterSkill> getSkill(String name) {
        Optional<CharacterSkill> skill = Optional.ofNullable(skills_.get(name));
        if (skill.isEmpty()) {
            LogUtils.log(String.format("%s has no skill: %s", name(), name));
        }
        return skill;
    }

    @Override
    public void turnOnAutoAttack() {
        if (!autoAttackFlag_ && target_.isPresent()) {
            autoAttackFlag_ = true;

            Character target = target_.get();
            int damage = (int) (dealWeaponDamage() * (1 - target.damageMitigation()));
            target.sufferDamage(damage);

            WorldSpaceTime.getInstance().pushMainHandAutoAttack(this);
            WorldSpaceTime.getInstance().pushOffHandAutoAttack(this);
        }
    }

    @Override
    public void turnOffAutoAttack() {
        autoAttackFlag_ = false;
    }

    public boolean isAutoAttacking() {
        return autoAttackFlag_;
    }

    @Override
    public boolean cast(String name) {
        // TODO clean duplicated underlying calls
        if (!canCast(name)) {
            return false;
        }
        Optional<CharacterSkill> skillOptional = getSkill(name);
        if (skillOptional.isPresent()) {
            CharacterSkill characterSkill = skillOptional.get();
            boolean casted = characterSkill.get().cast(this);
            if (casted) {
                triggerSkillCoolDown(name);
                triggerGlobalCoolDown(characterSkill.get());
            }
            return casted;
        } else {
            return false;
        }
    }

    @Override
    public String name() {
        return this.name;
    }
}
