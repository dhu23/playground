import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.event.*;
import java.util.Random;

public class MathGameMain extends JFrame implements ActionListener, ChangeListener {  

    private static final String CONFIRM_COMMAND = "Confirm";
    private static final String PLAY_AGAIN_COMMAND = "PlayAgain";

    private Random rand = new Random();
    private int a;
    private int b;
    private JLabel question;

    // private JList<Integer> numbers;

    private JButton confirm;
    private JLabel result;
    private JButton playAgain;

    private JSlider answerSlider;
    private JLabel answer;

    private JTextArea journal;
    private MathQuestion mathQuestion;

    public class MathQuestion {
        public enum Operation {
            Addition,
            Subtraction,
            Multiplication
        }
        public int a;
        public int b;
        public Operation op;

        public MathQuestion() {
            int opChoice = rand.nextInt(3);
            if (opChoice == 0) {
                op = Operation.Addition;
                a = rand.nextInt(20);
                b = rand.nextInt(20);
                question.setText(toString());
            } else if (opChoice == 1) {
                op = Operation.Subtraction;
                a = rand.nextInt(25);
                b = rand.nextInt(a); // b < a
                question.setText(toString());
            } else {
                op = Operation.Multiplication;
                a = rand.nextInt(15);
                b = rand.nextInt(15);
                question.setText(toString());
            }
            result.setText("");
            confirm.setEnabled(true);
            playAgain.setEnabled(false);
        }

        @Override
        public String toString() {
            switch(op) {
                case Addition:
                    return String.format("%d + %d", a, b);
                case Subtraction:
                    return String.format("%d - %d", a, b);
                case Multiplication:
                    return String.format("%d x %d", a, b);
            }
            return "";
        }

        public void checkAnswer(int answer) {
            boolean ok = false;
            switch (op) {
                case Addition: 
                    ok = answer == (a+b);
                    break;
                case Subtraction:
                    ok = answer == (a-b);
                    break;
                case Multiplication:
                    ok = answer == (a*b);
                    break;
            };
            if (ok) {
                result.setText("Correct");
                playAgain.setEnabled(true);
                confirm.setEnabled(false);
                journal.append(String.format("%s = %d (right)\n", toString(), answer));
            } else {
                result.setText("Try Again!");
                journal.append(String.format("%s = %d (wrong)\n", toString(), answer));
            }
        }
    }

    public MathGameMain() {

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

        answerSlider = new JSlider(0, 250, 100);
        answerSlider.addChangeListener(this);
        answer = new JLabel(String.valueOf(answerSlider.getValue()));

        topPanel.add(question);
        topPanel.add(answerSlider);
        topPanel.add(answer);

        mainPanel.add(topPanel, BorderLayout.NORTH);

        // center corner
        journal = new JTextArea("Journal:\n", 16, 40);
        journal.setEditable(false);
        JPanel centerPanel = new JPanel();
        centerPanel.setLayout(new FlowLayout());

        centerPanel.add(new JScrollPane(journal));

        mainPanel.add(centerPanel, BorderLayout.CENTER);


        add(mainPanel);

        // setupQuestion();
        mathQuestion = new MathQuestion();

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

    @Override
    public void actionPerformed(ActionEvent e) {
        String command = e.getActionCommand();
        System.out.println("command=" + command);
        if (command.equals(CONFIRM_COMMAND)) {
            int ansVal = answerSlider.getValue();
            mathQuestion.checkAnswer(ansVal);
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
            mathQuestion = new MathQuestion();
        }
    }

    @Override
    public void stateChanged(ChangeEvent e) {
        answer.setText(String.valueOf(answerSlider.getValue()));
    }

    public static void main(String[] args) {
        new MathGameMain();
    }
}
