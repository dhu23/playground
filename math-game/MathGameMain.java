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

    public MathGameMain() {

        setSize(480, 120);
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

        // left cornor
        // numbers = new JList<>(new Integer[] {1, 2});
        // mainPanel.add(numbers, BorderLayout.WEST);
        JPanel leftPanel = new JPanel();
        leftPanel.setLayout(new FlowLayout());

        question = new JLabel();
        leftPanel.add(question);

        mainPanel.add(leftPanel, BorderLayout.WEST);

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

        // center
        JPanel centerPanel = new JPanel();
        centerPanel.setLayout(new BoxLayout(centerPanel, BoxLayout.Y_AXIS));

        answerSlider = new JSlider(0, 20, 10);
        answerSlider.addChangeListener(this);
        answer = new JLabel("10");

        centerPanel.add(answerSlider);
        centerPanel.add(answer);

        mainPanel.add(centerPanel, BorderLayout.CENTER);

        add(mainPanel);

        setupQuestion();

        setVisible(true);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        // frame.pack(); // this would squeeze everything and pack up
    }

    private void setupQuestion() {
        a = rand.nextInt(10);
        b = rand.nextInt(10);
        question.setText(String.format("%s + %s", a, b));
        result.setText("");
        confirm.setEnabled(true);
        playAgain.setEnabled(false);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        String command = e.getActionCommand();
        System.out.println("command=" + command);
        if (command.equals(CONFIRM_COMMAND)) {
            if (answerSlider.getValue() == a + b) {
                result.setText("Correct");
                playAgain.setEnabled(true);
                confirm.setEnabled(false);
            } else {
                result.setText("Try again!");
            }
        } else if (command.equals(PLAY_AGAIN_COMMAND)) {
            setupQuestion();
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
