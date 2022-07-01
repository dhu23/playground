fn main() {
    // using mut because it is needed in the original code
    let mut list = vec![10, 14, 10, 12, 9, -2, 14, 10, 14];
    
    let ret1 = loop1(&list);
    let ret2 = loop2(&list);
    // let ret3 = loop3_bad(&mut list);
    let ret4 = loop4(&mut list);
    let ret5 = loop5(&mut list);
    let ret6 = loop6(&mut list);
    let ret7 = loop7(&mut list);

    println!("loop1 ret={:?}", ret1);
    println!("loop2 ret={:?}", ret2);
    // println!("loop3 ret={:?}", ret3);
    println!("loop4 ret={:?}", ret4);
    println!("loop5 ret={:?}", ret5);
    println!("loop6 ret={:?}", ret6);
    println!("loop7 ret={:?}", ret7);
}

fn loop1(list: &Vec<i32>) -> Option<f64> {
    if list.is_empty() {
        None
    } else {
        let mut sum: f64 = 0.0;
        for &i in list {
            sum += f64::from(i);
            // cannot write f64::from(*i)
            // error would be:
            // error[E0614]: type `i32` cannot be dereferenced
            //
            // How should I read the for syntax?
            // Is it because "&i" of &i32 type, therefore i is of "i32" type?
            // or should I treat "&i" a declaration that i is of "&i32" type?
        }
        Some(sum/list.len() as f64)
    }
}

fn loop2(list: &Vec<i32>) -> Option<f64> {
    if list.is_empty() {
        None
    } else {
        let mut sum: f64 = 0.0;
        for i in list {
            sum += f64::from(*i);
            // cannot write f64::from(i)
            // error would be:
            // the trait `From<&i32>` is not implemented for `f64`
            //
            // is i of "&i32" type?
            // is the dereferencing required here?
        }
        Some(sum/list.len() as f64)
    }
}

// This one causes compilation error, but why?
// If `list` is moved by the loop, why didn't this cause issue in loop1()?
//
// Rust ERROR:
//
// error[E0382]: borrow of moved value: `list`
//    --> for_loop_example.rs:65:18
//     |
// 57  | fn loop3(list: &mut Vec<i32>) -> Option<f64> {
//     |          ---- move occurs because `list` has type `&mut Vec<i32>`, which does not implement the `Copy` trait
// ...
// 62  |         for &mut i in list {
//     |                       ----
//     |                       |
//     |                       `list` moved due to this implicit call to `.into_iter()`
//     |                       help: consider borrowing to avoid moving into the for loop: `&list`
// ...
// 65  |         Some(sum/list.len() as f64)
//     |                  ^^^^ value borrowed here after move
//     |
// note: this function takes ownership of the receiver `self`, which moves `list`
// 

// fn loop3_bad(list: &mut Vec<i32>) -> Option<f64> {
//     if list.is_empty() {
//         None
//     } else {
//         let mut sum: f64 = 0.0;
//         for &mut i in list {
//             sum += f64::from(i);
//         }
//         Some(sum/list.len() as f64)
//     }
// }

fn loop4(list: &mut Vec<i32>) -> Option<f64> {
    if list.is_empty() {
        None
    } else {
        let mut sum: f64 = 0.0;
        for i in &*list {
            // what does &*list even do?
            sum += f64::from(*i);
        }
        Some(sum/list.len() as f64)
    }
}

fn loop5(list: &mut Vec<i32>) -> Option<f64> {
    if list.is_empty() {
        None
    } else {
        let mut sum: f64 = 0.0;
        for &i in &*list { // similar to loop4, excpet for using &i
            sum += f64::from(i);
        }
        Some(sum/list.len() as f64)
    }
}

fn loop6(list: &mut Vec<i32>) -> Option<f64> {
    if list.is_empty() {
        None
    } else {
        let mut sum: f64 = 0.0;
        for &mut i in &mut *list { // looking similar to loop5
            sum += f64::from(i);
        }
        Some(sum/list.len() as f64)
    }
}

fn loop7(list: &mut Vec<i32>) -> Option<f64> {
    if list.is_empty() {
        None
    } else {
        let mut sum: f64 = 0.0;
        for i in &mut *list { // looking similar to loop4, except for using mut
            sum += f64::from(*i);
        }
        Some(sum/list.len() as f64)
    }
}
