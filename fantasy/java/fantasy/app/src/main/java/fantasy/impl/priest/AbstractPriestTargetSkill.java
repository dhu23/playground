package fantasy.impl.priest;

import fantasy.impl.skill.AbstractSkill;
import fantasy.intf.Character;
import fantasy.intf.ResourceCost;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Optional;

public abstract class AbstractPriestTargetSkill extends AbstractSkill {
    private static final Logger logger = LoggerFactory.getLogger(fantasy.impl.deathknight.AbstractDeathKnightTargetSkill.class);

    public AbstractPriestTargetSkill(String name, int level, ResourceCost cost, boolean requiresTarget, int coolDownInMillis, int castTimeInMillis) {
        super(name, level, cost, requiresTarget, coolDownInMillis, castTimeInMillis);
    }

    @Override
    public boolean cast(Character caster) {
        if (caster instanceof Priest priest) {
            Optional<Character> targetOptional = priest.getTarget();
            if (targetOptional.isEmpty()) {
                return false;
            }
            Character target = targetOptional.get();
            return castOnTarget_(priest, target);
        }
        return false;
    }

    protected abstract boolean castOnTarget_(Priest priest, Character target);
}

