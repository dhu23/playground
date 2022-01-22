// use std::collections::Vec;

#[derive(Debug)]
enum SpreadsheetCell {
    Int(i32),
    Float(f64),
    Text(String),
}

fn main() {
    test_vec();
    test_string();
    test_hashmap();
}

fn test_vec() {
    let row = vec![
        SpreadsheetCell::Int(3),
        SpreadsheetCell::Text(String::from("blue")),
        SpreadsheetCell::Float(10.12),
    ];

    for i in row {
        println!("element is {:?}", i);
    }
    // row value moved already, cannot use any more

    let row2 = vec![
        SpreadsheetCell::Int(3),
        SpreadsheetCell::Text(String::from("blue")),
        SpreadsheetCell::Float(10.12),
    ];

    for i in &row2 {
        println!("element is {:?}", i);
    }
    for i in &row2 {
        println!("element is {:?}", i);
        match i {
            SpreadsheetCell::Int(i) => println!("got an int {}", i),
            SpreadsheetCell::Float(f) => println!("got a float {}", f),
            SpreadsheetCell::Text(s) => println!("got a string {}", s),
        }
    }
}

fn test_string() {
    let mut s = String::from("foo");
    s.push_str("bar");
    println!("s is {}", s);
    s.push('l');
    println!("s is {}", s);
    let s2 = "bar";
    s.push_str(s2);
    println!("s is {}", s);

    // this prints all the scalar values, including invisible chars
    for c in "नमस्ते".chars() {
        println!("{}", c);
    }

    for b in "नमस्ते".bytes() {
        println!("{}", b);
    }

    // getting grapheme clusters from string is complex, not provided by stdlib
}

fn test_hashmap() {
    // of the three common collections, vector, string and hashmap, 
    // hashmap is the least often used, so it's not included in the features
    // brought into scope automatically in the prelude
    // There is no built-in macro to construct either.
    use std::collections::HashMap;

    let mut scores = HashMap::new();

    scores.insert(String::from("Blue"), 10);
    scores.insert(String::from("Yellow"), 50);
    println!("scores {:?}", scores);

    let teams = vec![String::from("Blue"), String::from("Yellow")];
    let initial_scores = vec![10, 50];

    // need type annotation because it's possible to collect into many 
    // different data strucutes and Rust doesn't know which you want
    let scores2: HashMap<_, _> = teams.into_iter().zip(initial_scores.into_iter()).collect();
    println!("scores2: {:?}", scores2);

    let blue_team_name = String::from("Blue");
    // get returns an Option<&V>
    let score = scores.get(&blue_team_name);
    println!("score {:?}", score);

    let red_team_name = String::from("Red");
    let score2 = scores.get(&red_team_name);
    println!("score2 {:?}", score2);

    for (key, value) in &scores2 {
        println!("{}: {}", key, value);
    }

    // overwriting a value
    scores.insert(String::from("Blue"), 0);
    println!("score {:?}", scores); // Blue updated to 0

    // insert a value if the key has no value
    scores.entry(String::from("Blue")).or_insert(50); // does nothing
    scores.entry(String::from("Red")).or_insert(50); // insert
    println!("score {:?}", scores); // Blue updated to 0

    // update a value based on the old value
    let text = "hello world wonderful world";
    let mut map = HashMap::new();
    for word in text.split_whitespace() {
        let count = map.entry(word).or_insert(0); // why not &word
        *count += 1;
    }
    println!("{:?}", map);
}

