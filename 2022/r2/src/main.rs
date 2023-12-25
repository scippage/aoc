use std::fs;
use std::io;

use std::io::BufRead;
// use std::collections::BinaryHeap;
// use std::collections::HashMap;

fn a() -> io::Result<()> {
    let file = fs::File::open("src/input.txt")?;
    let reader = io::BufReader::new(file);
    let mut total_score = 0;
    for line in reader.lines() {
        let line = line?;
        let tokens: Vec<&str> = line.split(" ").collect();
        let round_score;
        match tokens[0] {
            "A" => {
                match tokens[1] {
                    "X" => round_score = 1 + 3,
                    "Y" => round_score = 2 + 6,
                    "Z" => round_score = 3 + 0,
                    _ => panic!("unexpected input")
                }
            },
            "B" => {
                match tokens[1] {
                    "X" => round_score = 1 + 0,
                    "Y" => round_score = 2 + 3,
                    "Z" => round_score = 3 + 6,
                    _ => panic!("unexpected input")
                }
            },
            "C" => {
                match tokens[1] {
                    "X" => round_score = 1 + 6,
                    "Y" => round_score = 2 + 0,
                    "Z" => round_score = 3 + 3,
                    _ => panic!("unexpected input")
                }
            },
            _ => panic!("unexpected input")
        };
        // println!("Line: {:?}, Round score: {:?}", line, round_score);
        total_score += round_score;
    }
    println!("Total score: {:?}", total_score);
    
    Ok(())
}


fn b() -> io::Result<()> {
    let file = fs::File::open("src/input.txt")?;
    let reader = io::BufReader::new(file);
    let mut total_score = 0;
    for line in reader.lines() {
        let line = line?;
        let tokens: Vec<&str> = line.split(" ").collect();
        let round_score;
        match tokens[0] {
            "A" => {
                match tokens[1] {
                    "X" => round_score = 3 + 0,
                    "Y" => round_score = 1 + 3,
                    "Z" => round_score = 2 + 6,
                    _ => panic!("unexpected input")
                }
            },
            "B" => {
                match tokens[1] {
                    "X" => round_score = 1 + 0,
                    "Y" => round_score = 2 + 3,
                    "Z" => round_score = 3 + 6,
                    _ => panic!("unexpected input")
                }
            },
            "C" => {
                match tokens[1] {
                    "X" => round_score = 2 + 0,
                    "Y" => round_score = 3 + 3,
                    "Z" => round_score = 1 + 6,
                    _ => panic!("unexpected input")
                }
            },
            _ => panic!("unexpected input")
        };
        // println!("Line: {:?}, Round score: {:?}", line, round_score);
        total_score += round_score;
    }
    println!("Total score: {:?}", total_score);
    
    Ok(())
}


fn main() -> io::Result<()> {
    a()?;
    b()?;

    Ok(())
}