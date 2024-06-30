package fantasy.impl;

import fantasy.LogUtils;
import fantasy.intf.Character;

import java.time.Duration;
import java.time.Instant;

public abstract class AmountOverTimeEffect extends AbstractEffect {
    protected int baseTicketAmount_;
    protected LogUtils.EffectType effectType_;

    public AmountOverTimeEffect(String name, int maxStack, int totalTickCount,
                                Duration tickFrequency, Character caster, Character target, int baseTickAmount,
                                LogUtils.EffectType effectType) {
        super(name, maxStack, 1, totalTickCount, tickFrequency, caster, target);
        this.baseTicketAmount_ = baseTickAmount;
        this.effectType_ = effectType;

        // set up damage over time
        WorldSpaceTime.getInstance().pushTickNotice(this);
    }

    public int tickAmount() {
        return this.baseTicketAmount_;
    }

    @Override
    public void tick() {
        if (this.remainingTickCount_ <= 0) {
            return;
        }

        int tick = getTicketAmount();
        switch (effectType_) {
            case Damage -> this.target_.sufferDamage(tick);
            case Healing -> this.target_.receiveHealing(tick);
        }
        WorldSpaceTime.getInstance().getLog().report(this.caster_, this.target_, this.effectType_, name(), tick);
        LogUtils.log(String.format("%s remaining tick: %s", name(), this.remainingTickCount_));
        --this.remainingTickCount_;

        this.nextTickTime_ = Instant.now().plus(this.tickFrequency_);
    }

    protected abstract int getTicketAmount();
}
