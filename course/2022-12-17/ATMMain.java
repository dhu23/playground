public class ATMMain {
    public static void main(String[] args) {
        // start atm machine
        ATM atm = new ATM();

        boolean running = true;
        // RunMonitor rm = new RunMonitor(running);
        // ask user input to log in
        while (running && atm.login()) {
            // some transactions (deposit or withdrawl) within your account
            atm.transact();
        } 
        // exit
        atm.exit();
    }
}
