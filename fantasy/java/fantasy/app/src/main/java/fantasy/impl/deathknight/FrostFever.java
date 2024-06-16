package fantasy.impl.deathknight;

import fantasy.intf.Character;

import java.util.TreeMap;

public class FrostFever extends AbstractDeathKnightTargetSkill {
    public static final String FROST_FEVER = "Frost Fever";

    public static final FrostFever LEVEL_1 = new FrostFever(1);

    protected FrostFever(int level) {
        super(FROST_FEVER, level, getCost_());
    }

    protected static DeathKnightResourceCost getCost_() {
        return ImmutableDeathKnightResourceCost.of(new TreeMap<>(), 0);
    }

    @Override
    protected void castOnTargetByDeathKnight(DeathKnight deathKnight, Character target) {
       //
    }

    private int getBaseTickDamage_() {
        return 21;
    }
}
