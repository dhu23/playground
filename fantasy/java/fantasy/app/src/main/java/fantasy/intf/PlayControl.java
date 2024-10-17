package fantasy.intf;

public interface PlayControl {
    void onSelect();
    void onUnselect();
    void onGlobalCoolDownFinish();
    void onSkillCoolDownFinish(Skill skill);
    void onEffectExpiration(Character target, Effect effect);
    void onSuccessfulCast(Skill skill);
}
