package attendence.gui;

import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.AbstractTableModel;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.*;
import java.util.List;

public class MainFrame extends JFrame implements ActionListener, ListSelectionListener {
    JRadioButton studentViewRadioButton;
    JRadioButton eventViewRadioButton;
    JTable athleteTable;
    JTable breakdownTable;
    JButton viewSwitchButton;
    JPanel centerPanel;
    CardLayout viewCardLayout = new CardLayout();
    int cardDisplayIndex = 0;
    List<String> viewCardLayoutNames = new ArrayList<>(Arrays.asList("athlete", "breakdown"));
    Athlete selectedAthlete = null;
    MyDataModel myDataModel;

    public MainFrame() {
        setSize(1024, 1024);
        setLocation(512, 0);
        setTitle("Attendance");

        this.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                super.windowClosing(e);
                System.exit(0);
            }
        });

        JPanel mainPanel = new JPanel();
        mainPanel.setLayout(new BorderLayout());

        // set up top
        JPanel topPanel = new JPanel();

        topPanel.setLayout(new FlowLayout());
        studentViewRadioButton = new JRadioButton("Student View");
        studentViewRadioButton.setSelected(true);
        eventViewRadioButton = new JRadioButton("Event View");
        eventViewRadioButton.setSelected(false);
        viewSwitchButton = new JButton("View");
        viewSwitchButton.addActionListener(this);
        viewSwitchButton.setEnabled(false);

        topPanel.add(studentViewRadioButton);
        topPanel.add(eventViewRadioButton);
        topPanel.add(viewSwitchButton);

        mainPanel.add(topPanel, BorderLayout.NORTH);

        // set up left
        // set up right
        // set up bottom
        // set up center
        centerPanel = new JPanel();
        centerPanel.setLayout(viewCardLayout);

        Object[][] data = {
                {"Kathy(OLD)", "Smith", "Snowboarding", new Integer(5), new Boolean(false)},
                {"John(OLD)", "Doe", "Rowing", new Integer(3), new Boolean(true)},
                {"Sue(OLD)", "Black", "Knitting", new Integer(2), new Boolean(false)},
                {"Jane(OLD)", "White", "Speed reading", new Integer(20), new Boolean(true)},
                {"Joe(OLD)", "Brown", "Pool", new Integer(10), new Boolean(false)}
        };
        String[] columnNames = {"First Name", "Last Name", "Sport", "# of Years", "Vegetarian"};

        myDataModel = new MyDataModel();
        athleteTable = new JTable(myDataModel);
        athleteTable.setPreferredScrollableViewportSize(new Dimension(1000, 200));
        athleteTable.setFillsViewportHeight(true);

        athleteTable.getSelectionModel().addListSelectionListener(this);

        JScrollPane athleteScrollPane = new JScrollPane(athleteTable);

        centerPanel.add(athleteScrollPane, "athlete");
        // mainPanel.add(athleteTable.getTableHeader(), BorderLayout.PAGE_START);

        breakdownTable = new JTable(data, columnNames);
        JScrollPane breakdownScrollPane = new JScrollPane(breakdownTable);

        centerPanel.add(breakdownScrollPane, "breakdown");

        mainPanel.add(centerPanel, BorderLayout.CENTER);

        viewCardLayout.show(centerPanel, viewCardLayoutNames.get(cardDisplayIndex));

        add(mainPanel);
        setVisible(true);
    }

    @Override
    public void actionPerformed(ActionEvent actionEvent) {
        if (actionEvent.getActionCommand() == "View") {
            System.out.println("VIEW BUTTON CLICKED");
            cardDisplayIndex += 1;
            cardDisplayIndex = cardDisplayIndex % viewCardLayoutNames.size();
            viewCardLayout.show(centerPanel, viewCardLayoutNames.get(cardDisplayIndex));

            if (selectedAthlete != null) {
                System.out.println("Showing participation for " + selectedAthlete.firstName);
                System.out.println(myDataModel.getAthleteParticipation(selectedAthlete));
            } else {
                System.err.println("Something wrong!!!!!!! selected shouldn't be null if button is clicked");
            }
        }
    }

    @Override
    public void valueChanged(ListSelectionEvent listSelectionEvent) {
        if (listSelectionEvent.getValueIsAdjusting()) {
            return;
        }
        int rowIdx = athleteTable.getSelectionModel().getLeadSelectionIndex();
        System.out.println("row idx:" + rowIdx);

        selectedAthlete = myDataModel.getAthletes().get(rowIdx);
        System.out.println("selected " + selectedAthlete.firstName);
        viewSwitchButton.setEnabled(true);
    }


