package fantasy.impl.deathknight;

import fantasy.impl.AmountOverTimeEffect;
import fantasy.impl.SkillUtils;
import fantasy.intf.Character;

import java.time.Duration;

public class FrostFever extends AmountOverTimeEffect {
    public static final String FROST_FEVER = "Frost Fever";

    public FrostFever(Character caster, Character target, int totalTickCount, int baseTickAmount) {
        super(FROST_FEVER, 1, totalTickCount, Duration.ofSeconds(3),
                caster, target, baseTickAmount, SkillUtils.AmountType.Frost);
    }

    @Override
    protected int getTicketAmount() {
        return this.baseTicketAmount_;
    }
}
