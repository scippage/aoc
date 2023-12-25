// use std::fs;
use std::io;

// use std::io::BufRead;
// use std::collections::BinaryHeap;
// use std::collections::HashMap;
// use std::collections::HashSet;
use std::collections::VecDeque;

use counter::Counter;

const INPUT: &str = include_str!("input.txt");

fn a() -> io::Result<()> {
    let mut res = 0;
    let mut queue = VecDeque::with_capacity(4);
    let mut counter: Counter<char> = Counter::new();
    let mut uniques = 0;
    const QUEUE_LEN: usize = 4usize;
    for l in INPUT.lines().take(1) {
        for (idx, c) in l.chars().enumerate() {
            if queue.len() == QUEUE_LEN {
                let popped = queue.pop_front().unwrap();
                counter[&popped] -= 1;
                if counter[&popped] == 0 {
                    uniques -= 1;
                }
            }
            counter[&c] += 1;
            if counter[&c] == 1 {
                uniques += 1;
            }
            queue.push_back(c);

            if uniques == QUEUE_LEN {
                res = idx+1;
                break;
            }
        }
    }
    println!("Res: {:?}", res);
    
    Ok(())
}


fn b() -> io::Result<()> {
    let mut res = 0;
    let mut queue = VecDeque::with_capacity(4);
    let mut counter: Counter<char> = Counter::new();
    let mut uniques = 0;
    const QUEUE_LEN: usize = 14usize;
    for l in INPUT.lines().take(1) {
        for (idx, c) in l.chars().enumerate() {
            if queue.len() == QUEUE_LEN {
                let popped = queue.pop_front().unwrap();
                counter[&popped] -= 1;
                if counter[&popped] == 0 {
                    uniques -= 1;
                }
            }
            counter[&c] += 1;
            if counter[&c] == 1 {
                uniques += 1;
            }
            queue.push_back(c);

            if uniques == QUEUE_LEN {
                res = idx+1;
                break;
            }
        }
    }
    println!("Res: {:?}", res);
    
    Ok(())
}


fn main() -> io::Result<()> {
    a()?;
    b()?;

    Ok(())
}