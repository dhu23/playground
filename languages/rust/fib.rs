// n --> f
// 0     0
// 1     1
// 2     1
// 3     2
// 4     3
fn main() {
    let fib2 = fib_slow(2);
    let fib10 = fib_slow(10);
    let fib20 = fib_slow(20);
    println!("fib(2) = {}, fib(10) = {}, fib(20) = {}", fib2, fib10, fib20);
    
    let fib2 = fib_fast(2);
    let fib10 = fib_fast(10);
    let fib20 = fib_fast(20);
    println!("fib(2) = {}, fib(10) = {}, fib(20) = {}", fib2, fib10, fib20);
}

fn fib_slow(n: u32) -> u32 {
    if n == 0 {
        0
    } else if n == 1 {
        1
    } else {
        fib_slow(n-2) + fib_slow(n-1)
    }
}

fn fib_fast(n: u32) -> u32 {
    if n == 0 {
        0
    } else if n == 1 {
        1
    } else {
        let mut a = 0;  // b
        let mut b = 1;  // a+b
        for _i in 0..n-1 {
            let c = a+b;
            a = b;
            b = c;
        }
        b
    }
}
