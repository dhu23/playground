fn main() {
    let a: [i32; 5] = [1, 2, 3, 4, 5];
    println!("a is {} {} {} {} {}", a[0], a[1], a[2], a[3], a[4]);

    let b: [bool; 2] = [true; 2];
    println!("b is {} {}", b[0], b[1]);

    let x: (i32, f64, bool) = (500, 6.4, true);
    println!("x tuple is {} {} {}", x.0, x.1, x.2);

    let (x1, x2, x3) = x;
    println!("x1: {}, x2: {}, x3: {}", x1, x2, x3);
}
