import javax.swing.*;
import java.awt.event.*;
import java.awt.*;


public class UIDemo implements ActionListener {
    private int count;
    private JLabel countLabel;


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

    // public static class ClickMeEvent implements ActionListener {
    //     
    //     private JLabel countLabel;
    //     private int count;

    //     public ClickMeEvent(JLabel cl) {
    //         this.countLabel = cl;
    //         count = 0;
    //     }

    //     @Override
    //     public void actionPerformed(ActionEvent e) {
    //         count += 1;
    //         countLabel.setText("count:" + count);
    //     }
    // }
    
    public UIDemo() {
        JFrame frame = new JFrame();

        frame.setTitle("UI Demo");
        frame.setSize(512, 512);
        frame.setVisible(true);
        frame.setLayout(new FlowLayout());
        frame.addWindowListener(new WindowManager());

        count = 0;
        countLabel = new JLabel();
        countLabel.setText("count: 0");
        JButton click = new JButton("Click Me!");
        frame.add(click);
        frame.add(countLabel);

        // click.addActionListener(new ClickMeEvent(countLabel));
        click.addActionListener(this);

        System.out.println("setup done!");
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        count += 1;
        countLabel.setText("count:" + count);
    }

    public static void main(String[] args) {
        // JFrame frame = new JFrame();

        // frame.setTitle("UI Demo");
        // frame.setSize(512, 512);
        // frame.setVisible(true);
        // frame.setLayout(new FlowLayout());
        // frame.addWindowListener(new WindowManager());

        // JLabel countLabel = new JLabel();
        // countLabel.setText("count: 0");
        // JButton click = new JButton("Click Me!");
        // frame.add(click);
        // frame.add(countLabel);

        // click.addActionListener(new ClickMeEvent(countLabel));

        // System.out.println("setup done!");
        new UIDemo();
    }
}
