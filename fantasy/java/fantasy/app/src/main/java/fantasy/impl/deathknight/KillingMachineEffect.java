package fantasy.impl.deathknight;

import fantasy.impl.AbstractEffect;
import fantasy.impl.spacetime.RealWorldSpaceTimeImpl1;
import fantasy.intf.Character;

import java.time.Duration;

public class KillingMachineEffect extends AbstractEffect {
    public static final String KILLING_MACHINE = "Killing Machine";

    public KillingMachineEffect(Character caster) {
        super(KILLING_MACHINE, 1, 1, 1, Duration.ofSeconds(8), caster, caster);

        // TODO change the API so that effects are registered with RealWorldSpaceTimeImpl1
        // therefore developers no longer need to remember to call it here
        RealWorldSpaceTimeImpl1.getInstance().scheduleTickNotice(this);
    }

    @Override
    protected void onTick_() {

    }
}
