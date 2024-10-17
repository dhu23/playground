package fantasy.intf;

public interface Skill extends Named {
    public static final String AUTO_ATTACK = "Auto Attack";

    int level();
    ResourceCost cost();

    boolean requiresTarget();
    // int maxRange();

    boolean cast(Character caster);
    int coolDownInMillis();
    int castTimeInMillis();

    boolean checkCastCondition(Character caster);
}
