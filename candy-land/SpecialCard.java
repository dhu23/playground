public class SpecialCard implements Card {
    final TileType _type;

    public SpecialCard(TileType tileType) {
        if (tileType == TileType.Cupcake
            || tileType == TileType.Gingerbreadman
            || tileType == TileType.Icecream
            || tileType == TileType.Gummystar
            || tileType == TileType.Chocolate
            || tileType == TileType.Popsticle) {

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
