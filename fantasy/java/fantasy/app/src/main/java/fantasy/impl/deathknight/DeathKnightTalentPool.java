package fantasy.impl.deathknight;

import fantasy.impl.AbstractTalent;
import fantasy.intf.Talent;

public class DeathKnightTalentPool {
    public static final String KILLING_MACHINE = "Killing Machine";

    public static class KillingMachine extends AbstractTalent {
        public static final KillingMachine LEVEL_1 = new KillingMachine(1);
        public static final KillingMachine LEVEL_2 = new KillingMachine(2);
        public static final KillingMachine LEVEL_3 = new KillingMachine(3);
        public static final KillingMachine LEVEL_4 = new KillingMachine(4);
        public static final KillingMachine LEVEL_5 = new KillingMachine(5);

        protected KillingMachine(int rank) {
            super(KILLING_MACHINE, rank);
        }

        public int procRatePerMinute() {
            return rank();
        }
    }
}
