import java.util.Scanner;
import java.util.Map;
import java.util.HashMap;

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

    public ATM() {
        accounts = new HashMap<String, Account>();
        // read in account information
        loadAccount();
    }

    public void login() {
        Scanner scanner = new Scanner(System.in);
        System.out.println("please enter your user name:");
        String userName = scanner.nextLine();

        System.out.println("Please enter your password:");
        String password = scanner.nextLine();
        
        Account acct = accounts.get(userName);
        if (acct == null) {
            System.out.println("account not found: " + userName);
        } else {
            if (acct.getPassword().equals(password)) {
                System.out.println("login successfully!");
            } else {
                System.out.println("login failed!");
            }
        }
    }

    public void exit() {
        System.out.println("Exiting...Good Bye!");
    }

    private void loadAccount() {
        accounts.put("acct1", new Account("acct1", "123", 0));
        accounts.put("acct2", new Account("acct2", "321", 100));
        accounts.put("acct3", new Account("acct3", "abc", 200));
        accounts.put("acct4", new Account("acct4", "def", 300));
        accounts.put("acct5", new Account("acct5", "xyz", 400));
        System.out.println(accounts.toString());
        System.out.println("loaded " + accounts.size());
    }
}
