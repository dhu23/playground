package fantasy.impl;

import fantasy.intf.Talent;

public abstract class AbstractTalent implements Talent {
    protected final int rank_;
    protected final String name_;

    public AbstractTalent(String name, int rank) {
        this.name_ = name;
        this.rank_ = rank;
    }

    @Override
    public int rank() {
        return this.rank_;
    }

    @Override
    public String name() {
        return this.name_;
    }
}
