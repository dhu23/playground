package attendence.core;

// event id, role

import attendence.core.Event;

import java.util.HashMap;
import java.util.Map;

public class Student {
    private final String studentId;
    private final String firstName;
    private final String lastName;
    private final int classYear;
    private Map<String, Event> involvedEvents;

    public Student(String studentId, String firstName, String lastName, int classYear) {
        this.studentId = studentId;
        this.firstName = firstName;
        this.lastName = lastName;
        this.classYear = classYear;
        this.involvedEvents = new HashMap<>();
    }

    public String studentId() {
        return studentId;
    }

    public String name() {
        return String.format("%s, %s", lastName, firstName);
    }

    public boolean participate(Event event) {
        if (involvedEvents.containsKey(event.eventId())) {
            System.out.println(String.format("Student %s already registered event %s", studentId, event.eventId()));
            return false;
        } else if (event.addParticipant(this)) {
            this.involvedEvents.put(event.eventId(), event);
            return true;
        } else {
            return false;
        }
    }

    public int score() {
        int ret = 0;
        for (Map.Entry<String, Event> entry : involvedEvents.entrySet()) {
            ret += entry.getValue().score();
        }
        return ret;
    }

    @Override
    public String toString() {
        return String.format("%s(%s) of class %d", name(), studentId, classYear);
    }
}
