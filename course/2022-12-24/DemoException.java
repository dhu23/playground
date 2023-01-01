import java.lang.ArrayIndexOutOfBoundsException;
import java.lang.IndexOutOfBoundsException;
import java.util.*;


public class DemoException {
    private List<String> array;
    private List<String> emails;

    private static class EmailFail extends Exception {
    }

    private static class DataFetchFail extends Exception {
    }

    public DemoException() {
        array = new ArrayList<>();
        array.add("a");
        array.add("b");
        array.add("c");

        emails = new ArrayList<>();
        emails.add("abc@gmail.com");
        emails.add("abcd@gmail.com");
    }

    public String fetchData(int index) throws DataFetchFail {
        // try {
        //     return array.get(index);
        // } catch (IndexOutOfBoundsException e) {
        //     throw new DataFetchFail();
        // }

        if (index >= array.size()) {
            throw new DataFetchFail();
        } else {
            return array.get(index);
        }
    }

    public void notifyEmail(int i) throws EmailFail {
        try {
            System.out.println("sending email: " + emails.get(i));
        } catch (IndexOutOfBoundsException e) {
            throw new EmailFail();
        }
    }

    public static void main(String[] args) {
        DemoException de = new DemoException();
        for (int i = 0; i < 10; ++i) {
            try {
                System.out.println(de.fetchData(i));
                de.notifyEmail(i);
            } catch (EmailFail e) {
                System.out.println("got an email error");
            } catch (DataFetchFail e) {
                System.out.println("got a data fetch error");
            }
        }
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

