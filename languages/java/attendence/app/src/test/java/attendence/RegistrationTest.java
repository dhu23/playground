package attendence;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class RegistrationTest {
    private Student janeDoe;
    private Student joeBloggs;
    private Student johnSmith;

    private Event hackathon;
    private Event football;

    @BeforeEach
    public void setup() {
        janeDoe = new Student("1234", "Jane", "Doe", 2022);
        joeBloggs = new Student("5678", "Joe", "Bloggs", 2023);
        johnSmith = new Student("0000", "John", "Smith", 2024);

        hackathon = new Event("event123", "Hackathon", 10, 2);
        football = new Event("event456", "Football", 15, 20);

        Assertions.assertEquals(2, hackathon.freeSlotCount());
    }

    @Test
    public void testEventRegistration() {
        Assertions.assertTrue(janeDoe.participate(hackathon));
        Assertions.assertFalse(janeDoe.participate(hackathon));
        Assertions.assertEquals(1, hackathon.freeSlotCount());

        Assertions.assertTrue(joeBloggs.participate(hackathon));
        Assertions.assertFalse(joeBloggs.participate(hackathon));
        Assertions.assertEquals(0, hackathon.freeSlotCount());

        Assertions.assertFalse(johnSmith.participate(hackathon));
        Assertions.assertEquals(0, hackathon.freeSlotCount());
    }

    @Test
    public void testScore() {
        janeDoe.participate(hackathon);
        Assertions.assertEquals(10, janeDoe.score());

        janeDoe.participate(hackathon); // does nothing
        Assertions.assertEquals(10, janeDoe.score());

        janeDoe.participate(football);
        Assertions.assertEquals(10 + 15, janeDoe.score());
    }
}
