use std::io;
// by default, Rust brings only a few types into the scope of every porgram
// in the prelude.
use rand::Rng; // traits
use std::cmp::Ordering;

fn main() {
    println!("Guess the number!");

    let secret_number = rand::thread_rng().gen_range(1..101);

    println!("The secret number is: {}", secret_number);

    loop {
        println!("Please input your guess.");

        // mut means mutable variable
        // :: indicates associated function on the type String
        let mut guess = String::new();

        // stdin() returns an instance of std::io::Stdin type
        io::stdin()
            .read_line(&mut guess) // &-ref. by default, reference is immutable
            .expect("Failed to read line");

        // rust allows us to shadow the previous value of guess
        // to avoid two unique variables, guess_str and guess_num
        let guess: u32 = match guess.trim().parse() {
            Ok(num) => num,
            Err(_) => continue,
        };

        println!("You guessed: {}", guess);

        // A match expression is made up of arms
        // match feels like how case-of works in Haskell. 
        // I wrote a Haskell version in the safe directory without safe read
        match guess.cmp(&secret_number) {
            Ordering::Less => println!("Too small!"),
            Ordering::Greater => println!("Too big!"),
            Ordering::Equal => {
                println!("You win!");
                break;
            }
        }
    }
}

// this program builds a binary crate while we load in library crate, like rand
