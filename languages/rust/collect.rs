// use std::collections::Vec;

#[derive(Debug)]
enum SpreadsheetCell {
    Int(i32),
    Float(f64),
    Text(String),
}

fn main() {
    test_vec();
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
