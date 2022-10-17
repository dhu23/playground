public class Fibonacci {

    public static int recursiveFib(int n) {
        if (n < 2) {
            return n;
        } else {
            return recursiveFib(n-1) + recursiveFib(n-2);
        }
    }
    
    public static int loopLinearRollFib(int n) {
        if (n == 0) {
            return 0;
        } else if (n == 1) {
            return 1;
        }

        int a = 0;
        int b = 1;

        while (n > 1) {
            int tmp = a;
            a = b;
            b += tmp;
            --n;
        }
        return b;
    }

    public static void timeTest1() {
        int diffCount = 0;
        for (int c = 0; c < 100; ++c) {
            for (int i = 1; i < 1000; ++i) {
                int fibx = loopLinearRollFib(i);
            }
        }
    }

    public static int timeTest2(int v) {
        int ret = 0;
        for (int i = 1; i < v; ++i) {
            ret += recursiveFib(i);
        }
        return ret;
    }

    public static void main(String[] args) {
        System.out.println(timeTest2(44)); // up to fib 43
    }
}
