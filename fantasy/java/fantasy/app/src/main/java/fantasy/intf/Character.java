package fantasy.intf;

import fantasy.impl.data.IntegerInterval;
import fantasy.impl.item.Weapon;

import java.time.Duration;
import java.util.Optional;

public interface Character extends Named {
    long uniqueId();
    int level();

    int hp();
    int maxHp();

    int modifyHp(int amount);
    int sufferDamage(int amount);
    int receiveHealing(int amount);

    Duration mainHandAttackSpeed();
    Optional<Duration> offHandAttackSpeed();

    void setSkill(Skill skill);
    void setTalent(Talent talent);

    void receiveEffect(Effect effect);
    default void removeEffect(Effect effect) {
        removeEffect(effect.name());
    }
    void removeEffect(String name);
    default boolean isUnderEffect(Effect effect) {
        return isUnderEffect(effect.name());
    }
    boolean isUnderEffect(Character caster, Effect effect);
    boolean isUnderEffect(String name);
    boolean isUnderEffect(Character caster, String name);
    Optional<Effect> getEffect(String effectName);

    void onEffectExpiration(Character target, Effect effect);

    default boolean isAlive() {
        return hp() > 0;
    }
    default boolean isDead() {
        return !isAlive();
    }

    IntegerInterval emptyHandedDamage();
    IntegerInterval weaponDamage();
    int armor();
    double damageMitigation();

    int dealWeaponDamage();

    Optional<Character> getTarget();
    void selectTarget(Character character);
    void unSelectTarget();

    void setControl(PlayControl playControl);

    void turnOnAutoAttack();
    void turnOffAutoAttack();
    boolean isAutoAttacking();
    void attackWithMainHand();
    void attackWithOffHand();

    boolean cast(String name);
    boolean isCoolDownReady(String name);

    void triggerGlobalCoolDown(Skill skill);
    void clearGlobalCoolDown();
    boolean isUnderGlobalCoolDown();

    void triggerSkillCoolDown(String name);
    void clearSkillCoolDown(String name);
    boolean isSkillUnderCoolDown(String name);

    boolean equipWeapon(Weapon item);
}
