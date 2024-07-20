package fantasy.impl.spacetime;

import fantasy.intf.Character;
import fantasy.intf.Effect;
import fantasy.intf.Skill;
import fantasy.intf.WorldTime;

public class RealTimeWithSleeping implements WorldTime {
    @Override
    public void stop() {

    }

    @Override
    public void scheduleAutoAttack(Character caster, long nextInMillis, boolean mainHand) {

    }

    @Override
    public void scheduleGlobalCoolDownEvent(Character caster, Skill skill, long coolDownInMillis) {

    }

    @Override
    public void scheduleSkillCoolDownEvent(Character caster, Skill skill) {

    }

    @Override
    public void scheduleRuneCoolDownEvent(Character caster, int runeId, long coolDownInMillis) {

    }

    @Override
    public void scheduleTickNotice(Effect effect) {

    }
}
