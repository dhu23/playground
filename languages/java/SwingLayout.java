import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.border.TitledBorder;

public class SwingLayout extends JFrame {
    public SwingLayout() {

        setSize(640, 400);
        setLocation(1920, 0);
        setTitle("WindowTitle");

        // the main panel, the whole thing where everything sits in
        JPanel mainPanel = new JPanel();
        mainPanel.setBorder(new TitledBorder("PanelCaption"));
        mainPanel.setLayout(new GridLayout(2, 2));
        // end of main panel
        
        JPanel panel1 = new JPanel();
        panel1.setBorder(new TitledBorder("Panel"));
        panel1.setLayout(new GridLayout(1, 2));

        JList<String> list = new JList<>(new String[] {"Item 1", "Item 2", "Item 3", "Item 4", "Item 5"});
        panel1.add(list);

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
        panel1_r.add(new JButton("Button"), BorderLayout.SOUTH);

        panel1.add(panel1_r);

        mainPanel.add(panel1);

        JTabbedPane tabbedPane = new JTabbedPane();
        JPanel tabPanel1 = new JPanel();
        tabPanel1.setLayout(new BorderLayout());

        JPanel tabPanelCheckboxPanel = new JPanel();
        tabPanelCheckboxPanel.setLayout(new BoxLayout(tabPanelCheckboxPanel, BoxLayout.Y_AXIS));
        tabPanelCheckboxPanel.add(new JCheckBox("UncheckedCheckBox"));
        JCheckBox cb = new JCheckBox("CheckedCheckBox");
        cb.setSelected(true);
        tabPanelCheckboxPanel.add(cb);
        JCheckBox cb2 = new JCheckBox("InactiveCheckBox");
        cb2.setEnabled(false);
        tabPanelCheckboxPanel.add(cb2);
        tabPanel1.add(tabPanelCheckboxPanel, BorderLayout.CENTER);
        tabPanel1.add(new JSlider(0, 100, 70), BorderLayout.SOUTH);
        tabbedPane.addTab("SelectedTab", tabPanel1);
        tabbedPane.addTab("OtherTab", new JPanel());

        mainPanel.add(tabbedPane);
        
        JPanel panel3 = new JPanel();
        panel3.setLayout(new BoxLayout(panel3, BoxLayout.Y_AXIS));
        panel3.add(new JTextField("TextField"));
        panel3.add(new JPasswordField("PasswordField"));
        JComboBox<String> combo = new JComboBox<String>(new String[] {"Item 1", "Item 2"});
        panel3.add(combo);

        JPanel panel4 = new JPanel();
        panel4.setLayout(new BorderLayout());
        panel4.add(panel3, BorderLayout.NORTH);
        mainPanel.add(panel4);
        // Panel3 added to Panel4 (bottom left) as NORTH so it stays at the top

        mainPanel.add(new JTextArea("TextArea"));
        
        add(mainPanel);

        setVisible(true);
    }

    public static void main(String[] args) {
        new SwingLayout();
    }
}
