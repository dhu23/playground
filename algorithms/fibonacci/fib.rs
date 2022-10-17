fn main() {
    // println!("fib30 {}", fib_recursive(30));
    println!("{}", time_test2());
}

fn fib_recursive(i: u32) -> u32 {
    if i < 2 {
        i
    } else {
        fib_recursive(i-2) + fib_recursive(i-1)
    }
}

fn time_test2() -> u32 {
    let mut ret: u32 = 0;
    for i in 1..40 {
        ret += fib_recursive(i);
    }
    ret
}
