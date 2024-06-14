package skillrotation.intf;

import skillrotation.impl.IntegerArbitraryRange;
import skillrotation.impl.data.IntegerInterval;

import java.util.Optional;
import java.util.random.RandomGenerator;

public interface Character extends Named {
    long uniqueId();
    int level();

    int hp();
    int maxHp();

    default boolean isAlive() {
        return hp() > 0;
    }
    default boolean isDead() {
        return !isAlive();
    }

    IntegerInterval weaponDamage();
    int armor();

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
}
