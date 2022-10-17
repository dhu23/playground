On my Ubuntu 22.04 computer

adding fibonacci(1) + fibonacci(2) + ... + fibonacci(39) together, which 
gives 165580140, using an inefficient recursive implementation:


Python:
real   0m45.823.s
user   0m45.804s
sys    0m0.016s

Java: built with `javac Fibonacci.java` without optimization
real   0m0.726s
user   0m0.709s
sys    0m0.032s

Haskell: built with `ghc Fib.hs`
real   0m10.721s
user   0m10.645s
sys    0m0.072s

Haskell: built with `ghc -O Fib.hs`
real   0m1.183s
user   0m1.182s
sys    0m0.001s

Rust: built with `rustc fib.rs`
real   0m1.794s
user   0m1.793s
sys    0m0.001s

Rust: built with `rustc -O fib.rs`
real   0m0.522s
user   0m0.521s
sys    0m0.001s

C++: built with `g++ -o fibonacci.tsk fibonacci.m.cpp`
real   0m1.337s
user   0m1.329s
sys    0m0.008s

C++: built with `g++ -O3 -o fibonacci.tsk fibonacci.m.cpp`
real   0m0.399s
user   0m0.398s
sys    0m0.001s

