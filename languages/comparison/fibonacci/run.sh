echo Running fibonacci comparision
#echo With kdb/q
#QHOME=~/q
#time rlwrap -r ~/q/l32/q fib.q -q
#
#echo With Python
#time python fibonacci.py

echo With Haskell
ghc Fib.hs
time ./Fib
rm ./Fib

echo With Rust
rustc fib.rs
time ./fib
rm ./fib

echo With C++
g++ -o fibonacci.tsk fibonacci.m.cpp
time ./fibonacci.tsk
rm ./fibonacci.tsk

echo With Optimized Haskell
ghc -O Fib.hs
time ./Fib
rm ./Fib

echo With Java
javac Fibonacci.java
time java Fibonacci

echo With Optimized Rust
rustc -O fib.rs
time ./fib
rm ./fib

echo With Optimized C++
g++ -O3 -o fibonacci.tsk fibonacci.m.cpp
time ./fibonacci.tsk
rm ./fibonacci.tsk

