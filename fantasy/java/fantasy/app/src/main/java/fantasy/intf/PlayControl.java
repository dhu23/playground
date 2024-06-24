package fantasy.intf;

import fantasy.impl.Effect;

public interface PlayControl {
    void onSelect();
    void onUnselect();
    void onGlobalCoolDownFinish();
    void onSkillCoolDownFinish(Skill skill);
    void onEffectExpiration(Character target, Effect effect);
}
