use std::fs::File;

fn main() {
    let greeting_file_result = File::open("/home/daowen/github/playground/languages/rust/error.rs");

    let _greeting_file = match greeting_file_result {
        Ok(file) => println!("{:?}", file),
        Err(error) => panic!("Problem opening the file: {:?}", error),
    };

    let non_exist_file_result = File::open("/home/daowen/imaginary.rs");
    let _non_exist_file = match non_exist_file_result {
        Ok(file) => println!("{:?}", file),
        Err(error) => panic!("Problem opening the file: {:?}", error),
    };
}
