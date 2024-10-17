package fantasy.impl.resource;

import fantasy.impl.resource.cost.BaseManaCost;
import fantasy.intf.Character;
import fantasy.intf.Skill;

public class ManaResource {
    private final int baseMana_;
    private int mana_;

    public ManaResource(int baseMana, int mana) {
        this.baseMana_ = baseMana;
        this.mana_ = mana;
    }

    public void setMana(int value) {
        this.mana_ = value;
    }

    public String summary() {
        return String.format("%d mana", this.mana_);
    }

    public boolean hasResourceFor(Skill skill, Character caster) {
        if (skill.cost() instanceof BaseManaCost baseManaCost) {
            int cost = (int) (baseManaCost.baseManaPercentage() * this.baseMana_ / 100.0);

            // check if there is enough resource
            if (cost > this.mana_) {
                return false;
            }
            return true;
        }
        return false;
    }

    public boolean consume(Skill skill, Character caster) {
        if (skill.cost() instanceof BaseManaCost baseManaCost) {
            int cost = (int) (baseManaCost.baseManaPercentage() * this.baseMana_ / 100.0);

            // check if there is enough resource
            if (cost > this.mana_) {
                return false;
            }

            this.mana_ -= cost;
            return true;
        }
        return false;
    }
}
