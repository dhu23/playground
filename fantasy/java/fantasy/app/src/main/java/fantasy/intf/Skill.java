package fantasy.intf;

public interface Skill extends Named {
    int level();
    ResourceCost cost();
    boolean cast(Character caster);
    int coolDownInMillis();
}
