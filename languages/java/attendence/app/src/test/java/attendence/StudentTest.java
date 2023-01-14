package attendence;

import attendence.core.Student;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class StudentTest {
    private static final String STUDENT_ID = "1234";
    private static final String FIRST_NAME = "Jane";
    private static final String LAST_NAME = "Doe";
    private static final int CLASS_YEAR = 2023;

    @Test
    public void testStudentObject() {
        Student student = new Student(STUDENT_ID, FIRST_NAME, LAST_NAME, CLASS_YEAR);
        Assertions.assertEquals(STUDENT_ID, student.studentId());
        Assertions.assertEquals("Doe, Jane", student.name());
    }
}
