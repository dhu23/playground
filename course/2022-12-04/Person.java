public class Person {

    protected String firstName; // "James"
    protected String lastName;  // "Smith"
    // private String birthDay; // "2000-01-01
    
    // constructor
    // public Person() {
    //     firstName = "DummyFirstName";
    //     lastName = "DummyLastName";
    // }
    public Person(String first, String last) {
        firstName = first;
        lastName = last;
    }

    public String getFirstName() { return firstName; }
    public String getLastName() { return lastName; }

    // from Object
    public String toString() {
        // ie. James Smith
        // return firstName + " " + lastName;
        return String.format("%s %s", firstName, lastName);
        // return String.format("%s %s", getFristName(), lastName);
    }

    // public static void main(String[] args) {
    //     Person js = new Person();
    //     js.firstName = "James";
    //     js.lastName = "Smith";
    //     System.out.println(js.firstName);
    //     System.out.println(js.lastName);

    //     Person dm = new Person();
    //     System.out.println(dm.firstName);
    //     System.out.println(dm.lastName);

    //     js.firstName = "Jaems";
    //     System.out.println(js.firstName);
    // }
}
