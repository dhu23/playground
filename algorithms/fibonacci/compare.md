On my Ubuntu 22.04 computer

Test1
adding fibonacci(1) + fibonacci(2) + ... + fibonacci(39) together, which 
gives 165580140, using an inefficient recursive implementation:


Test2
same as Test1 but adding to fibonacci(41), aka two more extra terms, which
gives 433494436


Test3
same as before but adding to fibonacci(43), two more over Test2, which
gives 1134903169


       Test1                  Test2                  Test3

kdb/q: run via `time q fib.q -q`
real   1m27.847s
user   1m27.821s                    
sys    0m0.010s

Python:
real   0m45.823.s
user   0m45.804s
sys    0m0.016s

Haskell: built with `ghc Fib.hs`
real   0m10.721s              0m25.432s              1m6.435s
user   0m10.645s              0m25.328s              1m6.150s
sys    0m0.072s               0m0.100sa              0m0.280s

Rust: built with `rustc fib.rs`
real   0m1.794s               0m4.321s               0m11.318s
user   0m1.793s               0m4.317s               0m11.318s
sys    0m0.001s               0m0.004s               0m0.000s

C++: built with `g++ -o fibonacci.tsk fibonacci.m.cpp`
real   0m1.337s               0m3.254s               0m8.502s
user   0m1.329s               0m3.253s               0m8.501s
sys    0m0.008s               0m0.001s               0m0.001s

Haskell: built with `ghc -O Fib.hs`
real   0m1.183s               0m2.785s               0m7.476s
user   0m1.182s               0m2.781s               0m7.471s
sys    0m0.001s               0m0.004s               0m0.004s

Java: built with `javac Fibonacci.java` without optimization
real   0m0.726s               0m1.652s               0m4.295s
user   0m0.709s               0m1.647s               0m4.283s
sys    0m0.032s               0m0.021s               0m0.024s

Rust: built with `rustc -O fib.rs`
real   0m0.522s               0m1.177s               0m3.122s
user   0m0.521s               0m1.172s               0m3.118s
sys    0m0.001s               0m0.005s               0m0.004s

C++: built with `g++ -O3 -o fibonacci.tsk fibonacci.m.cpp`
real   0m0.399s               0m0.902s               0m2.407s
user   0m0.398s               0m0.894s               0m2.406s
sys    0m0.001s               0m0.008s               0m0.000s

