import java.io.File;
import java.nio.ByteBuffer;
import java.nio.file.Path;
import java.nio.channels.AsynchronousFileChannel;
import java.nio.file.StandardOpenOption;
import java.nio.file.Paths;
import java.time.Instant;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeFormatterBuilder;
import java.time.ZoneOffset;

import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;


public class TryOut {
    public static void tryStringSplit() {
        String emptyLine = "";
        String[] tokens = emptyLine.split("\\.");
        System.out.println(tokens.length);
        for (String i : tokens) {
            System.out.println("|" + i + "|");
        }
    }
    public static void tryFileObj() {
        File file1 = new File("/this/is/my/path/filename1.txt");
        File file2 = new File("abc/filename2.txt");
        File file3 = new File("filename3.txt");

        System.out.println("=====================");
        System.out.println(file1.getName());
        System.out.println(file1.getParent());
        System.out.println(file1.getPath());
        System.out.println(file1.getAbsolutePath());
        System.out.println(file1.getParentFile());
        System.out.println(file1.isAbsolute());
        System.out.println(file1.canRead());

        System.out.println("=====================");
        System.out.println(file2.getName());
        System.out.println(file2.getParent());
        System.out.println(file2.getPath());
        System.out.println(file2.getAbsolutePath());
        System.out.println(file2.getParentFile());
        System.out.println(file2.isAbsolute());
        System.out.println(file2.canRead());

        System.out.println("=====================");
        System.out.println(file3.getName());
        System.out.println(file3.getParent());
        System.out.println(file3.getPath());
        System.out.println(file3.getAbsolutePath());
        System.out.println(file3.getParentFile());
        System.out.println(file3.isAbsolute());
        System.out.println(file3.canRead());
    }
    public static void tryPathObj() {
        System.out.println("-------------------------------------");
        Path path1 = Paths.get("/this/is/my/path/filename1.txt");
        System.out.println(path1.toString());
        System.out.println(path1.toAbsolutePath().toString());
        System.out.println(path1.getFileName().toString());
        System.out.println(path1.getParent().toString());
        System.out.println(path1.getRoot().toString());

        System.out.println("-------------------------------------");
        Path path2 = Paths.get("abs/filename2.txt");
        System.out.println(path2.toString());
        System.out.println(path2.toAbsolutePath().toString());
        System.out.println(path2.getFileName().toString());
        System.out.println(path2.getParent().toString());
        // System.out.println(path2.getRoot().toString()); returns null from getRoot()
        
        Path dir = Paths.get("/this/is/my/path");
        Path file = dir.resolve("filename.txt");
        System.out.println(dir.toString());
        System.out.println(file.toString());
    }
    public static void tryDateTimeFormatter() {
        Instant ins = Instant.now();
        DateTimeFormatter formatter = DateTimeFormatter
            .ofPattern("yyyyMMdd-HHmmss-SSSSSSZ")
            .withZone(ZoneOffset.UTC);
        String format = formatter.format(ins);
        System.out.println(ins);
        System.out.println(ins.toString());
        System.out.println(format);
        Instant ins2 = LocalDateTime
            .parse(format, formatter).atZone(ZoneOffset.UTC)
            .toInstant();
        System.out.println(ins2);
    }
    public static void tryAsyncFile() {
        Path file = Paths.get("test-file.txt");
        try {
            AsynchronousFileChannel asyncFile = AsynchronousFileChannel.open(
                    file,
                    StandardOpenOption.WRITE,
                    StandardOpenOption.CREATE);
            System.out.println("writing to file");
            asyncFile.write(ByteBuffer.wrap("whatever\nstring\ndata".getBytes()), 0).get();
            System.out.println("done writing");
        } catch (Exception e) {
            System.out.println("failed to write");
        }

        // System.out.println("reading file....");
        // AsynchronousFileChannel asyncRead = AsynchronousFileChannel.open(
        //         file, StandardOpenOption.READ);

    }
    public static class ClickerListener implements ActionListener {
        private int count = 0;
        private int idx = 0;
        private String alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        private JLabel countLabel;
        private JTextField text1;
        private JTextField text2;

        public ClickerListener(JLabel label, JTextField text1, JTextField text2) {
            countLabel = label;
            this.text1 = text1; 
            this.text2 = text2; 
        }

        @Override
        public void actionPerformed(ActionEvent e) {
            System.out.println("clicked!!");
            ++count;
            ++idx;
            if (idx == alphabet.length()) { idx = 0; }
            // countLabel.setText("Number of clicks: " + count);
            try {
                int x = Integer.valueOf(text1.getText());
                int y = Integer.valueOf(text2.getText());
                countLabel.setText(String.format("%s + %s = %d", x, y, x+y));
            } catch (Exception ex) {
                countLabel.setText(String.format("cannot add %s and %s", text1.getText(), text2.getText()));
            }
        }
    }
    public static void trySwing() {
        JFrame frame = new JFrame("TestFrame");
        frame.setVisible(true);
        frame.setSize(800, 400);

        frame.setLayout(new FlowLayout()); // instead of CardLayout

        JLabel label1 = new JLabel("Username");
        frame.add(label1);
        JTextField text1 = new JTextField(20); // number of characters to type
        frame.add(text1);

        JLabel label2 = new JLabel("Password");
        frame.add(label2);
        JTextField text2 = new JTextField(20);
        frame.add(text2);

        JButton button = new JButton("Submit");
        frame.add(button);

        JButton click = new JButton("Click Me Pleeeeeeeease!");
        // JLabel countLabel = new JLabel("Number of clicks: 0");
        JLabel countLabel = new JLabel("letter: A");
        click.addActionListener(new ClickerListener(countLabel, text1, text2));

        JPanel panel = new JPanel();
        panel.setBorder(BorderFactory.createEmptyBorder(30, 30, 10, 30));
        panel.setLayout(new GridLayout(0, 1));
        panel.add(click);
        panel.add(countLabel);

        frame.add(panel, BorderLayout.CENTER);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setTitle("Try-out GUI");
        frame.pack();
    }
    public static void main(String[] args) {
        // tryAsyncFile();
        trySwing();
    }
}
