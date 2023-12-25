use std::fs;
use std::io;

use std::io::BufRead;
// use std::collections::BinaryHeap;
use std::collections::HashMap;
use std::collections::HashSet;

fn a(score_map: &HashMap<&char, u32>) -> io::Result<()> {
    let file = fs::File::open("src/input.txt")?;
    let reader = io::BufReader::new(file);
    let mut total_score = 0;
    for line in reader.lines() {
        let line: String = line?;
        let tokens: (&str, &str) = line.split_at(line.chars().count()/2);

        let bag_1: HashSet<char> = tokens.0.chars().collect();
        let bag_2: HashSet<char> = tokens.1.chars().collect();
        let dup: Vec<_> = bag_1.intersection(&bag_2).collect();
        assert!(dup.len() == 1);

        total_score += score_map.get(dup[0]).unwrap();
        // println!("Total score: {:?}, Duplicate: {:?}", total_score, dup[0]);        
    }
    println!("Total score: {:?}", total_score);
    
    Ok(())
}


fn b(score_map: &HashMap<&char, u32>) -> io::Result<()> {
    let file = fs::File::open("src/input.txt")?;
    let reader = io::BufReader::new(file);
    let mut total_score = 0;
    let mut line_group: Vec<String> = vec![];
    for (n, line) in reader.lines().enumerate() {
        let line: String = line?;
        line_group.push(line);

        if n % 3 == 2 {
            let bags: Vec<HashSet<char>> = line_group.into_iter().map(|x| x.chars().collect()).collect();
            let bag: HashSet<char> = bags[0].clone();
            let dup: HashSet<char> = bags.iter().skip(1).fold(bag, |acc, x| acc.intersection(&x).cloned().collect());
            assert!(dup.len() == 1);
            let dup_vec: Vec<char> = dup.into_iter().collect();
            total_score += score_map.get(&dup_vec[0]).unwrap();
            println!("Total score: {:?}, Duplicate: {:?}", total_score, dup_vec);
            line_group = vec![];
        }
    }
    println!("Total score: {:?}", total_score);
    
    Ok(())
}


fn main() -> io::Result<()> {
    let alph: Vec<char> = ('a'..='z').chain('A'..='Z').collect();
    let scores: Vec<u32> = (1..=52).collect();
    let score_map: HashMap<_, _> = alph.iter().zip(scores).collect();

    a(&score_map)?;
    b(&score_map)?;

    Ok(())
}