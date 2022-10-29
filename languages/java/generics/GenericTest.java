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

    public static void printBuildings1(List<Building> buildings) {
        for (var b : buildings) {
            System.out.println(b.toString());
        }
    }

    public static void printBuildings2(List<? extends Building> buildings) {
        for (var b : buildings) {
            System.out.println(b.toString());
        }
    }

    public static void addHousesToBuildings(List<? super House> buildings) {
        buildings.add(new House());
    }

    public static void testSubstitution1() {
        List<Building> buildings = new ArrayList<Building>();
        buildings.add(new Building());
        buildings.add(new Office());
        buildings.add(new House());
        GenericTest.printBuildings1(buildings);

        GenericTest.addHousesToBuildings(buildings);
        GenericTest.printBuildings2(buildings);
    }

    public static void testSubstitution2() {
        List<Office> offices = new ArrayList<Office>();
        offices.add(new Office());
        offices.add(new Office());
        offices.add(new Office());
        // GenericTest.printBuildings1(offices); // compilation error
        GenericTest.printBuildings2(offices);

        // addHousesToBuildings(offices); // compilation error
    }

    public static void main(String[] args) {
        GenericTest.printArray();
        GenericTest.arrayToList(GenericTest.intArray);
        GenericTest.testSubstitution1();
        GenericTest.testSubstitution2();
    }
}
