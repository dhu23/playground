package javacandyland;

import java.util.ArrayList;

public class Board {
    final ArrayList<Tile> _tiles;

    public static Board createStandardBoard() {
        var tiles = new ArrayList<Tile>();
        tiles.add(new Tile(Tile.TileType.Red, false));
        tiles.add(new Tile(Tile.TileType.Purple, false));
        tiles.add(new Tile(Tile.TileType.Yellow, false));
        tiles.add(new Tile(Tile.TileType.Blue, false));
        tiles.add(new Tile(Tile.TileType.Orange, false));
        tiles.add(new Tile(Tile.TileType.Green, false));
        tiles.add(new Tile(Tile.TileType.Red, false));
        tiles.add(new Tile(Tile.TileType.Purple, false));
        tiles.add(new Tile(Tile.TileType.Cupcake, false));
        tiles.add(new Tile(Tile.TileType.Yellow, false));
        tiles.add(new Tile(Tile.TileType.Blue, false));
        tiles.add(new Tile(Tile.TileType.Orange, false));
        tiles.add(new Tile(Tile.TileType.Green, false));
        tiles.add(new Tile(Tile.TileType.Red, false));
        tiles.add(new Tile(Tile.TileType.Purple, false));
        tiles.add(new Tile(Tile.TileType.Yellow, false));
        tiles.add(new Tile(Tile.TileType.End, false));

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