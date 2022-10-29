import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;

public class MapGetOrDefault {
    private static ArrayList<Integer> defaultVal = new ArrayList<Integer>();

    public static void main(String[] args) {
        Map<String, Integer> counter = new HashMap<>();
        Map<String, List<Integer>> pos = new HashMap<>();
        Integer c = 0;
        for (String s : new String[]{"a", "b", "c", "a"}) {
            Integer val = counter.getOrDefault(s, 0);
            val += 1;
            counter.put(s, val);
            // List<Integer> posVal = pos.getOrDefault(s, new ArrayList<Integer>());
            List<Integer> posVal = pos.getOrDefault(s, defaultVal);
            posVal.add(c);
            if (val == 1) pos.put(s, posVal);
            ++c;
        }
        System.out.println(counter.toString());
        System.out.println(pos.toString());
    }
}
