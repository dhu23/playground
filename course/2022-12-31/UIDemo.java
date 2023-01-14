import javax.swing.*;
import javax.swing.border.TitledBorder;
import java.awt.event.*;
import java.awt.*;

import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;


public class UIDemo implements ActionListener {
    private static final String CLICK_ME_GAME_COMMAND = "ClickMe";
    private static final String SWITCH_COMMAND = "SwitchCard";

    private int count;
    private JLabel countLabel;
    private List<String> bodyCardLayoutNames = new ArrayList<>(Arrays.asList("tabs", "clickmegame"));
    private int currentCardIdx = 0;
    private JPanel bodyContainerPanel;
    private CardLayout bodyCardLaytout = new CardLayout();


    public static class WindowManager implements WindowListener {
        @Override
        public void windowActivated(WindowEvent e) {
            System.out.println("activated");
        }
        @Override
        public void windowClosed(WindowEvent e) {
            System.out.println("closed");
        }
        @Override
        public void windowClosing(WindowEvent e) {
            System.out.println("closing");
            System.exit(0);
        }
        @Override
        public void windowDeactivated(WindowEvent e) {
            System.out.println("deactivated");
        }
        @Override
        public void windowDeiconified(WindowEvent e) {
            System.out.println("deiconified");
        }
        @Override
        public void windowIconified(WindowEvent e) {
            System.out.println("iconified");
        }
        @Override
        public void windowOpened(WindowEvent e) {
            System.out.println("opened");
        }
    }

    public UIDemo() {
        JFrame frame = new JFrame();

        frame.setTitle("UI Demo");
        frame.setSize(800, 800);
        frame.addWindowListener(new WindowManager());


        JPanel mainPanel = new JPanel();
        mainPanel.setBorder(new TitledBorder("main"));
        mainPanel.setLayout(new BorderLayout());


        // left side of the main panel 

        JPanel leftPanel = new JPanel();
        leftPanel.setLayout(new BoxLayout(leftPanel, BoxLayout.Y_AXIS));

        JButton button1 = new JButton("Button1");
        JButton button2 = new JButton("Button2");

        leftPanel.add(button1);
        leftPanel.add(button2);

        mainPanel.add(leftPanel, BorderLayout.WEST);


        // center of the main panel
        JPanel centerPanel = new JPanel();
        centerPanel.setLayout(new BorderLayout());
        
        // top of the center
        JButton switchButton = new JButton("Switch");
        centerPanel.add(switchButton, BorderLayout.NORTH);
        switchButton.setActionCommand(SWITCH_COMMAND);
        switchButton.addActionListener(this);

        // center of the center
        bodyContainerPanel = new JPanel();

        bodyContainerPanel.setLayout(bodyCardLaytout);


        JTabbedPane tabs = new JTabbedPane();


        // creating first tab
        JPanel firstTab = new JPanel();
        firstTab.setLayout(new FlowLayout());

        JLabel l1 = new JLabel("label 1");
        JLabel l2 = new JLabel("label 2");
        JLabel l3 = new JLabel("label 3");
        JLabel l4 = new JLabel("label 4");

        firstTab.add(l1);
        firstTab.add(l2);
        firstTab.add(l3);
        firstTab.add(l4);

        // creating second tab
        JPanel secondTab = new JPanel();
        JTextArea textArea = new JTextArea("some text");
        secondTab.setLayout(new FlowLayout());
        secondTab.add(textArea);
        

        tabs.addTab("2x2 Labels", firstTab);
        tabs.addTab("textarea", secondTab);

        bodyContainerPanel.add(tabs, "tabs");

        JPanel clickMeGamePanel = new JPanel();

        count = 0;
        countLabel = new JLabel();
        countLabel.setText("count: 0");
        JButton click = new JButton("Click Me!");
        click.setActionCommand(CLICK_ME_GAME_COMMAND);
        click.addActionListener(this);

        clickMeGamePanel.add(click);
        clickMeGamePanel.add(countLabel);

        bodyContainerPanel.add(clickMeGamePanel, "clickmegame");

        bodyCardLaytout.show(bodyContainerPanel, bodyCardLayoutNames.get(currentCardIdx));

        centerPanel.add(bodyContainerPanel, BorderLayout.CENTER);

        mainPanel.add(centerPanel, BorderLayout.CENTER);


        frame.add(mainPanel);
        frame.setVisible(true);

        System.out.println("setup done!");
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        System.out.println(e);

        if (e.getActionCommand().equals(CLICK_ME_GAME_COMMAND)) {
            count += 1;
            countLabel.setText("count:" + count);
        } else if (e.getActionCommand().equals(SWITCH_COMMAND)) {
            currentCardIdx = (currentCardIdx + 1) % bodyCardLayoutNames.size();
            bodyCardLaytout.show(bodyContainerPanel, bodyCardLayoutNames.get(currentCardIdx));
        }
    }

    public static void main(String[] args) {
        new UIDemo();
    }
}

// flow layout
// border layout
// card layout
