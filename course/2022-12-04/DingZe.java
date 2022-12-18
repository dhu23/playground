import java.util.List;
import java.util.ArrayList;


// 
// -----------------------------------------------------
//     | x  | as  | 5  | a  | x | 
// -----------------------------------------------------
//

public class DingZe {
    public static void main(String[] args) {
        tryListInterface();
    }

    public static void tryListInterface() {
        String[] arr1 = new String[]{"a", "b", "c", "d", "e"};
        printArray(arr1);
        System.out.println(arr1.length);

        // does not compile
        // List<String> arr2 = new String[]{"a", "b", "c", "d", "e"};

        ArrayList<String> arr2 = new ArrayList<String>();
        arr2.add("a");
        System.out.println(arr2.size());
        arr2.add("b");
        System.out.println(arr2.size());
        arr2.add("c");
        System.out.println(arr2.size());
        arr2.add("d");
        System.out.println(arr2.size());
        arr2.add("e");
        System.out.println(arr2.size());
        
    }

    public static void printArray(String[] arr) {
        for (String each : arr) {
            System.out.println(each);
        }
    }

    public static void printList(List<String> list) {
    }
}
