package javacandyland;

public class Tile {
    final TileType _type;
    final boolean _licorice;

    public enum TileType {
        Start,
        End,
        Blue,
        Red,
        Green,
        Yellow,
        Orange,
        Purple,
        Cupcake,
        Icecream,
        Gummystar,
        Gingerbreadman,
        Popsticle,
        Chocolate
    }

    public Tile(TileType tileType, boolean licorice) {
        _type = tileType;
        _licorice = licorice;
    }

    public TileType getType() {
        return _type;
    }

    public String toString() {
        if (_licorice) {
            return "Tile[" + _type.name() + "with Licorice]";
        } else {
            return "Tile[" + _type.name() + "]";
        }
    }
}
