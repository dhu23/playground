package math.game;

import javax.swing.*;
import javax.swing.text.DefaultCaret;
import java.awt.*;
import java.awt.event.*;
import java.util.List;
import java.util.Random;

public class MathGame extends JFrame implements ActionListener {

    private static final String DISPLAY_ANSWER_COMMAND = "DisplayAnswer";
    private static final String PLAY_AGAIN_COMMAND = "PlayAgain";

    private final Random rand = new Random();

    // entire UI
    private final JPanel mainPanel = new JPanel();
    private final CardLayout mainCardLayout = new CardLayout();
    private int cardLayoutIndex = 1;
    private final List<String> cardLayoutNames = List.of("Welcome", "Game");

    // welcome UI
    private final JPanel welcomePanel = new JPanel();
    private final JLabel welcome;

    // play UI
    private final JPanel playPanel = new JPanel();
    private final JLabel question;
    private final JButton confirm;
    private final JLabel answer;
    private final JTextArea journal;
    private final JButton playAgain;

    // game mechanism
    private Expression mathQuestion;
    private boolean questionLogged;

    public MathGame() {
        setSize(560, 450);
        setLocation(768, 256);
        setTitle("Math Game!");

        this.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                System.exit(0);
            }
        });

        mainPanel.setLayout(mainCardLayout);

        // welcome panel setup
        welcomePanel.setLayout(new BorderLayout());
        // welcome top corner
        JPanel welcomeTopPanel = new JPanel();
        welcomeTopPanel.setLayout(new FlowLayout());

        welcome = new JLabel();
        welcome.setFont(new Font(null, Font.PLAIN, 40));
        welcome.setText("Welcome");

        welcomeTopPanel.add(welcome);
        welcomePanel.add(welcomeTopPanel, BorderLayout.NORTH);

        // welcome center corner
        JPanel welcomeCenterPanel = new JPanel();
        welcomeCenterPanel.setLayout(new FlowLayout());

        // play panel setup
        playPanel.setLayout(new BorderLayout());

        // play panel bottom corner
        JPanel playBottomPanel = new JPanel();
        playBottomPanel.setLayout(new GridLayout(1, 3));

        confirm = new JButton("Display Answer");
        confirm.setActionCommand(DISPLAY_ANSWER_COMMAND);
        confirm.addActionListener(this);

        playAgain = new JButton("Play Again");
        playAgain.setActionCommand(PLAY_AGAIN_COMMAND);
        playAgain.addActionListener(this);

        playBottomPanel.add(confirm);
        playBottomPanel.add(playAgain);

        playPanel.add(playBottomPanel, BorderLayout.SOUTH);

        // play panel top corner
        JPanel playTopPanel = new JPanel();
        playTopPanel.setLayout(new FlowLayout());

        question = new JLabel();
        question.setFont(new Font(null, Font.PLAIN, 48));
        playTopPanel.add(question);

        playPanel.add(playTopPanel, BorderLayout.NORTH);

        // play panel center corner
        JPanel playCenterPanel = new JPanel();
        playCenterPanel.setLayout(new FlowLayout());

        answer = new JLabel();
        answer.setFont(new Font(null, Font.PLAIN, 20));
        playCenterPanel.add(answer);

        journal = new JTextArea("Journal:\n", 16, 40);
        journal.setEditable(false);
        // always keep the focus at the last line of TextArea
        DefaultCaret journalCaret = (DefaultCaret) journal.getCaret();
        journalCaret.setUpdatePolicy(DefaultCaret.ALWAYS_UPDATE);
        JScrollPane journalScrollPane = new JScrollPane(journal);
        playCenterPanel.add(journalScrollPane);

        playPanel.add(playCenterPanel, BorderLayout.CENTER);

        mainPanel.add(welcomePanel, cardLayoutNames.get(0));
        mainPanel.add(playPanel, cardLayoutNames.get(1));

        add(mainPanel);

        mainCardLayout.show(mainPanel, cardLayoutNames.get(cardLayoutIndex));

        makeQuestion();

        setVisible(true);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        // frame.pack(); // this would squeeze everything and pack up
    }

    private void makeQuestion() {
        int choice = rand.nextInt(3);
        if (choice == 0) {
            mathQuestion = Addition.makeAddition(rand, 1500);
        } else if (choice == 1) {
            mathQuestion = Subtraction.makeSubtraction(rand, 100);
        } else {
            mathQuestion = Multiplication.makeMultiplication(rand, 256);
        }
        question.setText(mathQuestion.display());
        answer.setText("Answer = ?");
        questionLogged = false;
    }

    private void logQuestion() {
        if (!questionLogged) {
            journal.append(String.format("%s = %s\n", mathQuestion.display(), mathQuestion.getValue()));
            questionLogged = true;
        }
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        String command = e.getActionCommand();
        System.out.println("command=" + command);
        if (command.equals(DISPLAY_ANSWER_COMMAND)) {
            answer.setText(String.format("Answer = %d", mathQuestion.getValue()));
            logQuestion();
        } else if (command.equals(PLAY_AGAIN_COMMAND)) {
            logQuestion();
            makeQuestion();
        }
    }
}
