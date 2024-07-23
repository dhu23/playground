package fantasy.impl;

import fantasy.intf.Character;
import fantasy.intf.Skill;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import java.time.Instant;
import java.util.List;

public class LogUtils {
    private static final Logger logger = LoggerFactory.getLogger(LogUtils.class);
    private static final List<String> HEADER = List.of("time", "caster", "target", "effect_type", "skill", "value", "critical");

    private Path logPath;

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

    public void report(Instant now, Character caster, Character target,
                       SkillUtils.AmountType amountType, String skillName, int amount, boolean critical) {
        switch (amountType) {
           case Healing -> {

            }
            default -> {
                String line = String.format("%s's %s inflicts %d damage to %s",
                        caster.name(), skillName, amount, target.name());
                logger.info(line);
            }
        }
        List<String> tokens = List.of(now.toString(), caster.name(), target.name(),
                amountType.name(), skillName, String.valueOf(amount), critical ? "Y" : "N");
        try {
            Files.write(this.logPath, List.of(String.join(",", tokens)), StandardOpenOption.APPEND);
        } catch (IOException e) {
            logger.error("failed to log");
        }
    }
}
