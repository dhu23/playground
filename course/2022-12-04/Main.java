import java.util.ArrayList;

public class Main {
    public static void main(String[] args) {
        Person js = new Person("James", "Smith");
        System.out.println(js.getFirstName());

        Person dm = new Person("David", "Muir");
        System.out.println(dm.getFirstName());
        
        Person[] personArray = new Person[]{new Person("Dingze", "Yu"), js, dm};
        System.out.println(personArray.length);
        for (Person person : personArray) {
            
            // System.out.println(person.getFirstName());
            // System.out.println(person.getLastName());
            System.out.println(person.toString());
        }

        // System.out.println(personArray.toString());
        // do something else
        ArrayList<Person> personList = new ArrayList<Person>();
        personList.add(new Person("Dingze", "Yu"));
        personList.add(js);
        personList.add(dm);
        for (Person person : personList) {
            System.out.println(person.toString());
        }

        System.out.println(personList.toString());

        Student dz = new Student("Dingze", "Yu", 12345, 11, "Mater");

        System.out.println(dz.getFirstName());
        System.out.println(dz.getSchool());
        System.out.println(dz.toString());
    }
}
