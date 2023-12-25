use std::cmp;
use std::fmt;
use std::io;
use std::cell::RefCell;
use std::rc::Rc;

// use std::io::BufRead;
// use std::collections::BinaryHeap;
use std::collections::HashMap;
// use std::collections::HashSet;
// use std::collections::VecDeque;

// use counter::Counter;

#[derive(Debug)]
pub struct File {
    name: String,
    size: u64,
}

#[derive(Debug)]
pub struct Dir {
    name: String,
}

#[derive(Debug)]
enum NodeValue {
    File(File),
    Dir(Dir),
}

struct Node {
    value: NodeValue,
    children: HashMap<String, Rc<RefCell<Node>>>,
    parent: Option<Rc<RefCell<Node>>>,
}

impl fmt::Debug for Node {
  fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
      f.debug_struct("Node")
          .field("value", &self.value)
          .field("children", &self.children)
          .finish()
  }
}

impl Node {
  pub fn new(dir: Dir) -> Node {
      Node {
          value: NodeValue::Dir(dir),
          children: HashMap::new(),
          parent: None,
      }
  }

  fn append_node<'a>(&mut self, name: String, node: Rc<RefCell<Node>>) {
      self.children.insert(name.clone(), node);
  }

  fn size(&self) -> u64 {
      match &self.value {
          NodeValue::File(file) => file.size,
          NodeValue::Dir(_) => {
              let mut size = 0;
              for child in self.children.values() {
                  size += child.borrow().size();
              }
              size
          }
      }
  }
}

// A command is either "cd <>" or "ls".
enum Command {
  Cd(String),
  Ls,
}

enum Line {
  Command(Command),
  FileWithSize(String, u64),
  Dir(String),
}

fn parse_line(line: &str) -> Line {
  if line.starts_with("$") {
      let command = line[2..].trim();

      if command.starts_with("cd") {
          let dir = command[2..].trim();
          Line::Command(Command::Cd(dir.to_string()))
      } else if command == "ls" {
          Line::Command(Command::Ls)
      } else {
          panic!("Unknown command: {}", command);
      }
  } else {
      if line.starts_with("dir") {
          let dir_name = line[4..].trim();
          Line::Dir(dir_name.to_string())
      } else {
          let mut parts = line.split_whitespace();
          let size: u64 = parts.next().unwrap().to_string().parse::<u64>().unwrap();
          let name = parts.next().unwrap().to_string();
          Line::FileWithSize(name, size)
      }
  }
}

fn a() -> io::Result<()> {
    let input = include_str!("input.txt");

    let root = Rc::new(RefCell::new(Node::new(Dir {
        name: "/".to_string(),
    })));

    let x: () = root.borrow_mut();
    // println!("{:?}", root.borrow_mut());
    
    let mut current_dir = Rc::clone(&root);
    
    let lines = input.lines().skip(1).map(parse_line);
    
    for line in lines {
        match line {
            // For a file, we create a new file struct and add it to the current directory's
            // children.
            Line::FileWithSize(filename, size) => {
                let child_node = Rc::new(RefCell::new(Node {
                    value: NodeValue::File(File {
                        name: filename.to_string(),
                        size,
                    }),
                    children: HashMap::new(),
                    parent: None,
                }));
    
                current_dir
                    .borrow_mut()
                    .append_node(filename, Rc::clone(&child_node));
    
                child_node.borrow_mut().parent = Some(Rc::clone(&current_dir));
            }
    
            // For a directory, we create a new directory struct, set its parent
            // the current directory, and add it to the current directory's
            // children.
            Line::Dir(dir_name) => {
                let child_node = Rc::new(RefCell::new(Node::new(Dir {
                    name: dir_name.to_string(),
                })));
    
                current_dir
                    .borrow_mut()
                    .append_node(dir_name, Rc::clone(&child_node));
    
                let mut mut_child = child_node.borrow_mut();
                mut_child.parent = Some(Rc::clone(&current_dir));
            }
    
            // "ls" is kind of not very useful, so we just ignore it.
            Line::Command(Command::Ls) => continue,
    
            // For "cd", we find the directory in the current directory's children
            // and make it the current directory. If the directory is "/", we go
            // back to the root. If the directory is "..", we go to the parent
            // of the current directory.
            Line::Command(Command::Cd(dir_name)) => match dir_name.as_str() {
                "/" => {
                    current_dir = Rc::clone(&root);
                }
                ".." => {
                    let current_dir_clone = Rc::clone(&current_dir);
                    current_dir = Rc::clone(current_dir_clone.borrow().parent.as_ref().unwrap());
                }
                _ => {
                    let child_clone = Rc::clone(&current_dir.borrow().children[&dir_name]);
                    current_dir = child_clone;
                }
            },
        }
    }

    let res = calc_size_a(&root.borrow());
    println!("{:?}", res);

    let target_size = root.borrow().size() - 40000000;
    println!("{:?}", target_size);

    let res2 = calc_size_b(&root.borrow(), target_size, root.borrow().size());
    println!("{:?}", res2);

    Ok(())
}


fn calc_size_a(node: &Node) -> u64 {
  let mut total = 0;
  let size = node.size();

  if size <= 100000 {
      total += size;
  }

  for child in node.children.values() {
      match child.borrow().value {
          NodeValue::File(_) => continue,
          NodeValue::Dir(_) => {
              total += calc_size_a(&child.borrow());
          }
      }
  }

  return total;
}

fn calc_size_b(node: &Node, target_size: u64, mut best_size: u64) -> u64 {
  let size = node.size();

  if size > target_size {
    best_size = cmp::min(size, best_size);
  }

  for child in node.children.values() {
      match child.borrow().value {
          NodeValue::File(_) => continue,
          NodeValue::Dir(_) => {
              best_size = calc_size_b(&child.borrow(), target_size, best_size);
          }
      }
  }

  return best_size;
}

fn b() -> io::Result<()> {
    Ok(())
}

fn main() -> io::Result<()> {
    a()?;
    b()?;

    Ok(())
}