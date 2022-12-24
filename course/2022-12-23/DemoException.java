import java.lang.ArrayIndexOutOfBoundsException;
import java.lang.IndexOutOfBoundsException;
import java.util.*;


public class DemoException {


    public static void main(String[] args) {
        foo1();
    }

    public static void foo1() {
        System.out.println("foo1");
        try {
            String ret = foo2();
            System.out.println("foo2 called successfully: ret=" + ret);
        } catch (IndexOutOfBoundsException e) {
            System.out.println("another save: got an exception!" + e);
        }
    }

    public static String foo2() {
        List<String> array = new ArrayList<>();
        array.add("a");
        array.add("b");
        try {
            String ret = array.get(3); // type [] offset
            System.out.println("got it: " + ret);
            // do something else
            return ret;
        } catch (ArrayIndexOutOfBoundsException e) {
            System.out.println("got an exception!" + e);
            return "";
        } 
        // catch (IndexOutOfBoundsException e) {
        //     System.out.println("another save: got an exception!" + e);
        //     return "got it";
        // }
    }
}


// addStudent()
//     try {
//     createStudent()
//     addToDataBase()
//     notifyPrinciple()
//     assignCourse()
//     } catch (FailToConnect) {
//         ignore
//         or you can send someone email
//     }
// 
// 
// createStudent()
//     dostuff
//     return student
// 
// addToDataBase(student)
//     if (!connectToDatabase) {
//         return -1;
//     }
//     saveStudentToDB(student)
//     // return true/false

