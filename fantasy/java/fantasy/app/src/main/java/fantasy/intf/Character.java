package fantasy.intf;

import fantasy.impl.Effect;
import fantasy.impl.data.IntegerInterval;
import fantasy.impl.item.Weapon;

import java.time.Duration;
import java.time.Instant;
import java.util.Optional;

public interface Character extends Named {
    long uniqueId();
    int level();

    int hp();
    int maxHp();

    int modifyHp(int amount);
    int sufferDamage(int amount);
    int receiveHealing(int amount);

    Instant receiveEffect(Effect effect, Duration duration);
    void removeEffect(Effect effect);
    boolean isUnderEffect(Effect effect);
    Optional<Duration> remainingDuration(Effect effect);

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

    Optional<Skill> getSkill(String name);
    void cast(String name);
    boolean hasResourceToCast(String name);
    boolean isCoolDownReady(String name);
    default boolean canCast(String name) {
        return hasResourceToCast(name) && isCoolDownReady(name);
    }

    void triggerGlobalCoolDown(Skill skill);
    void clearGlobalCoolDown();
    boolean onGlobalCoolDown();

    boolean equipWeapon(Weapon item);
}
