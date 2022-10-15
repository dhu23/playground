import java.util.ArrayList;

public class Board {
    final ArrayList<Tile> _tiles;

    public static Board createStandardBoard() {
        var tiles = new ArrayList<Tile>();
        tiles.add(new Tile(TileType.Red, false));
        tiles.add(new Tile(TileType.Purple, false));
        tiles.add(new Tile(TileType.Yellow, false));
        tiles.add(new Tile(TileType.Blue, false));
        tiles.add(new Tile(TileType.Orange, false));
        tiles.add(new Tile(TileType.Green, false));
        tiles.add(new Tile(TileType.Red, false));
        tiles.add(new Tile(TileType.Purple, false));
        tiles.add(new Tile(TileType.Cupcake, false));
        tiles.add(new Tile(TileType.Yellow, false));
        tiles.add(new Tile(TileType.Blue, false));
        tiles.add(new Tile(TileType.Orange, false));
        tiles.add(new Tile(TileType.Green, false));
        tiles.add(new Tile(TileType.Red, false));
        tiles.add(new Tile(TileType.Purple, false));
        tiles.add(new Tile(TileType.Yellow, false));
        tiles.add(new Tile(TileType.End, false));

        return new Board(tiles);
    }

    private Board(ArrayList<Tile> tiles) {
        _tiles = tiles;
    }

    public boolean atDestination(int pos) {
        return _tiles.size() - 1 == pos;
    }

    public Tile getTile(int pos) {
        return _tiles.get(pos);
    }
}

interface Card {

    public int move(int initPos, Board board);
}