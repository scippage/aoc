use std::fs;
use std::io;

use std::io::BufRead;
use std::collections::BinaryHeap;

fn a() -> io::Result<()> {
    let file = fs::File::open("src/input.txt")?;
    let reader = io::BufReader::new(file);
    let mut current = 0;
    let mut max = 0;
    for line in reader.lines() {
        let line = line?;
        if line.is_empty() {
            if current > max {
                max = current;
            }
            current = 0;
            continue;
        }
        match line.parse::<i32>() {
            Ok(n) => {
                current += n;
            },
            Err(e) => {
                println!("Error: {}", e);
                continue;
            }
        };
    }
    println!("Max: {}", max);
    Ok(())
}


fn b() -> io::Result<()> {
    let file = fs::File::open("src/input.txt")?;
    let reader = io::BufReader::new(file);
    let mut heap = BinaryHeap::new();
    let mut current = 0;
    for line in reader.lines() {
        let line = line?;
        if line.is_empty() {
            heap.push(current);
            current = 0;
            continue;
        }
        match line.parse::<i32>() {
            Ok(n) => {
                current += n;
            },
            Err(e) => {
                println!("Error: {}", e);
                continue;
            }
        };
    }
    let mut total = 0;
    for i in 0..3 {
        let tmp = heap.pop().expect("Heap is empty");
        total += tmp;
        println!("Elf: {}", tmp);
    }
    println!("Total: {}", total);
    Ok(())
}


fn main() -> io::Result<()> {
    a()?;
    b()?;

    Ok(())
}