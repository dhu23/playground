import java.util.List;
import java.util.ArrayList;

public class GenericTest {
    public static Integer[] intArray = {1, 2, 3};

    public static void printArray() {
        for (Integer i : intArray) {
            System.out.println("int array element: " + i.toString());
        }
    }

    // Java Generics. To me it is kinda like a C++ template
    public static <T> List<T> arrayToList(T[] tArray) {
        // List is an interface in Java
        // Usse ArrayList as a concrete implementation
        List<T> tList = new ArrayList<T>();
        for (T t : tArray) {
            tList.add(t);
        }

        for (T t : tList) {
            System.out.println("T array element: " + t.toString());
        }
        return tList;
    }
}
