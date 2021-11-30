public class Student {
    private String firstName;
    private String lastName;
    private double gpa;
    private int expectedGraduationYear;
    private String declaredMajor;

    public Student(
            String firstName, String lastName,
            double gpa, int expectedGraduationYear,
            String declaredMajor
    ) {
        this.firstName = firstName;
        this.lastName = lastName;
        this.gpa = gpa;
        this.expectedGraduationYear = expectedGraduationYear;
        this.declaredMajor = declaredMajor;
    }

    public int delayGraduation(int years) {
        if (years > 0) {
            this.expectedGraduationYear += years;
        }
        return this.expectedGraduationYear;
    }
}
