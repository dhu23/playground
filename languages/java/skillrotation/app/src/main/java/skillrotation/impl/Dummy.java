package skillrotation.impl;

import skillrotation.impl.data.IntegerInterval;
import skillrotation.intf.Character;
import skillrotation.intf.Skill;

import java.util.Optional;

public class Dummy implements Character {
    @Override
    public long uniqueId() {
        return 0;
    }

    @Override
    public int level() {
        return -1;
    }

    @Override
    public int hp() {
        return 0;
    }

    @Override
    public int maxHp() {
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
    public Optional<Character> getTarget() {
        return Optional.empty();
    }

    @Override
    public void selectTarget(Character character) {

    }

    @Override
    public void unSelectTarget() {

    }

    @Override
    public Optional<Skill> getSkill(String name) {
        return Optional.empty();
    }

    @Override
    public void cast(String name) {

    }

    @Override
    public void triggerGlobalCoolDown(Skill skill) {

    }

    @Override
    public void clearGlobalCoolDown() {

    }

    @Override
    public boolean onGlobalCoolDown() {
        return false;
    }

    @Override
    public String name() {
        return "Dummy";
    }

    @Override
    public String description() {
        return name();
    }

    @Override
    public String shortDescription() {
        return name();
    }
}
