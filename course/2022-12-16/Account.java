public class Account {
    private String userName;
    private String password;
    // private String accountId;
    private int balance;

    public Account(String userName, String password) {
        this.userName = userName;
        this.password = password;
        this.balance = 0;
    }

    public Account(String userName, String password, int initialBalance) {
        this.userName = userName;
        this.password = password;
        this.balance = initialBalance;
    }

    public int getBalance() {
        return balance;
    }

    public String getPassword() {
        return password;
    }

    public void report() {
        System.out.println(" ===== Summary ===== ");
        System.out.println(" username:    " + userName);
        System.out.println(" balance:     " + balance);
    }

    @Override
    public String toString() {
        return String.format("Username[%s] Balance[%d]", userName, balance);
    }

    public void deposit(int cash) {
        System.out.println("depositing " + cash);
        balance += cash;
        System.out.println("new balance: " + balance);
    }

    public void withdraw(int cash) {
        if (cash <= balance) {
            System.out.println("withdrawing " + cash);
            balance -= cash;
            System.out.println("new balance: " + balance);
        } else {
            System.out.println("not enough cash!");
        }
    }
}
