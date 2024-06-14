package fantasy.intf;

public interface Skill extends Named {
    int level();
    ResourceCost cost();
    void cast(Character caster);
}
