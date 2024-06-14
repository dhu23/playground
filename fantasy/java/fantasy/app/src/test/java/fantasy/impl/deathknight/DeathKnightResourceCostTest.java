package fantasy.impl.deathknight;

import org.junit.jupiter.api.Test;

import java.util.TreeMap;

public class DeathKnightResourceCostTest {
    @Test
    public void testPrintOutRunicPowerOnly() {
        System.out.println(ImmutableDeathKnightResourceCost.of(new TreeMap<>(), 40));
    }

    @Test
    public void testPrintOutRunesOnly() {
        TreeMap<DeathKnightResourceCost.RuneType, Integer> runes = new TreeMap<>();
        runes.put(DeathKnightResourceCost.RuneType.Frost, 1);
        runes.put(DeathKnightResourceCost.RuneType.Unholy, 1);
        System.out.println(ImmutableDeathKnightResourceCost.of(runes, 0));
    }

    @Test
    public void testMixed() {
        TreeMap<DeathKnightResourceCost.RuneType, Integer> runes = new TreeMap<>();
        runes.put(DeathKnightResourceCost.RuneType.Frost, 1);
        runes.put(DeathKnightResourceCost.RuneType.Unholy, 1);
        System.out.println(ImmutableDeathKnightResourceCost.of(runes, -15));
    }
}
