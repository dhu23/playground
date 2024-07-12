package fantasy.impl;

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
        WorldSpaceTime.getInstance().pushTickNotice(this);
    }

    public int tickAmount() {
        return this.baseTicketAmount_;
    }

//    @Override
//    public void tick() {
//        if (this.remainingTickCount_ <= 0) {
//            return;
//        }
//
//        int tick = getTicketAmount();
//        switch (amountType_) {
//            case Damage -> this.target_.sufferDamage(tick);
//            case Healing -> this.target_.receiveHealing(tick);
//        }
//        WorldSpaceTime.getInstance().getLog().report(this.caster_, this.target_, this.amountType_, name(), tick);
//        LogUtils.log(String.format("%s remaining tick: %s", name(), this.remainingTickCount_));
//        --this.remainingTickCount_;
//
//        this.nextTickTime_ = Instant.now().plus(this.tickFrequency_);
//    }

    @Override
    protected void onTick_() {
        int tick = getTicketAmount();
        SkillUtils.SkillAmount amount = ImmutableSkillAmount.of(tick, amountType_, false, caster_, target_, name());
        target_.receive(amount);
//        switch (amountType_) {
//            case Damage -> this.target_.sufferDamage(tick);
//            case Healing -> this.target_.receiveHealing(tick);
//        }
//        WorldSpaceTime.getInstance().getLog().report(this.caster_, this.target_, this.amountType_, name(), tick, false);
    }

    protected abstract int getTicketAmount();
}
