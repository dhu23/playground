package skillrotation.impl;

import skillrotation.intf.Character;
import skillrotation.intf.ResourceCost;
import skillrotation.intf.Skill;

import java.util.random.RandomGenerator;

public abstract class AbstractSkill implements Skill {
    protected final String name;
    protected final int level;
    protected final ResourceCost cost;

    public AbstractSkill(String name, int level, ResourceCost cost) {
        this.name = name;
        this.level = level;
        this.cost = cost;
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
