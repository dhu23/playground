package attendence;

import java.util.HashSet;
import java.util.Set;

public class Event {
    private final String eventId;
    private final String name;
    private final int score;
    private final int capacity;
    private Set<Integer> participants;

    public Event(String eventId, String name, int score, int capacity) {
        this.eventId = eventId;
        this.name = name;
        this.score = score;
        this.capacity = capacity;
        this.participants = new HashSet<>();
    }

    public String eventId() {
        return this.eventId;
    }

    public String name() {
        return this.name;
    }

    public int score() {
        return score;
    }

    public int freeSlotCount() {
        return capacity - participants.size();
    }

    public boolean hasParticipant(Student student) {
        return this.participants.contains(student.studentId());
    }

    // client code should not call this function.
    // registration should be done through Student object
    public boolean addParticipant(Student student) {
        if (participants.contains(student.studentId())) {
            System.out.println(String.format("Event %s already has student %s", eventId, student.studentId()));
            return false;
        } else if (freeSlotCount() <= 0) {
            System.out.println(String.format("Event %s is full!", eventId));
            return false;
        } else {
            participants.add(Integer.valueOf(student.studentId()));
            return true;
        }
    }
}
