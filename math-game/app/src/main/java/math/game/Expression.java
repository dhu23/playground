package math.game;

public interface Expression {
    String display();

    int getValue();

    default boolean checkAnswer(int answer) {
        return answer == getValue();
    }
}
