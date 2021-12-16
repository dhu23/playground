fn main() {
    let s1 = gives_ownership(); 
    let mut s2 = String::from("hello");
    let s3 = takes_and_gives_back(s2); // s2 is moved

    // println!("s2={}", s2); // s2 moved, compile error

    s2 = String::from("hello2"); // s2 is re-assigned
    println!("s1={} s2={} s3={}", s1, s2, s3);


    let mut s = String::from("hello");
    change(&mut s);
    println!("s={}", s);

    let r1 = &mut s; // you can only have one &mut a time
    // this prevents data race at compile time
    // let r2 = &mut s; // this would be a compile error
    change(r1); // r1 scope ends here
    println!("s={}", s);

    let r2 = &mut s; // this is fine
    change(r2);
    println!("s={}", s);

    test_slice();
}

fn gives_ownership() -> String {
    let some_string = String::from("yours");
    some_string
}

fn takes_and_gives_back(a_string: String) -> String {
    a_string
}

// reference/borrowing
fn change(some_string: &mut String) {
    some_string.push_str(", world");
}

// slice
fn test_slice() {
    let s = String::from("Hello World!");
    // let slice = &mut s[0..2]; // cannot borrow as mutable, compile error
    let slice = &s[0..2];
    println!("slice={}", slice);
}
