package fantasy.impl.skill;

import fantasy.intf.Character;
import fantasy.intf.ResourceCost;
import fantasy.intf.Skill;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Optional;

public abstract class AbstractSkill implements Skill {
    private static final Logger logger = LoggerFactory.getLogger(AbstractSkill.class);

    protected final String name;
    protected final int level;
    protected final ResourceCost cost;
    protected final boolean requiresTarget;
    protected final int coolDownInMillis;
    protected final int castTimeInMillis;

    public AbstractSkill(String name, int level, ResourceCost cost, boolean requiresTarget, int coolDownInMillis, int castTimeInMillis) {
        this.name = name;
        this.level = level;
        this.cost = cost;
        this.requiresTarget = requiresTarget;
        this.coolDownInMillis = coolDownInMillis;
        this.castTimeInMillis = castTimeInMillis;
    }

    @Override
    public int level() {
        return this.level;
    }

    @Override
    public ResourceCost cost() {
        return this.cost;
    }

    @Override
    public boolean requiresTarget() {
        return requiresTarget;
    }

    @Override
    public int coolDownInMillis() {
        return this.coolDownInMillis;
    }

    @Override
    public int castTimeInMillis() {
        return this.castTimeInMillis;
    }

    @Override
    public String name() {
        return this.name;
    }

    @Override
    public String description() {
        return String.format("%s(Level %d)", name(), level());
    }

    @Override
    public String shortDescription() {
        return description();
    }

    @Override
    public boolean checkCastCondition(Character caster) {
        // check target if needed
        if (requiresTarget() && caster.getTarget().isEmpty()) {
            return false;
        }

        return true;
    }
}
