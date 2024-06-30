package fantasy.impl;

import fantasy.intf.Character;
import fantasy.intf.Effect;

import java.time.Duration;
import java.time.Instant;
import java.util.Optional;

public abstract class AbstractEffect implements Effect {
    protected final String name_;
    protected final int maxStack_;
    protected final int totalTickCount_;
    protected final Duration tickFrequency_;

    protected final Character caster_;
    protected final Character target_;

    protected long id_;
    protected int stackCount_;
    protected int remainingTickCount_;
    protected Instant nextTickTime_;

    // TODO verify maxStack > 0
    public AbstractEffect(String name, int maxStack, int initialStackCount, int totalTickCount,
                          Duration tickFrequency, Character caster, Character target) {
        this.name_ = name;
        this.maxStack_ = maxStack;
        this.totalTickCount_ = totalTickCount;
        this.tickFrequency_ = tickFrequency;

        this.caster_ = caster;
        this.target_ = target;

        this.id_ = WorldSpaceTime.getInstance().getId();
        this.stackCount_ = initialStackCount;
        this.remainingTickCount_ = totalTickCount;
        this.nextTickTime_ = Instant.now().plus(tickFrequency);
    }

    @Override
    public Character caster() {
        return this.caster_;
    }

    @Override
    public Character target() {
        return this.target_;
    }

    @Override
    public long id() {
        return this.id_;
    }

    @Override
    public String name() {
        return this.name_;
    }

    @Override
    public boolean isExpired() {
        return remainingTickCount_ <= 0;
    }

    /**
     * add a stack if possible and refresh duration and ticking
     */
    @Override
    public void addStack() {
        if (this.maxStack_ > this.stackCount_) {
            ++this.stackCount_;
        }
        // refresh duration and ticking
        this.id_ = WorldSpaceTime.getInstance().getId();
        this.remainingTickCount_ = this.totalTickCount_;
        this.nextTickTime_ = Instant.now().plus(this.tickFrequency_);
    }

    @Override
    public Optional<Instant> nextTick() {
        if (this.remainingTickCount_ == 0) {
            return Optional.empty();
        } else {
            return Optional.of(this.nextTickTime_);
        }
    }
}
