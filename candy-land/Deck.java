import java.util.List;
import java.util.ArrayList;

public class Deck {
    private List<Card> _cards;
    private int _idx;


    public static Deck createStandardDeck() {
        var cards = new ArrayList<Card>();
        for (var tileType : new TileType[]{TileType.Red, TileType.Blue, TileType.Purple, TileType.Yellow, TileType.Green, TileType.Orange}) {
            for (int i = 0; i < 6; ++i) {
                cards.add(new ColorCard(1, tileType));
            }
        }
        return new Deck(cards);
    }

    private Deck(ArrayList<Card> cards) {
        if (cards.isEmpty()) {
            throw new IllegalArgumentException("empty deck");
        }
        _cards = cards;
        _idx = 0;
        this.shuffle();
    }

    public void shuffle() {
        _idx = 0;
    }

    public Card draw() {
        int currIdx = _idx;
        ++_idx;
        if (_cards.size() == _idx) {
            this.shuffle();
        }
        return _cards.get(currIdx);
    }
}
