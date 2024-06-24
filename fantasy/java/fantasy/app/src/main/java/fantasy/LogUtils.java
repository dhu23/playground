package fantasy;

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

    public static String reportDamage(Character caster, Character target, Skill skill, int amount) {
        String line = String.format("%s's %s inflicts %d damage to %s", caster.name(), skill.name(), amount, target.name());
        log(line);
        return line;
    }

    // TODO move to somewhere else
    public enum EffectType {
        Damage,
        Healing,
        Buff,
        DeBuff,
    }

    private Path logPath;

    private static final List<String> HEADER = List.of("time", "caster", "target", "effect_type", "skill", "value");

    /**
     * file format
     * time | caster name | target | damage/heal | skill | amount
     * @param path
     */
    public LogUtils(String path) throws IOException {
        this.logPath = Path.of(path, String.format("log.%s", System.currentTimeMillis()));
        Files.createFile(this.logPath);
        Files.write(this.logPath, List.of(String.join(",", HEADER)));
    }

    public void report(Character caster, Character target, EffectType effectType, Skill skill, int amount) {
        switch (effectType) {
            case Damage -> {
                reportDamage(caster, target, skill, amount);
            }
            case Healing -> {

            }
        }
        List<String> tokens = List.of(Instant.now().toString(), caster.name(), target.name(),
                effectType.name(), skill.name(), String.valueOf(amount));
        try {
            Files.write(this.logPath, List.of(String.join(",", tokens)), StandardOpenOption.APPEND);
        } catch (IOException e) {
            log("failed to log");
        }
    }
}
