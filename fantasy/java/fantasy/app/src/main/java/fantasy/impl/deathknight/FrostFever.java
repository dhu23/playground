package fantasy.impl.deathknight;

import fantasy.LogUtils;
import fantasy.impl.AmountOverTimeEffect;
import fantasy.intf.Character;

import java.time.Duration;

public class FrostFever extends AmountOverTimeEffect {
    public static final String FROST_FEVER = "Frost Fever";

    public FrostFever(Character caster, Character target, int baseTickAmount) {
        super(FROST_FEVER, 1, 1, Duration.ofSeconds(3),
                caster, target, baseTickAmount, LogUtils.EffectType.Damage);
    }

    @Override
    protected int getTicketAmount() {
        return this.baseTicketAmount_;
    }
}
