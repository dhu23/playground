import java.util.Scanner;
import java.util.Map;
import java.util.HashMap;
import java.io.File;
import java.io.FileWriter;

// target acct2342f  -> 123
//
// avg each account len = N, you have M accounts
// N * M comparison

// acct1 --> Account for acct1 --> 1+3+3+1 = 8
// acct2 --> Account for acct2 --> 9
// acct3 --> Account for acct3 --> 10
//
// acct12 -> 10
// acct21
//
// 0 -> M-1

public class ATM {
    private Map<String, Account> accounts;
    private Scanner scanner;
    // private MessageReader reader;
    private Account activeAccount;

    public ATM() {
        accounts = new HashMap<String, Account>();
        // read in account information
        // loadStaticAccount(); 
        loadAccountFromFile();
        scanner = new Scanner(System.in);
        activeAccount = null;
    }

    public boolean login() {
        System.out.println(" ===== LOGIN =====");

        while (activeAccount == null) {
            System.out.println("please enter your user name:");
            String userName = scanner.nextLine();

            if (userName.equals("%shutdown%")) {
                break;
            }

            System.out.println("Please enter your password:");
            String password = scanner.nextLine();

            Account acct = accounts.get(userName);
            if (acct == null) {
                System.out.println("account not found: " + userName);
            } else {
                if (acct.getPassword().equals(password)) {
                    System.out.println("login successfully!");
                    activeAccount = acct;
                } else {
                    System.out.println("login failed!");
                }
            }
        } 
        return activeAccount != null;
    }

    public void transact() {
        while (true) {
            displayTransactionMenu();
            
            String selection = scanner.nextLine();
            if (selection.equals("1")) { 
                // account deposit
                System.out.println("input deposit cash:");
                int cash = Integer.valueOf(scanner.nextLine());
                activeAccount.deposit(cash);
                                         
            } else if (selection.equals("2")) { 
                // account withdraw
                System.out.println("input wthdraw cash:");
                int cash = Integer.valueOf(scanner.nextLine());
                activeAccount.withdraw(cash);
            } else if (selection.equals("3")) {
                activeAccount.report();

            } else if (selection.equals("4")) {
                System.out.println("exiting...");

                try {
                    // update files
                    FileWriter fileWriter = new FileWriter("ATMProfile.tmp.txt");
                    for (Map.Entry<String, Account> entry : accounts.entrySet()) {
                        Account acct = entry.getValue();
                        fileWriter.write(acct.toFileLine());
                    }
                    fileWriter.close();
                } catch (Exception e) {
                    System.out.println("File writing failed");
                }

                activeAccount = null;
                break;
            } else {
                System.out.println("Invalid selection, please enter 1, 2, 3, 4");
            }
        }

        // you get here by hitting 4
    }

    public void exit() {
        System.out.println("Exiting...Good Bye!");
    }

    private void loadStaticAccount() {
        accounts.put("acct1", new Account("acct1", "123", 0));
        accounts.put("acct2", new Account("acct2", "321", 100));
        accounts.put("acct3", new Account("acct3", "abc", 200));
        accounts.put("acct4", new Account("acct4", "def", 300));
        accounts.put("acct5", new Account("acct5", "xyz", 400));
        System.out.println(accounts.toString());
        System.out.println("loaded " + accounts.size());
    }

    private void loadAccountFromFile() {
        // read from a file
        System.out.println("Reading from Account file ====>");
        File inputFile = new File("ATMProfile.txt");
        try {
            Scanner fileScanner = new Scanner(inputFile);
            while (fileScanner.hasNextLine()) {
                String inputLine = fileScanner.nextLine();

                // just read in a line
                // parse this inputline to build an Account
                if (!inputLine.isEmpty()) {
                    String[] tokens = inputLine.split(",");
                    if (tokens.length != 3) {
                        System.out.println("bad line: " + inputLine);
                    } else {
                        Account account = new Account(tokens[0], tokens[1], Integer.valueOf(tokens[2]));
                        accounts.put(tokens[0], account); 
                        System.out.println("read in account: " + account.toString());
                    }
                }
            }
        } catch (Exception e) {
            System.out.println("Failed to read file");
        }
    }

    private void displayTransactionMenu() {
        System.out.println("What do you want to do?");
        System.out.println("1. deposit");
        System.out.println("2. withdraw");
        System.out.println("3. check summary");
        System.out.println("4. exit");
    }
}
