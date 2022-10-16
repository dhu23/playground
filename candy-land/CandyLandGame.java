import java.util.List;
import java.util.ArrayList;
import java.util.Scanner;


public class CandyLandGame {
    private List<Player> _players;
    private Board _board;
    private Deck _deck;
    private boolean _started;
    private int _currentPlayerIdx;

    public CandyLandGame() {
        _players = new ArrayList<Player>();
        _board = Board.createStandardBoard();
        _deck = Deck.createStandardDeck();
        _started = false;

        System.out.println("Welcome to Candy Land.");
        System.out.println("Let's choose play colors before starting the game...");

        PlayerColor[] playerColorChoices = {PlayerColor.Blue, PlayerColor.Red, PlayerColor.Green, PlayerColor.Yellow};
        Scanner scanner = new Scanner(System.in);
        do {
            System.out.println("Type the command # and hit enter:");
            for (PlayerColor playerColor : playerColorChoices) {
                System.out.println(playerColor.ordinal() + ":" + playerColor.name());
            }
            System.out.println(playerColorChoices.length + ":Start the game");
            int colorIdx = scanner.nextInt();
            if (0 <= colorIdx && colorIdx < playerColorChoices.length) {
                this.addPlayer(playerColorChoices[colorIdx]);
            } else if (colorIdx == playerColorChoices.length) {
                this.start();
            } else {
                System.out.println("Incorrect input. Try again...");
            }

        } while (!this.isStarted());
        _currentPlayerIdx = 0;
    }

    private boolean addPlayer(PlayerColor color) {
        if (_started) {
            return false;
        }
        // just use a linear search to detect dup just to be lazy
        for (Player p : _players) {
            if (p.getColor() == color) {
                System.out.println("Someone has already took that color");
                return false;
            }
        }
        _players.add(new Player(color, _board));
        return true;
    }

    private void start() {
        if (!_players.isEmpty()) {
            _started = true;
        } else {
            System.out.println("There is no players yet!");
        }
    }

    private boolean isStarted() {
        return _started;
    }

    private Player nextPlayer() {
        int currentPlayerIdx = _currentPlayerIdx;
        _currentPlayerIdx = (_currentPlayerIdx + 1) % _players.size();
        return _players.get(currentPlayerIdx);
    }

    public void play() {
        while (true) {
            // new round
            System.out.println("New Round ------------>");
            Player player = this.nextPlayer();
            System.out.println(player.toString() + " is playing...");
            if (player.canPlay()) {
                // draw card and move
                Card card = _deck.draw();
                System.out.println("drew card: " + card.toString());
                if (player.play(card)) {
                    System.out.println("We got a WINNER!!!!!");
                    break;
                }
            } else {
                System.out.println(player.toString() + " has to pass.");
                player.pass();
            }
        }
    }

    // the main function
    public static void main(String[] args) {
        var game = new CandyLandGame();
        game.play();
    }
}
