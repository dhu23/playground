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
    public double criticalChance() {
        return 0;
    }

    @Override
    public double spellCriticalChance() {
        return 0;
    }

    @Override
    public int dealWeaponDamage() {
        return 0;
    }

    @Override
    public boolean equipWeapon(Weapon item) {
        return false;
    }

    @Override
    public boolean consumeResource(Skill skill) {
        return false;
    }

    @Override
    protected String characterClassName() {
        return "Dummy Target";
    }

    @Override
    public String shortDescription() {
        return "";
    }

    @Override
    protected void onAttackWithMainHand_() {

    }

    @Override
    protected void onAttackWithOffHand_() {

    }
}
