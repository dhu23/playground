package fantasy.impl.skill;

import fantasy.intf.ResourceCost;
import fantasy.intf.Skill;

// casting a skill needs a few evaluation steps:
// 1. cast requirement, such as target, distance, etc
// 2. cool down requirement
// 3. resource requirement

// once the above are evaluated as true, the skill is cast
// resource is consumed on successful cast
public abstract class AbstractSkill implements Skill {
    protected final String name;
    protected final int level;
    protected final ResourceCost cost;
    protected final int coolDownInMillis;
    protected final int castTimeInMillis;

    public AbstractSkill(String name, int level, ResourceCost cost, int coolDownInMillis, int castTimeInMillis) {
        this.name = name;
        this.level = level;
        this.cost = cost;
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
}
