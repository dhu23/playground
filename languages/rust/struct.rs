struct User {
    active: bool,
    username: String,
    email: String,
    sign_in_count: u64,
}

// fn test1() {
//     let un = String::from("Some One");
//     let em = String::from("someone@somewhere.com");

//     let mut user1 = User {
//         email: em, // this line causes compile error. 
//         // String doesn't implement Copy trait
//         username: un,
//         active: true,
//         sign_in_count: 1,
//     };

//     println!("user.email={}, em={}", user1.email, em);
//     user1.email = String::from("someoneelse@somewhereelse.net");
//     println!("user.email={}", user1.email);
// }

struct Color(i32, i32, i32);
struct Point(i32, i32, i32);

struct AlwaysEqual; // Unit like struct, or a tuple struct base of ()

fn main() {
    let mut user1 = User {
        email: String::from("someone@somewhere.com"),
        username: String::from("Some One"),
        active: true,
        sign_in_count: 1,
    };

    print_user(&user1);
    user1.email = String::from("someone@somewhereelse.com");
    print_user(&user1);

    let user2 = User {
        email: String::from("another@example.com"),
        ..user1
    };
    print_user(&user2);
    // print_user(&user1); // compile error: value borrowed after partial move
    // since the username field of user1 is moved into user2
    
    let user3 = User {
        email: String::from("another2@example.com"),
        username: String::from("Some one2"),
        ..user2
    };
    print_user(&user3);
    print_user(&user2); // user2 still good

    tuple_struct();

    test_rect();
}

fn print_user(u: &User) {
    println!("user.email={}", u.email);
    println!("user.username={}", u.username);
    println!("user.active={}", u.active);
    println!("user.sign_in_count={}", u.sign_in_count);
}

fn tuple_struct() {
    let black = Color(0, 0, 0);
    let origin = Point(0, 0, 0);
    println!("black color {} {} {}", black.0, black.1, black.2);
    println!("origin {} {} {}", origin.0, origin.1, origin.2);

    let sub = AlwaysEqual; // unused warning
}

#[derive(Debug)]
struct Rectangle {
    width: u32,
    height: u32,
}

// this resembles Python oop style to me
impl Rectangle {
    // &self is short for self: &Self, and Self is an alias for Rectangle
    fn area(&self) -> u32 {
        self.width * self.height
    }

    fn scale(&mut self, ratio: u32) -> &Self {
        self.width *= ratio;
        self.height *= ratio;
        self
    }

    fn scale0(&mut self, ratio: u32) {
        self.width *= ratio;
        self.height *= ratio;
    }

    fn can_hold(&self, other: &Rectangle) -> bool {
        self.width > other.width && self.height > other.height
    }
}

impl Rectangle {
    fn square(size: u32) -> Rectangle {
        Rectangle {
            width: size,
            height: size,
        }
    }
}

// From the book
// the main benefit of using methods instead of functions, in addition to
// using method syntax and not having to repeat the type of self in every
// method's signature, is for organization. We put all the things we can do
// with a n instance of a type in one impl block rather than making future
// users of our code search for capabilities of Rectangle in various places
// in the library we provide

fn test_rect() {
    let rect1 = Rectangle {
        width: 30, 
        height: 50,
    };
    println!("rect1 is {:?}", rect1);
    println!("rect1 is {:#?}", rect1);

    let scale = 2;
    let rect2 = Rectangle {
        width: dbg!(30 * scale),
        height: 50,
    };
    dbg!(&rect2); // dbg!(rect2) would take ownership of rect2
    println!("rect2 is {:?}", rect2);
    println!("rect2 area is {}", rect2.area());

    let mut rect3 = Rectangle {
        width: 2, 
        height: 3,
    };
    rect3.scale0(3);
    println!("rect3 scaled up to {:?}", rect3);
    rect3.scale(4);
    println!("rect3 scaled up to {:?}", rect3);

    println!("rect3 holds rect2 ?{}", rect3.can_hold(&rect2));

    let sq1 = Rectangle::square(3);
    println!("sq1 is {:?}", &sq1);
}
