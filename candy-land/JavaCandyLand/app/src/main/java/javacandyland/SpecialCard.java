package javacandyland;

public class SpecialCard implements Card {
    final Tile.TileType _type;

    public SpecialCard(Tile.TileType tileType) {
        if (tileType == Tile.TileType.Cupcake
            || tileType == Tile.TileType.Gingerbreadman
            || tileType == Tile.TileType.Icecream
            || tileType == Tile.TileType.Gummystar
            || tileType == Tile.TileType.Chocolate
            || tileType == Tile.TileType.Popsticle) {

            _type = tileType;
        } else {
            throw new IllegalArgumentException("special card needs special tile. Received " + tileType.toString());
        }
    }

    public int move(int initPos, Board board) {
        // just use a linear search to find the tile in the board
        int pos = 0;
        while (!board.atDestination(pos)) {
            var tile = board.getTile(pos);
            if (tile.getType() == _type) {
                return pos;
            }
            ++pos;
        }
        return initPos;
    }

    public String toString() {
        return _type.name() + " Card";
    }
}
