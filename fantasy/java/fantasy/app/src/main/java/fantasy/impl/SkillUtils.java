package fantasy.impl;

import fantasy.LogUtils;
import fantasy.intf.Character;
import fantasy.intf.Skill;
import org.immutables.value.Value;

import java.util.random.RandomGenerator;

public class SkillUtils {
    public enum AmountType {
        Physical,
        Frost,
        Shadow,
        Fire,
        Natural,
        Arcane,
        Holy,
        Healing
    }

    @Value.Immutable
    public static abstract class SkillAmount {
        @Value.Parameter
        public abstract int amount();

        @Value.Parameter
        public abstract AmountType type();

        @Value.Parameter
        public abstract boolean critical();

        @Value.Parameter
        public abstract Character caster();

        @Value.Parameter
        public abstract Character target();

        @Value.Parameter
        public abstract String skillName();
    }

    public static SkillAmount calculate(Character caster, Character target, String skillName,
                                        AmountType type, double base, double bonusMultiplier,
                                        double criticalChance, double criticalMultiplier,
                                        RandomGenerator randomGenerator) {
        double damage = base;
        damage *= bonusMultiplier;
        LogUtils.log(String.format("%s base: %s, multiplier: %s, crit chance: %s, crit multiplier: %s",
                skillName, base, bonusMultiplier, criticalChance, criticalMultiplier));
        boolean critical = RandomUtils.roll(criticalChance, randomGenerator);
        if (critical) {
            damage *= criticalMultiplier;
        }
        return ImmutableSkillAmount.of((int) damage, type, critical, caster, target, skillName);
    }

    public static SkillAmount calculate(Character caster, Character target, Skill skill,
                                        AmountType type, double base, double bonusMultiplier,
                                        double criticalChance, double criticalMultiplier,
                                        RandomGenerator randomGenerator) {
        return calculate(caster, target, skill.name(), type, base, bonusMultiplier, criticalChance, criticalMultiplier, randomGenerator);
    }
}
