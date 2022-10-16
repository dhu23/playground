import java.lang.Math;

enum PlayerColor {
    Blue,
    Red,
    Green,
    Yellow
}

public class Player {
    private final PlayerColor _color;
    private int _penalty;
    private int _pos;
    final Board _board;

    public Player(PlayerColor color, Board board) {
        _color = color;
        _penalty = 0;
        _pos = 0;
        _board = board;
    }

    public String toString() {
        String ret = _color.name() + " player at pos=" + _pos;
        if (_penalty > 0) {
            return ret + " with " + _penalty + " Penalties";
        } else {
            return ret;
        }
    }

    public PlayerColor getColor() {
        return _color;
    }

    public boolean canPlay() {
        return _penalty == 0;
    }

    public void pass() {
        _penalty = Math.max(0, _penalty - 1);
    }

    public boolean play(Card card) {
        _pos = card.move(_pos, _board);
        return _board.atDestination(_pos);
    }
}
