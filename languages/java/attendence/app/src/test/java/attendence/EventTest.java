package attendence;

import attendence.core.Event;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class EventTest {
    private static final String EVENT_ID = "event123";
    private static final String NAME = "Hackathon";
    private static final int SCORE = 3;
    private static final int CAPACITY = 4;

    @Test
    public void testEventObject() {
        Event event = new Event(EVENT_ID, NAME, SCORE, CAPACITY);
        Assertions.assertEquals(EVENT_ID, event.eventId());
        Assertions.assertEquals(NAME, event.name());
        Assertions.assertEquals(SCORE, event.score());
    }
}
