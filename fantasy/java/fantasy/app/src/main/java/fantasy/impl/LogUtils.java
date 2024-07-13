package fantasy.impl;

import fantasy.intf.Character;
import fantasy.intf.Skill;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import java.time.Instant;
import java.util.List;

public class LogUtils {
    public static void log(String pattern, Object... objects) {
        System.out.println(String.format("%s: " + pattern, Instant.now(), objects));
    }

    public static String reportDamage(Character caster, Character target, String skill, int amount) {
        String line = String.format("%s's %s inflicts %d damage to %s", caster.name(), skill, amount, target.name());
        log(line);
        return line;
    }

    public static String reportDamage(Character caster, Character target, Skill skill, int amount) {
        return reportDamage(caster, target, skill.name(), amount);
    }

    // TODO move to somewhere else
    public enum EffectType {
        Damage,
        Healing,
        Buff,
        DeBuff,
    }

    private Path logPath;

    private static final List<String> HEADER = List.of("time", "caster", "target", "effect_type", "skill", "value", "critical");

    /**
     * file format
     * time | caster name | target | damage/heal | skill | amount | critical
     * @param path
     */
    public LogUtils(String path) throws IOException {
        this.logPath = Path.of(path, String.format("log.%s", System.currentTimeMillis()));
        Files.createDirectories(Path.of(path));
        Files.createFile(this.logPath);
        Files.write(this.logPath, List.of(String.join(",", HEADER)));
    }

    public void report(Character caster, Character target, SkillUtils.AmountType amountType, String skillName, int amount, boolean critical) {
        switch (amountType) {
           case Healing -> {

            }
            default -> {
                reportDamage(caster, target, skillName, amount);
            }
        }
        List<String> tokens = List.of(Instant.now().toString(), caster.name(), target.name(),
                amountType.name(), skillName, String.valueOf(amount), critical ? "Y" : "N");
        try {
            Files.write(this.logPath, List.of(String.join(",", tokens)), StandardOpenOption.APPEND);
        } catch (IOException e) {
            log("failed to log");
        }
    }

    public void report(Character caster, Character target, SkillUtils.AmountType amountType, Skill skill, int amount, boolean critical) {
        report(caster, target, amountType, skill.name(), amount, critical);
    }
}
