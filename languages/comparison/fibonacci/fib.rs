fn main() {
    println!("{}", time_test2(44)); // up to fib 43
}

fn fib_recursive(i: u32) -> u32 {
    if i < 2 {
        i
    } else {
        fib_recursive(i-2) + fib_recursive(i-1)
    }
}

fn time_test2(v: u32) -> u32 {
    let mut ret: u32 = 0;
    for i in 1..v {
        ret += fib_recursive(i);
    }
    ret
}
