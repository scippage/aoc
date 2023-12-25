use std::fs;
use std::io;

use std::io::BufRead;
// use std::collections::BinaryHeap;
// use std::collections::HashMap;
// use std::collections::HashSet;

fn a() -> io::Result<()> {
    let file = fs::File::open("src/input.txt")?;
    let reader = io::BufReader::new(file);
    let mut total_score = 0;
    for line in reader.lines() {
        let line: String = line?;
        let tokens = line.split(",");
        let ranges: Vec<Vec<u32>> = tokens.map(|token| {
            token.split("-").map(|id| {
                id.parse().unwrap()
            }).collect::<Vec<u32>>()
        }).collect();

        if ranges[0][0] >= ranges[1][0] && ranges[0][1] <= ranges[1][1] || 
            ranges[0][0] <= ranges[1][0] && ranges[0][1] >= ranges[1][1]
        {
            total_score += 1;
        }
        // println!("Total score: {:?}, ranges: {:?}", total_score, ranges);        
    }
    println!("Total score: {:?}", total_score);
    
    Ok(())
}


fn b() -> io::Result<()> {
    let file = fs::File::open("src/input.txt")?;
    let reader = io::BufReader::new(file);
    let mut total_score = 0;
    for line in reader.lines() {
        let line: String = line?;
        let tokens = line.split(",");
        let ranges: Vec<Vec<u32>> = tokens.map(|token| {
            token.split("-").map(|id| {
                id.parse().unwrap()
            }).collect::<Vec<u32>>()
        }).collect();

        if !(ranges[1][0] > ranges[0][1] || ranges[1][1] < ranges[0][0])
        {
            total_score += 1;
        }
        // println!("Total score: {:?}, ranges: {:?}", total_score, ranges);        
    }
    println!("Total score: {:?}", total_score);
    
    Ok(())
}


fn main() -> io::Result<()> {
    a()?;
    b()?;

    Ok(())
}