//    private static class MyList<E> extends ArrayList<E> {
//        public MyList() {
//
//        }
//    }

    private static class Athlete {
        public String firstName = "";
        public String lastName = "";
        public String sport = "";
        public int numOfYears = 0;
        public boolean isVegetarian = false;

        public Athlete(String fn, String ln, String s, int noy, boolean v) {
            firstName = fn;
            lastName = ln;
            sport = s;
            numOfYears = noy;
            isVegetarian = v;
        }
    }


    public static class MyDataModel extends AbstractTableModel {
        public static String[] COLUMN_FIELDS = {"First Name", "Last Name", "Sport", "# of Years", "Vegetarian", "# of Competitions"};
//        private List<Athlete> data = Arrays.asList(
//                new Athlete("Kathy", "Smith", "Snowboarding", new Integer(5), new Boolean(false)),
//                new Athlete("John", "Doe", "Rowing", new Integer(3), new Boolean(true)),
//                new Athlete("Sue", "Black", "Knitting", new Integer(2), new Boolean(false)),
//                new Athlete("Jane", "White", "Speed reading", new Integer(20), new Boolean(true)),
//                new Athlete("Joe", "Brown", "Pool", new Integer(10), new Boolean(false))
//        );
        private List<Athlete> data;
        private Map<Integer, Set<String>> matchParticipation; // year -> list of people that participated

//        public MyDataModel (List<Athlete> athletes, Map<Integer, Set<String>> matchParticipation) {
//            this.data = athletes;
//            this.matchParticipation = matchParticipation;
//        }

        public MyDataModel() {
            data = new ArrayList<>();
            // read data from file
            data.add(new Athlete("Kathy", "Smith", "Snowboarding", new Integer(5), new Boolean(false)));
            data.add(new Athlete("John", "Doe", "Rowing", new Integer(3), new Boolean(true)));
            data.add(new Athlete("Sue", "Black", "Knitting", new Integer(2), new Boolean(false)));
            data.add(new Athlete("Jane", "White", "Speed reading", new Integer(20), new Boolean(true)));
            data.add(new Athlete("Joe", "Brown", "Pool", new Integer(10), new Boolean(false)));

            matchParticipation = new HashMap<>();
            matchParticipation.put(2004, new HashSet<>(Arrays.asList("Kathy Smith", "Sue Black")));
            matchParticipation.put(2008, new HashSet<>(Arrays.asList("Sue Black")));
        }

        public List<Athlete> getAthletes() {
            return data;
        }

        public Set<Integer> getAthleteParticipation(Athlete athlete) {
            Set<Integer> years = new HashSet<>();
            String fullName = String.format("%s %s", athlete.firstName, athlete.lastName);
            for (Map.Entry<Integer, Set<String>> entry : matchParticipation.entrySet()) {
                if (entry.getValue().contains(fullName)) {
                    years.add(entry.getKey());
                }
            }
            return years;
        }

        @Override
        public int getRowCount() {
            return data.size();
        }

        @Override
        public int getColumnCount() {
            // return columnNames.length;
            // return Athlete.COLUMN_FIELDS.length;
            return COLUMN_FIELDS.length;
        }

        // simulates Object[][] call at position (i, j)
        @Override
        public Object getValueAt(int row, int col) {
            // return data[row][col];
            // return data.get(row).getField(col);
            switch(col) {
                case 0:
                    return data.get(row).firstName;
                case 1:
                    return data.get(row).lastName;
                case 2:
                    return data.get(row).sport;
                case 3:
                    return data.get(row).numOfYears;
                case 4:
                    return data.get(row).isVegetarian;
                case 5:
                    return getAthleteParticipation(data.get(row)).size();
                default:
                    throw new RuntimeException("bad col:" + col);
            }
        }

        @Override
        public String getColumnName(int col) {
            return COLUMN_FIELDS[col];
        }

        @Override
        public Class<?> getColumnClass(int col) {
            // return getValueAt(0, col).getClass();
            // return Athlete.getColumnClass(col);
            switch(col) {
                case 0:
                case 1:
                case 2:
                    return String.class;
                case 3:
                case 5:
                    return Integer.class;
                case 4:
                    return Boolean.class;
                default:
                    throw new RuntimeException("bad col:" + col);
            }
        }
    }
}
