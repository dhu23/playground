package fantasy.impl.deathknight;

import fantasy.LogUtils;
import fantasy.impl.AmountOverTimeEffect;
import fantasy.intf.Character;

import java.time.Duration;

public class BloodPlague extends AmountOverTimeEffect {
    public static final String BLOOD_PLAGUE = "Blood Plague";

    public BloodPlague(Character caster, Character target, int baseTickAmount) {
        super(BLOOD_PLAGUE, 1, 5, Duration.ofSeconds(3),
                caster, target, baseTickAmount, LogUtils.EffectType.Damage);
    }

    @Override
    protected int getTicketAmount() {
        return this.baseTicketAmount_;
    }
}
