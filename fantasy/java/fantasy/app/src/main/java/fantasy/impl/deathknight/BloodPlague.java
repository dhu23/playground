package fantasy.impl.deathknight;

import fantasy.impl.AmountOverTimeEffect;
import fantasy.impl.SkillUtils;
import fantasy.intf.Character;

import java.time.Duration;

public class BloodPlague extends AmountOverTimeEffect {
    public static final String BLOOD_PLAGUE = "Blood Plague";

    public BloodPlague(Character caster, Character target, int totalTickCount, int baseTickAmount) {
        super(BLOOD_PLAGUE, 1, totalTickCount, Duration.ofSeconds(3),
                caster, target, baseTickAmount, SkillUtils.AmountType.Shadow);
    }

    @Override
    protected int getTicketAmount() {
        return this.baseTicketAmount_;
    }
}
