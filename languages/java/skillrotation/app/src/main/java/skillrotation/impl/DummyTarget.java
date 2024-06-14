package skillrotation.impl;

import skillrotation.impl.data.IntegerInterval;
import skillrotation.intf.Character;
import skillrotation.intf.Skill;

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
    public IntegerInterval weaponDamage() {
        return null;
    }

    @Override
    public int armor() {
        return 0;
    }

    @Override
    public int dealWeaponDamage() {
        return 0;
    }

    @Override
    public Optional<Skill> getSkill(String name) {
        return Optional.empty();
    }

    @Override
    public void cast(String name) {

    }

    @Override
    public boolean hasResourceToCast(String name) {
        return false;
    }

    @Override
    public boolean isCoolDownReady(String name) {
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
