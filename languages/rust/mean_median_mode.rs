// read from understanding-rust-loops article online
// Since `into_iter` consumes the ownership for each item passed in, then the
// collection of values from the range will not be available after the loop.
// assigning it to a variable and trying to use it later won't work
//
// when looping over a collection, you have a few different ways to choose
// ownership of the items used.
//
// * iter() - iterates over &T (a borrowed reference to the item)
// * iter_mut() - iterates over &mut T (an editable borrowed reference to the item)
// * into_iter() - iterates over T (takes/consumes ownership)

fn main() {
    let mut list = vec![10, 14, 10, 12, 9, -2, 14, 10, 14];
    let mean = get_mean(&list);
    let median = get_median(&mut list); // mutability for sorting
    let mode = get_mode(&list);
    println!("mean={:?}, median={:?}, mode={:?}", mean, median, mode);

    let (mean, median, mode) = get_list_stats(&mut list);
    println!("mean={:?}, median={:?}, mode={:?}", mean, median, mode);
    println!("original list: {:?}", list);
}

// exercise 1 from Chapter 8
// given a list of integers, use a vector and return the 
// mean(the average value)
// median(when sorted, the value in the middle position) and 
// mode(the value that occurs most often; a hash map will be helpful here) 
// of the list
fn get_mean(list: &Vec<i32>) -> Option<f64> {
    if list.is_empty() {
        None
    } else {
        let mut sum: f64 = 0.0;
        // why does this work too?
        // for &i in list {
        //     sum += f64::from(i); // why can't I use *i?
        // }
        for i in list {
            sum += f64::from(*i); // why can't I use i?
        }
        Some(sum/list.len() as f64)
    }
}

fn get_median(list: &mut Vec<i32>) -> Option<f64> {
    if list.is_empty() {
        None
    } else {
        list.sort();
        let len = list.len();
        if len % 2 == 0 {
            let a = list[len/2];
            let b = list[len/2+1];
            println!("{:?}", list);
            Some((a+b) as f64/2.0)
        } else {
            Some(list[len/2] as f64)
        }
    }
}

fn get_mode(list: &Vec<i32>) -> Vec<i32> {
    if list.is_empty() {
        Vec::new()
    } else {
        use std::collections::HashMap;
        let mut counter = HashMap::new();
        for i in list {
            let count = counter.entry(i).or_insert(0);
            *count += 1;
        }

        let mut max_count = 0;
        for (_, count) in &counter {
            if *count > max_count {
                max_count = *count;
            }
        }
        let mut mode: Vec<i32> = Vec::new();
        for (i, count) in &counter {
            if *count == max_count {
                mode.push(**i);
            }
        }
        mode
    }
}

fn get_list_stats(list: &mut Vec<i32>) -> (Option<f64>, Option<f64>, Vec<i32>) {
    if list.is_empty() {
        (None, None, Vec::new())
    } else {
        list.sort();

        use std::collections::HashMap;

        let mut sum: f64 = 0.0;
        let mut counter = HashMap::new();
        for &mut i in &mut *list { // I don't understand &mut *list
            sum += f64::from(i);
            let count = counter.entry(i).or_insert(0);
            *count += 1;
        }
        // for i in &*list {
        //     sum += f64::from(*i);
        //     let count = counter.entry(*i).or_insert(0);
        //     *count += 1;
        // }
        
        // for &mut i in list { // why does this move list? isn't a reference?
        // }

        let len = list.len();
        let median = if len % 2 == 0 {
            (list[len/2] + list[len/2+1]) as f64/2.0
        } else {
            list[len/2] as f64
        };

        let mut max_count = 0;
        for (_, count) in &counter {
            if *count > max_count {
                max_count = *count;
            }
        }
        let mut mode: Vec<i32> = Vec::new();
        for (i, count) in &counter {
            if *count == max_count {
                mode.push(*i);
            }
        }

        // println!("{:?}", list);
        (Some(sum/len as f64), Some(median), mode)
    }
}
