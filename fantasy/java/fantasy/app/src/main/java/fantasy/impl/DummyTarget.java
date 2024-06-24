package fantasy.impl;

import fantasy.impl.data.IntegerInterval;
import fantasy.impl.item.Weapon;
import fantasy.intf.Skill;

import java.util.Optional;

public class DummyTarget extends AbstractCharacter {
    public DummyTarget() {
        super("DummyTarget", -1, 0);
    }

    @Override
    public long uniqueId() {
        return 0;
    }

    @Override
    public IntegerInterval emptyHandedDamage() {
        return null;
    }

    @Override
    public IntegerInterval weaponDamage() {
        return null;
    }

    @Override
    public int armor() {
        return 0;
    }

    @Override
    public double damageMitigation() {
        return 0.3;
    }

    @Override
    public int dealWeaponDamage() {
        return 0;
    }

    @Override
    public boolean hasResourceToCast(String name) {
        return false;
    }

    @Override
    public boolean equipWeapon(Weapon item) {
        return false;
    }

    @Override
    public String description() {
        return "";
    }

    @Override
    public String shortDescription() {
        return "";
    }
}
