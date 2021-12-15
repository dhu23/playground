fn main() {
    no_else_if();
    loop_label();
    break_loop_with_value();
    for_loop();
}


fn no_else_if() {
    let flag = true;
    // let x = if flag { 5 }; // compile fail. 
    let x = if flag { 5 } else { 6 };
    // if without else would be automatically evaluated to ()
    println!("x is {}", x);
}

fn loop_label() {
    let mut count = 0;
    'counting_up: loop {
        println!("count = {}", count);
        let mut remaining = 10;

        loop {
            println!("remaining = {}", remaining);
            if remaining == 9 {
                break;
            }
            if count == 2 {
                break 'counting_up;
            }
            remaining -= 1;
        }

        count += 1;
    }
    println!("End count = {}", count);
}

fn break_loop_with_value() {
    let mut counter = 0; // if it were 20, then the loop won't return

    let result = loop {
        counter += 1;

        if counter == 10 {
            break counter * 2;
        }
    };

    println!("The result is {}", result);
}

fn for_loop() {
    for number in (1..4).rev() {
        println!("{}!", number);
    }
    println!("LIFTOFF!");
}
