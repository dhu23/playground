package fantasy.intf;

public interface WorldTime {
    void stop();

    default void scheduleMainHandAutoAttack(Character caster) {
        scheduleAutoAttack(caster, caster.mainHandAttackSpeed().toMillis(), true);
    }

    default void scheduleOffHandAutoAttack(Character caster) {
        caster.offHandAttackSpeed().ifPresent(offHandSpeed -> {
            scheduleAutoAttack(caster, offHandSpeed.toMillis(), false);
        });
    }

    void scheduleAutoAttack(Character caster, long nextInMillis, boolean mainHand);
    void scheduleGlobalCoolDownEvent(Character caster, Skill skill, long coolDownInMillis);
    void scheduleSkillCoolDownEvent(Character caster, Skill skill);
    void scheduleRuneCoolDownEvent(Character caster, int runeId, long coolDownInMillis);
    void scheduleTickNotice(Effect effect);
}
