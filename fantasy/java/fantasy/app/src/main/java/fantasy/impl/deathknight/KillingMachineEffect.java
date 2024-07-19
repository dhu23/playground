package fantasy.impl.deathknight;

import fantasy.impl.AbstractEffect;
import fantasy.impl.spacetime.RealTimeImpl1;
import fantasy.impl.spacetime.WorldSpaceTime;
import fantasy.intf.Character;

import java.time.Duration;

public class KillingMachineEffect extends AbstractEffect {
    public static final String KILLING_MACHINE = "Killing Machine";

    public KillingMachineEffect(Character caster) {
        super(KILLING_MACHINE, 1, 1, 1, Duration.ofSeconds(8), caster, caster);

        // TODO change the API so that effects are registered with RealTimeImpl1
        // therefore developers no longer need to remember to call it here
        WorldSpaceTime.getInstance().getWorldTime().scheduleTickNotice(this);
    }

    @Override
    protected void onTick_() {

    }
}
