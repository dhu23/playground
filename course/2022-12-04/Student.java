

// Student is a sub-class
// Person is a super-class
public class Student extends Person {
    
    private int studentId;
    private int grade; // 7/8/9/10 etc
    private String school;

    public String getSchool() {
        return school;
    }

    public Student(String first, String last, int id, int grade, String school) {
        super(first, last);
        studentId = id;
        this.grade = grade;
        this.school = school;
    }

    public String toString() {
        return String.format("%s %s(%d)@school=%s, grade=%s", this.firstName, this.lastName, this.studentId, this.school, this.grade);
    }
}
