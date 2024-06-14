package fantasy;

import java.time.Instant;

public class LogUtils {
    public static void log(String pattern, Object... objects) {
        System.out.println(String.format("%s: " + pattern, Instant.now(), objects));
    }
}
