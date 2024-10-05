package fantasy.impl.resource.cost;

import fantasy.intf.ResourceCost;
import org.immutables.value.Value;

@Value.Immutable
public abstract class BaseManaCost implements ResourceCost {
    @Value.Parameter
    public abstract int baseManaPercentage();

    @Override
    @Value.Derived
    public String description() {
        return String.format("%s%% Base Mana", baseManaPercentage());
    }
}
