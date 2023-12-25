// use std::fs;
use std::io;

// use std::io::BufRead;
// use std::collections::BinaryHeap;
// use std::collections::HashMap;
// use std::collections::HashSet;
use std::collections::VecDeque;

// https://github.com/tsatke/aoc2022/blob/main/src/day5.rs
const INPUT: &str = include_str!("input.txt");

#[derive(Debug)]
struct Move {
    count: usize,
    from: usize,
    to: usize,
}

impl Move {
    // sample line
    // move 1 from 7 to 4
    fn from_line(line: &str) -> Self {
        let mut split = line.split(|c| c == ' ');
        let count = split.nth(1).unwrap().parse::<usize>().unwrap();
        let from = split.nth(1).unwrap().parse::<usize>().unwrap();
        let to = split.nth(1).unwrap().parse::<usize>().unwrap();
        Self { count, from, to }
    }
}

fn a() -> io::Result<()> {
    // let file = fs::File::open("src/input.txt")?;
    // let reader = io::BufReader::new(file);
    // let mut total_score = 0;

    let mut stacks: Vec<VecDeque<char>> = vec![VecDeque::new(); 9];
    //                 [M]     [W] [M]    
    //             [L] [Q] [S] [C] [R]    
    //             [Q] [F] [F] [T] [N] [S]
    //     [N]     [V] [V] [H] [L] [J] [D]
    //     [D] [D] [W] [P] [G] [R] [D] [F]
    // [T] [T] [M] [G] [G] [Q] [N] [W] [L]
    // [Z] [H] [F] [J] [D] [Z] [S] [H] [Q]
    // [B] [V] [B] [T] [W] [V] [Z] [Z] [M]
    //  1   2   3   4   5   6   7   8   9 
    for line in INPUT.lines().take(8) {
        if !line.is_empty() {
            let mut elements = [None; 9];
            for (i, block) in (0..line.len()).step_by(4).enumerate() {
                let c = line.as_bytes()[block+1] as char;
                if c == ' ' {
                    elements[i] = None
                } else { 
                    elements[i] = Some(c)
                };
            }
            for (i, elm) in elements
                .into_iter()
                .enumerate()
                .filter(|(_, elm)| elm.is_some())
            {
                stacks[i].push_front(elm.unwrap());
            }
        } else {
            break;
        }
    }

    // for specific input
    for line in INPUT.lines().skip(10) {
        let m = Move::from_line(line);
        for _ in 0..m.count {
            let elem = stacks[m.from - 1].pop_back().unwrap();
            stacks[m.to - 1].push_back(elem);
        }
    }

    let mut res = String::with_capacity(stacks.len());
    stacks
        .iter()
        .for_each(|s| res.push(*s.back().unwrap()));

    println!("Res: {:?}", res);
    
    Ok(())
}


fn b() -> io::Result<()> {
    let mut stacks: Vec<VecDeque<char>> = vec![VecDeque::new(); 9];
    for line in INPUT.lines().take(8) {
        if !line.is_empty() {
            let mut elements = [None; 9];
            for (i, block) in (0..line.len()).step_by(4).enumerate() {
                let c = line.as_bytes()[block+1] as char;
                if c == ' ' {
                    elements[i] = None
                } else { 
                    elements[i] = Some(c)
                };
            }
            for (i, elm) in elements
                .into_iter()
                .enumerate()
                .filter(|(_, elm)| elm.is_some())
            {
                stacks[i].push_front(elm.unwrap());
            }
        } else {
            break;
        }
    }

    for line in INPUT.lines().skip(10) {
        let m = Move::from_line(line);
        let mut elems: VecDeque<char> = VecDeque::with_capacity(m.count);
        for _ in 0..m.count {
            let elem = stacks[m.from - 1].pop_back().unwrap();
            elems.push_front(elem);
        }
        elems
            .into_iter()
            .for_each(|c| stacks[m.to - 1].push_back(c));
    }

    let mut res = String::with_capacity(stacks.len());
    stacks
        .iter()
        .for_each(|s| res.push(*s.back().unwrap()));

    println!("Res: {:?}", res);
    
    Ok(())
}


fn main() -> io::Result<()> {
    a()?;
    b()?;

    Ok(())
}