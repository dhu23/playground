package fantasy.impl.item;

import fantasy.impl.data.IntegerInterval;
import org.immutables.value.Value;

@Value.Immutable
public abstract class Weapon {
    public enum Style {
        TwoHandedWeapon,
        MainHand,
        OneHand,
        OffHand
    }

    public enum Category {
        Axe,
        Sword,
        Mace
    }

    @Value.Parameter
    public abstract IntegerInterval weaponDamage();

    @Value.Parameter
    public abstract int attackSpeedInMillis();

    @Value.Parameter
    public abstract String name();

    @Value.Parameter
    public abstract Style style();

    @Value.Parameter
    public abstract Category category();
}
