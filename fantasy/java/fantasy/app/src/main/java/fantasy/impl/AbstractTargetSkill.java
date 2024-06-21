package fantasy.impl;

import fantasy.intf.Character;
import fantasy.intf.ResourceCost;

import java.util.Optional;

public abstract class AbstractTargetSkill extends AbstractSkill {
    public AbstractTargetSkill(String name, int level, ResourceCost cost, int coolDownInMillis) {
        super(name, level, cost, coolDownInMillis);
    }

    @Override
    public boolean cast(Character caster) {
        Optional<Character> targetOptional = caster.getTarget();
        if (targetOptional.isEmpty()) {
            // I need a target
            System.out.println("I need a target!");
            return false;
        }

        return castOnTarget_(caster, targetOptional.get());
    }

    protected abstract boolean castOnTarget_(Character caster, Character target);
}
