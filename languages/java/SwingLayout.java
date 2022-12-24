import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.border.TitledBorder;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.ChangeEvent;

public class SwingLayout extends JFrame 
    implements 
    ActionListener, 
    ListSelectionListener, 
    KeyListener,
    ChangeListener, 
    MouseListener,
    MouseMotionListener,
    MouseWheelListener {
    // class MyWindowListener implements WindowListener {
    //     @Override
    //     public void windowOpened(WindowEvent e) {
    //         System.out.println("Window Opened");
    //     }

    //     @Override
    //     public void windowClosing(WindowEvent e) {
    //         System.out.println("Window Closing...");
    //         System.exit(0);
    //     }

    //     @Override
    //     public void windowClosed(WindowEvent e) {
    //         System.out.println("Window Closed");
    //     }

    //     @Override
    //     public void windowIconified(WindowEvent e) {
    //         System.out.println("Window Iconified");
    //     }

    //     @Override
    //     public void windowDeiconified(WindowEvent e) {
    //         System.out.println("Window Deiconified");
    //     }

    //     @Override
    //     public void windowActivated(WindowEvent e) {
    //         System.out.println("Window Activated");
    //     }

    //     @Override
    //     public void windowDeactivated(WindowEvent e) {
    //         System.out.println("Window Deactivated");
    //     }
    // }

    // class MyWindowListener extends WindowAdapter {
    //     @Override
    //     public void windowClosing(WindowEvent e) {
    //         System.exit(0);
    //     }
    // }

    private JTextArea myTextArea;
    private JCheckBox myCheckBox1;
    private JCheckBox myCheckBox2;
    private JList<String> list;
    private JSlider slider;

    private static String BUTTON_ACTION_COMMAND = "Button";
    private static String CHECKBOX_1_ACTION_COMMAND = "CB1";
    private static String CHECKBOX_2_ACTION_COMMAND = "CB2";

    // a shared ActionListener
    // class MyActionListener implements ActionListener {
    //     @Override
    //     public void actionPerformed(ActionEvent e) {
    //         String actionCommand = e.getActionCommand();
    //         if (actionCommand.equals(BUTTON_ACTION_COMMAND)) {
    //             myTextArea.setText("Button pressed");
    //         } else if (actionCommand.equals(CHECKBOX_1_ACTION_COMMAND)) {
    //             myTextArea.setText("Checkbox 1 " + myCheckBox1.isSelected());
    //         } else if (actionCommand.equals(CHECKBOX_2_ACTION_COMMAND)) {
    //             myTextArea.setText("Checkbox 2 " + myCheckBox2.isSelected());
    //         }
    //     }
    // }
    
    // for ActionListener
    @Override
    public void actionPerformed(ActionEvent e) {
        String actionCommand = e.getActionCommand();
        if (actionCommand.equals(BUTTON_ACTION_COMMAND)) {
            myTextArea.setText("Button pressed");
        } else if (actionCommand.equals(CHECKBOX_1_ACTION_COMMAND)) {
            myTextArea.setText("Checkbox 1 " + myCheckBox1.isSelected());
        } else if (actionCommand.equals(CHECKBOX_2_ACTION_COMMAND)) {
            myTextArea.setText("Checkbox 2 " + myCheckBox2.isSelected());
        }
    }

    // for ListSelectionListener
    @Override
    public void valueChanged(ListSelectionEvent e) {
        myTextArea.setText(list.getSelectedValue());
    }

    // for KeyListener
    @Override
    public void keyTyped(KeyEvent e) {
        myTextArea.setText("Typed " + e.getKeyChar());
    }

    @Override
    public void keyPressed(KeyEvent e) {
        myTextArea.setText("Pressed " + e.getKeyChar());
    }

    @Override
    public void keyReleased(KeyEvent e) {
        myTextArea.setText("Released " + e.getKeyChar());
    }

    // for ChangeListener
    @Override
    public void stateChanged(ChangeEvent e) {
        JSlider s = (JSlider) e.getSource();
        myTextArea.setText("slider = " + s.getValue());
    }

    // for MouseListener
    @Override
    public void mouseClicked(MouseEvent e) {
        myTextArea.setText("Clicked " + e.getX() + "," + e.getY());
    }

    @Override
    public void mousePressed(MouseEvent e) {
        myTextArea.setText("Pressed " + e.getX() + "," + e.getY());
    }

    @Override
    public void mouseReleased(MouseEvent e) {
        myTextArea.setText("Released " + e.getX() + "," + e.getY());
    }

    @Override
    public void mouseEntered(MouseEvent e) {
        myTextArea.setText("Entered");
    }

    @Override
    public void mouseExited(MouseEvent e) {
        myTextArea.setText("Exited");
    }

    // for MouseMotionLister
    @Override
    public void mouseDragged(MouseEvent e) {
        myTextArea.setText("Dragged" + e.getX() + "," + e.getY());
    }

    @Override
    public void mouseMoved(MouseEvent e) {
        myTextArea.setText("Moved " + e.getX() + "," + e.getY());
    }

    // for MouseWheelListener
    @Override
    public void mouseWheelMoved(MouseWheelEvent e) {
        myTextArea.setText("mouseWheelMoved " + e.getUnitsToScroll());
    }

    public SwingLayout() {

        setSize(640, 400);
        setLocation(1920, 0);
        setTitle("WindowTitle");

        // MyWindowListener windowListener = new MyWindowListener();
        // this.addWindowListener(windowListener);
        this.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                System.exit(0);
            }
        });

        // MyActionListener myActionListener = new MyActionListener();

        // the main panel, the whole thing where everything sits in
        JPanel mainPanel = new JPanel();
        mainPanel.setBorder(new TitledBorder("PanelCaption"));
        mainPanel.setLayout(new GridLayout(2, 2));
        // end of main panel
        
        JPanel panel1 = new JPanel();
        panel1.setBorder(new TitledBorder("Panel"));
        panel1.setLayout(new GridLayout(1, 2));

        list = new JList<>(new String[] {"Item 1", "Item 2", "Item 3", "Item 4", "Item 5"});
        panel1.add(list);
        list.addListSelectionListener(this);

        JPanel radioButtonPanel = new JPanel();
        JRadioButton radioButton1 = new JRadioButton("RadioButton1");
        radioButton1.setSelected(true);
        JRadioButton radioButton2 = new JRadioButton("RadioButton2");
        JRadioButton radioButton3 = new JRadioButton("RadioButton3");
        JRadioButton radioButton4 = new JRadioButton("InactiveRadio");
        radioButton4.setEnabled(false);
        radioButtonPanel.setLayout(new BoxLayout(radioButtonPanel, BoxLayout.Y_AXIS));
        radioButtonPanel.add(radioButton1);
        radioButtonPanel.add(radioButton2);
        radioButtonPanel.add(radioButton3);
        radioButtonPanel.add(radioButton4);

        JPanel panel1_r = new JPanel();
        panel1_r.setLayout(new BorderLayout());
        panel1_r.add(radioButtonPanel, BorderLayout.CENTER);

        JButton myButton = new JButton("Button");
        panel1_r.add(myButton, BorderLayout.SOUTH);

        myButton.setActionCommand(BUTTON_ACTION_COMMAND);
        // myButton.addActionListener(new ActionListener() {
        //     @Override
        //     public void actionPerformed(ActionEvent e) {
        //         myTextArea.setText("Button pressed: " + e.getActionCommand());
        //     }
        // });
        
        // myButton.addActionListener(myActionListener);
        myButton.addActionListener(this);

        panel1.add(panel1_r);

        mainPanel.add(panel1);

        JTabbedPane tabbedPane = new JTabbedPane();
        JPanel tabPanel1 = new JPanel();
        tabPanel1.setLayout(new BorderLayout());

        JPanel tabPanelCheckboxPanel = new JPanel();
        tabPanelCheckboxPanel.setLayout(new BoxLayout(tabPanelCheckboxPanel, BoxLayout.Y_AXIS));

        myCheckBox1 = new JCheckBox("UncheckedCheckBox");
        tabPanelCheckboxPanel.add(myCheckBox1);
        myCheckBox1.setActionCommand(CHECKBOX_1_ACTION_COMMAND);
        // myCheckBox1.addActionListener(new ActionListener() {
        //     @Override
        //     public void actionPerformed(ActionEvent e) {
        //         myTextArea.setText("Checkbox1 " + myCheckBox1.isSelected());
        //     }
        // });
        
        // myCheckBox1.addActionListener(myActionListener);
        myCheckBox1.addActionListener(this);

        myCheckBox2 = new JCheckBox("CheckedCheckBox");
        myCheckBox2.setSelected(true);
        tabPanelCheckboxPanel.add(myCheckBox2);
        myCheckBox2.setActionCommand(CHECKBOX_2_ACTION_COMMAND);
        // myCheckBox2.addActionListener(new ActionListener() {
        //     @Override
        //     public void actionPerformed(ActionEvent e) {
        //         myTextArea.setText("Checkbox2 " + myCheckBox2.isSelected());
        //     }
        // });
        
        // myCheckBox2.addActionListener(myActionListener);
        myCheckBox2.addActionListener(this);

        JCheckBox cb2 = new JCheckBox("InactiveCheckBox");
        cb2.setEnabled(false);
        tabPanelCheckboxPanel.add(cb2);
        tabPanel1.add(tabPanelCheckboxPanel, BorderLayout.CENTER);

        slider = new JSlider(0, 100, 70);
        tabPanel1.add(slider, BorderLayout.SOUTH);
        slider.addChangeListener(this);

        tabbedPane.addTab("SelectedTab", tabPanel1);
        tabbedPane.addTab("OtherTab", new JPanel());

        mainPanel.add(tabbedPane);
        
        JPanel panel3 = new JPanel();
        panel3.setLayout(new BoxLayout(panel3, BoxLayout.Y_AXIS));

        JTextField textField = new JTextField("TextField");
        panel3.add(textField);
        textField.addKeyListener(this);

        panel3.add(new JPasswordField("PasswordField"));
        JComboBox<String> combo = new JComboBox<String>(new String[] {"Item 1", "Item 2"});
        panel3.add(combo);

        JPanel panel4 = new JPanel();
        panel4.setLayout(new BorderLayout());
        panel4.add(panel3, BorderLayout.NORTH);
        mainPanel.add(panel4);
        // Panel3 added to Panel4 (bottom left) as NORTH so it stays at the top

        myTextArea = new JTextArea("TextArea");
        mainPanel.add(myTextArea);

        myTextArea.addMouseListener(this);
        myTextArea.addMouseMotionListener(this);
        myTextArea.addMouseWheelListener(this);
        
        add(mainPanel);

        setVisible(true);
    }

    public static void main(String[] args) {
        new SwingLayout();
    }
}
