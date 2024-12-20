package math.game;

import javax.swing.*;
import javax.swing.text.DefaultCaret;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;

public class MathGame extends JFrame implements ActionListener {

    private static final String DISPLAY_ANSWER_COMMAND = "DisplayAnswer";
    private static final String PLAY_AGAIN_COMMAND = "PlayAgain";

    private final Random rand = new Random();
    private final JLabel question;

    private final JButton confirm;
    private final JLabel answer;
    private final JTextArea journal;

    private final JButton playAgain;

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

        JPanel mainPanel = new JPanel();
        mainPanel.setLayout(new BorderLayout());

        // bottom corner
        JPanel bottomPanel = new JPanel();
        bottomPanel.setLayout(new GridLayout(1, 3));

        confirm = new JButton("Display Answer");
        confirm.setActionCommand(DISPLAY_ANSWER_COMMAND);
        confirm.addActionListener(this);

        playAgain = new JButton("Play Again");
        playAgain.setActionCommand(PLAY_AGAIN_COMMAND);
        playAgain.addActionListener(this);

        bottomPanel.add(confirm);
        bottomPanel.add(playAgain);

        mainPanel.add(bottomPanel, BorderLayout.SOUTH);

        // top corner
        JPanel topPanel = new JPanel();
        topPanel.setLayout(new FlowLayout());

        question = new JLabel();
        question.setFont(new Font(null, Font.PLAIN, 48));
        topPanel.add(question);

        mainPanel.add(topPanel, BorderLayout.NORTH);

        // center corner
        JPanel centerPanel = new JPanel();
        centerPanel.setLayout(new FlowLayout());

        answer = new JLabel();
        answer.setFont(new Font(null, Font.PLAIN, 20));
        centerPanel.add(answer);

        journal = new JTextArea("Journal:\n", 16, 40);
        journal.setEditable(false);
        // always keep the focus at the last line of TextArea
        DefaultCaret journalCaret = (DefaultCaret) journal.getCaret();
        journalCaret.setUpdatePolicy(DefaultCaret.ALWAYS_UPDATE);
        JScrollPane journalScrollPane = new JScrollPane(journal);
        centerPanel.add(journalScrollPane);

        mainPanel.add(centerPanel, BorderLayout.CENTER);

        add(mainPanel);

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
