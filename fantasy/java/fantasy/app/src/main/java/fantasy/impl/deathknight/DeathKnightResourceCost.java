package fantasy.impl.deathknight;

import org.immutables.value.Value;
import fantasy.intf.ResourceCost;

import java.util.ArrayList;
import java.util.List;
import java.util.TreeMap;

@Value.Immutable
public abstract class DeathKnightResourceCost implements ResourceCost {
    public enum RuneType {
        Blood,
        Frost,
        Unholy,
        Death
    }

    @Value.Parameter
    public abstract TreeMap<RuneType, Integer> runes();

    @Value.Parameter
    public abstract int runicPower();

    @Override
    @Value.Derived
    public String description() {
        StringBuilder builder = new StringBuilder();
        runes().forEach((runeType, count) -> {
            if (count > 0) {
                builder.append(String.format("%s %s ", count, runeType));
            }
        });
        if (runicPower() != 0) {
            builder.append(String.format("%d Runic Power", runicPower()));
        }
        return builder.toString();
    }

    @Value.Derived
    public List<RuneType> getRuneList() {
        List<RuneType> runeList = new ArrayList<>();
        runes().forEach(((runeType, count) -> {
            for (int i = 0; i < count; ++i) {
                runeList.add(runeType);
            }
        }));
        return runeList;
    }
}
