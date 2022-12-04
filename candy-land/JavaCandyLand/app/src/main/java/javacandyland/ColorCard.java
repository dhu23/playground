package javacandyland;

import java.lang.IllegalArgumentException;

public class ColorCard implements Card {
    final int _count;
    final Tile.TileType _type;

    public ColorCard(int count, Tile.TileType tileType) {
        if (tileType == Tile.TileType.Blue
            || tileType == Tile.TileType.Red
            || tileType == Tile.TileType.Green
            || tileType == Tile.TileType.Yellow
            || tileType == Tile.TileType.Orange
            || tileType == Tile.TileType.Purple) {

            if (count < 1) {
                throw new IllegalArgumentException("cannot create color card with value less than 1");
            }

            _count = count;
            _type = tileType;
        } else {
            throw new IllegalArgumentException("color card needs a color tile type. Received " + tileType.toString());
        }
    }

    public int move(int initPos, Board board) {
        int pos = initPos;
        int count = _count;

        while (!board.atDestination(pos) && count > 0) {
            pos += 1;
            var tile = board.getTile(pos);
            if (tile.getType() == _type) {
                count -= 1;
            }
        }
        return pos;
    }

    public String toString() {
        return _type.name() + "(" + _count + ") Card";
    }
}
