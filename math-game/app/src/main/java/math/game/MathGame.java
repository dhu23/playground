package math.game;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.event.*;
import java.util.Random;

public class MathGame extends JFrame implements ActionListener {

    private static final String CONFIRM_COMMAND = "Confirm";
    private static final String PLAY_AGAIN_COMMAND = "PlayAgain";

    private final Random rand = new Random();
    private final JLabel question;

    private final JButton confirm;
    private final JLabel result;
    private final JButton playAgain;

    private JLabel answer;

    private Expression mathQuestion;

    public MathGame() {
        setSize(560, 400);
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

        // left corner
        // JPanel leftPanel = new JPanel();
        // leftPanel.setLayout(new FlowLayout());
        // mainPanel.add(leftPanel, BorderLayout.WEST);


        // bottom corner
        JPanel bottomPanel = new JPanel();
        bottomPanel.setLayout(new GridLayout(1, 3));

        confirm = new JButton("Confirm");
        confirm.setActionCommand(CONFIRM_COMMAND);
        confirm.addActionListener(this);

        result = new JLabel("");

        playAgain = new JButton("Play Again");
        playAgain.setActionCommand(PLAY_AGAIN_COMMAND);
        playAgain.addActionListener(this);

        bottomPanel.add(confirm);
        bottomPanel.add(result);
        bottomPanel.add(playAgain);

        mainPanel.add(bottomPanel, BorderLayout.SOUTH);

        // top corner
        JPanel topPanel = new JPanel();
        topPanel.setLayout(new BoxLayout(topPanel, BoxLayout.Y_AXIS));

        question = new JLabel();

        topPanel.add(question);

        mainPanel.add(topPanel, BorderLayout.NORTH);

        // center corner
        JPanel centerPanel = new JPanel();
        centerPanel.setLayout(new FlowLayout());

        mainPanel.add(centerPanel, BorderLayout.CENTER);

        add(mainPanel);

        mathQuestion = makeQuestion();

        setVisible(true);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        // frame.pack(); // this would squeeze everything and pack up
    }

    // private void setupQuestion() {
    //     a = rand.nextInt(20);
    //     b = rand.nextInt(20);
    //     question.setText(String.format("%s + %s", a, b));
    //     result.setText("");
    //     confirm.setEnabled(true);
    //     playAgain.setEnabled(false);
    // }

    private Expression makeQuestion() {
        int choice = rand.nextInt(3);
        if (choice == 0) {
            return TwoOperandAddition.makeAddition(rand, 800);
        } else if (choice == 1) {
            return TwoOperationSubtraction.makeSubtraction(rand, 100);
        } else {
            return TwoOperandMultiplication.makeMultiplication(rand, 200);
        }
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        String command = e.getActionCommand();
        System.out.println("command=" + command);
        if (command.equals(CONFIRM_COMMAND)) {
            // if (ansVal == a + b) {
            //     result.setText("Correct");
            //     playAgain.setEnabled(true);
            //     confirm.setEnabled(false);
            //     journal.append(String.format("%d + %d = %d (right)\n", a, b, a+b));
            // } else {
            //     result.setText("Try again!");
            //     journal.append(String.format("%d + %d = %d (wrong)\n", a, b, ansVal));
            // }
        } else if (command.equals(PLAY_AGAIN_COMMAND)) {
            // setupQuestion();
            mathQuestion = makeQuestion();
        }
    }
}
