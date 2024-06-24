package fantasy.intf;

public interface Skill extends Named {
    public static final String AUTO_ATTACK = "Auto Attack";

    int level();
    ResourceCost cost();
    boolean cast(Character caster);
    int coolDownInMillis();
}
