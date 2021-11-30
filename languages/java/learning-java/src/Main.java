import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        // main2(args);
        Math.pow(2, 3);
    }

    public static void main2(String[] args) {
        Scanner input = new Scanner(System.in);
        boolean isOnRepeat = true;
        while (isOnRepeat) {
            System.out.println("Playing current song");
            System.out.println("Would you like to take the song off of repeat? [yes/no]");
            String ans = input.next(); // so basically scanner blocks for user input

            if (ans.equals("yes")) {
                isOnRepeat = false;
            }
        }
        System.out.println("Playing next song");
    }

    public static void main1(String[] args) {
        System.out.println("Pick a number between 1 and 10");
        Scanner scanner = new Scanner(System.in);
        int num = scanner.nextInt();
        if (num < 5) {
            System.out.println("Enjoy the good luck a friend brings you");
        } else {
            System.out.println("Your shoe selection will make you very happy today");
        }
    }

    public static void main0(String[] args) {
        // System.out.println("Hello World from the IntelliJ IDE!");
        int studentAge = 15;
        double studentGPA = 3.45;
        boolean hasPerfectAttendance = true;
        String studentFirstName = "Kayla";
        String studentLastName = "Hammon";
        char studentFirstInitial = studentFirstName.charAt(0);
        char studentLastInitial = studentLastName.charAt(0);

        System.out.println(studentAge);
        System.out.println(studentGPA);
        System.out.println(hasPerfectAttendance);
        System.out.println(studentFirstName);
        System.out.println(studentLastName);
        System.out.println(studentFirstInitial);
        System.out.println(studentLastInitial);
        System.out.println(studentFirstName + " " + studentLastName +
                " has a GPA of " + studentGPA);
        System.out.println("What do you want to update it to?");
        Scanner input = new Scanner(System.in); // needs java.util.Scanner;
        studentGPA = input.nextDouble();
        System.out.println(studentFirstName + studentLastName +
                " now has a GPA of " + studentGPA);
    }
}
