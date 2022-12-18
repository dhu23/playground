public class ATMMain {
    public static void main(String[] args) {
        // start atm machine
        ATM atm = new ATM();
        
        // ask user input to log in
        atm.login();
        
        // some transactions (deposit or withdrawl) within your account
        
        // exit
        atm.exit();
    }
}
