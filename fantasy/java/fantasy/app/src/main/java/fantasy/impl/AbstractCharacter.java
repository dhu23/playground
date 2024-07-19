package fantasy.impl;

import com.google.common.base.Preconditions;
import fantasy.impl.item.Weapon;
import fantasy.impl.spacetime.RealWorldSpaceTimeImpl1;
import fantasy.intf.*;
import fantasy.intf.Character;

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
    private HashMap<String, Talent> talents_;

    // TODO support multiple effects from different casters
    protected HashMap<String, Effect> effects_;

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

        this.skills_ = new HashMap<>();
        this.talents_ = new HashMap<>();
        this.effects_ = new HashMap<>();

        this.target_ = Optional.empty();
        this.control_ = Optional.empty();

        this.onGlobalCoolDown_ = false;

        this.mainHand = Optional.empty();
        this.offHand = Optional.empty();
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
    public int receive(SkillUtils.SkillAmount amount) {
        switch (amount.type()) {
            case Physical -> {
                int damage = (int) (amount.amount() * (1.0 - damageMitigation()));
                sufferDamage(damage);
                RealWorldSpaceTimeImpl1.getInstance().getLog().report(
                        amount.caster(), amount.target(), amount.type(), amount.skillName(),
                        damage, amount.critical());
            }
            case Frost, Shadow, Fire, Arcane, Holy, Natural -> {
                sufferDamage(amount.amount());
                RealWorldSpaceTimeImpl1.getInstance().getLog().report(
                        amount.caster(), amount.target(), amount.type(), amount.skillName(),
                        amount.amount(), amount.critical());
            }
            case Healing -> {
                receiveHealing(amount.amount());
            }
        }
        return hp();
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

    public void setTalent(Talent talent) {
        talents_.put(talent.name(), talent);
    }

    @Override
    public void receiveEffect(Effect effect) {
        LogUtils.log(String.format("%s is affected by %s's %s", name(), effect.caster().name(), effect.name()));
        effects_.put(effect.name(), effect);
    }

    @Override
    public void removeEffect(String name) {
        Effect effect = effects_.remove(name);
        if (effect != null) {
            LogUtils.log(String.format("%s's %s fades from %s", effect.caster().name(), effect.name(), name()));
        }
    }

    @Override
    public boolean isUnderEffect(String name) {
        return effects_.containsKey(name);
    }

    @Override
    public boolean isUnderEffect(Character caster, Effect effect) {
        Optional<Effect> effectOptional = getEffect(name);
        if (effectOptional.isEmpty()) {
            return false;
        }
        Effect effected = effectOptional.get();
        return effected == effect && effected.caster() == caster;
    }

    @Override
    public boolean isUnderEffect(Character caster, String name) {
        Optional<Effect> effectOptional = getEffect(name);
        if (effectOptional.isEmpty()) {
            return false;
        }
        Effect effect = effectOptional.get();
        return effect.caster() == caster;
    }

    @Override
    public Optional<Effect> getEffect(String name) {
        return Optional.ofNullable(effects_.get(name));
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

    public void triggerGlobalCoolDown(Skill skill) {
        if (!onGlobalCoolDown_) {
            onGlobalCoolDown_ = true;
            RealWorldSpaceTimeImpl1.getInstance().scheduleGlobalCoolDownEvent(this, skill, 1500);
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
                RealWorldSpaceTimeImpl1.getInstance().scheduleSkillCoolDownEvent(this, characterSkill.get());
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

    protected Optional<Talent> getTalent(String name) {
        return Optional.ofNullable(talents_.get(name));
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

            attackWithMainHand();
            // TODO add full off hand support
            RealWorldSpaceTimeImpl1.getInstance().scheduleMainHandAutoAttack(this);
            RealWorldSpaceTimeImpl1.getInstance().scheduleOffHandAutoAttack(this);
        }
    }

    @Override
    public void turnOffAutoAttack() {
        autoAttackFlag_ = false;
    }

    @Override
    public boolean isAutoAttacking() {
        return autoAttackFlag_;
    }

    @Override
    public void attackWithMainHand() {
        if (target_.isPresent()) {
            onAttackWithMainHand_();

            Character target = target_.get();
            SkillUtils.SkillAmount amount = SkillUtils.calculate(this, target, "Auto Attack",
                    SkillUtils.AmountType.Physical,
                    dealWeaponDamage(), 1.0, criticalChance(), 2.0,
                    RealWorldSpaceTimeImpl1.getInstance().getRandomGenerator());

            target.receive(amount);
        }
    }

    protected abstract void onAttackWithMainHand_();

    @Override
    public void attackWithOffHand() {
        onAttackWithOffHand_();
        LogUtils.log("off hand attack is not implemented");
    }

    protected abstract void onAttackWithOffHand_();

    @Override
    public boolean cast(String name) {
        if (!isCoolDownReady(name)) {
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
