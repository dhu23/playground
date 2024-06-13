package skillrotation.impl;

import skillrotation.intf.Character;
import skillrotation.intf.ResourceCost;

import java.util.Optional;
import java.util.random.RandomGenerator;

public abstract class AbstractTargetSkill extends AbstractSkill {
    public AbstractTargetSkill(String name, int level, ResourceCost cost) {
        super(name, level, cost);
    }

    @Override
    public void cast(Character caster) {
        Optional<Character> targetOptional = caster.getTarget();
        if (targetOptional.isPresent()) {
            Character target = targetOptional.get();
            cast_(caster);
        } else {
            // I need a target
            System.out.println("I need a target!");
        }
    }

    protected abstract void cast_(Character caster);
}
