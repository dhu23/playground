package attendence;

// event id, role

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

public class Student {
    private final String studentId;
    private final String firstName;
    private final String lastName;
    private final int classYear;
    private Set<String> involvedEvents;

    public Student(String studentId, String firstName, String lastName, int classYear) {
        this.studentId = studentId;
        this.firstName = firstName;
        this.lastName = lastName;
        this.classYear = classYear;
        this.involvedEvents = new HashSet<>();
    }

    public String studentId() {
        return studentId;
    }

    public String name() {
        return String.format("%s, %s", lastName, firstName);
    }

    public boolean participate(Event event) {
        if (involvedEvents.contains(event.eventId())) {
            System.out.println(String.format("Student %s already registered event %s", studentId, event.eventId()));
            return false;
        } else if (event.addParticipant(this)) {
            this.involvedEvents.add(event.eventId());
            return true;
        } else {
            return false;
        }
    }

    @Override
    public String toString() {
        return String.format("%s(%s) of class %d", name(), studentId, classYear);
    }
}
