package fantasy.impl.deathknight;

import fantasy.impl.skill.AbstractSkill;
import fantasy.intf.Character;
import fantasy.intf.ResourceCost;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Optional;

// TODO find a generic way to do this
public abstract class AbstractDeathKnightTargetSkill extends AbstractSkill {
    private static final Logger logger = LoggerFactory.getLogger(AbstractDeathKnightTargetSkill.class);

    public AbstractDeathKnightTargetSkill(String name, int level, ResourceCost cost, boolean requiresTarget, int coolDownInMillis, int castTimeInMillis) {
        super(name, level, cost, requiresTarget, coolDownInMillis, castTimeInMillis);
    }

    @Override
    public boolean cast(Character caster) {
        if (caster instanceof DeathKnight deathKnight) {
            Optional<Character> targetOptional = deathKnight.getTarget();
            if (targetOptional.isEmpty()) {
                return false;
            }
            Character target = targetOptional.get();
            return castOnTarget_(deathKnight, target);
        }
        return false;
    }

    protected abstract boolean castOnTarget_(DeathKnight deathKnight, Character target);
}
