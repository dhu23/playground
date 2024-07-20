package fantasy.impl;

import fantasy.impl.spacetime.WorldSpaceTime;
import fantasy.intf.Character;

import java.time.Duration;

public abstract class AmountOverTimeEffect extends AbstractEffect {
    protected int baseTicketAmount_;
    protected SkillUtils.AmountType amountType_;

    public AmountOverTimeEffect(String name, int maxStack, int totalTickCount,
                                Duration tickFrequency, Character caster, Character target, int baseTickAmount,
                                SkillUtils.AmountType amountType) {
        super(name, maxStack, 1, totalTickCount, tickFrequency, caster, target);
        this.baseTicketAmount_ = baseTickAmount;
        this.amountType_ = amountType;

        // set up damage over time
        WorldSpaceTime.getInstance().getWorldTime().scheduleTickNotice(this);
    }

    public int tickAmount() {
        return this.baseTicketAmount_;
    }

    @Override
    protected void onTick_() {
        int tick = getTicketAmount();
        SkillUtils.SkillAmount amount = ImmutableSkillAmount.of(tick, amountType_, false, caster_, target_, name());
        target_.receive(amount);
    }

    protected abstract int getTicketAmount();
}
