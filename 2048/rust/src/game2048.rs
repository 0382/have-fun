extern crate nix;

use rand::Rng;
use nix::sys::termios;
use std::io;
use std::io::{Read, Write};
use std::cmp::max;

#[derive(Debug)]
pub struct Game2048 {
    status: [i32; 16],
}

#[derive(Debug)]
enum Direction {
    Up,
    Down,
    Left,
    Right,
}

impl Game2048 {
    pub fn new() -> Self {
        let mut game = Game2048{
            status: [0; 16]
        };
        let i = rand::thread_rng().gen_range(0, 16);
        game.status[i] = 2;
        game
    }

    pub fn start(&mut self) {
        println!("This is a 2048 game;");
        println!("use 'w'(up), 's'(down), 'a'(left), 'd'(right) to play.");
        println!("Ready?[Y/n]");
        let choose = getch() as char;
        if choose != 'Y' && choose != 'y' {
            return;
        }
        clean_console();
        self.print_frame();
        while self.step() {
            continue;
        }
    }

    fn print_frame(&self) {
        set_cursor_potition(0, 0);
        let line = "-----------------------------";
        println!("{}", line);
        for i in 0..4 {
            for j in 0..4 {
                print!("| ");
                if self.status[i*4 + j] != 0 {
                    print!("{:4} ", self.status[i*4+j]);
                } else {
                    print!("     ");
                }
            }
            println!("|\n{}", line);
        }
    }

    fn shift(arr: &mut [i32; 4]) {
        let mut temp = [0; 4];
        let mut read_idx = 0;
        let mut write_idx = 0;
        for i in 0..4 {
            if arr[i] != 0 {
                temp[read_idx] = arr[i];
                read_idx += 1;
            }
            arr[i] = 0;
        }
        let mut i = 0;
        while i < read_idx {
            if i < 3 && temp[i] == temp[i+1] {
                arr[write_idx] = temp[i] * 2;
                i += 2;
                write_idx += 1;
            } else {
                arr[write_idx] = temp[i];
                i += 1;
                write_idx += 1;
            }
        }
    }

    fn change_status(&mut self, direction: Direction) {
        match direction {
            Direction::Up => {
                let mut arr = [0; 4];
                for j in 0..4 {
                    for i in 0..4 {
                        arr[i] = self.status[i*4+j];
                    }
                    Self::shift(&mut arr);
                    for i in 0..4 {
                        self.status[i*4+j] = arr[i];
                    }
                }
            },
            Direction::Down => {
                let mut arr = [0; 4];
                for j in 0..4 {
                    for i in 0..4 {
                        arr[3-i] = self.status[i*4+j];
                    }
                    Self::shift(&mut arr);
                    for i in 0..4 {
                        self.status[i*4+j] = arr[3-i];
                    }
                }
            },
            Direction::Left => {
                let mut arr = [0; 4];
                for i in 0..4 {
                    for j in 0..4 {
                        arr[j] = self.status[i*4+j];
                    }
                    Self::shift(&mut arr);
                    for j in 0..4 {
                        self.status[i*4+j] = arr[j];
                    }
                }
            },
            Direction::Right => {
                let mut arr = [0; 4];
                for i in 0..4 {
                    for j in 0..4 {
                        arr[3-j] = self.status[i*4+j];
                    }
                    Self::shift(&mut arr);
                    for j in 0..4 {
                        self.status[i*4+j] = arr[3-j];
                    }
                }
            },
        }
    }

    fn add_new_number(&mut self) {
        let mut zero_counts = 0;
        for i in 0..16 {
            zero_counts += (self.status[i] == 0) as i32;
        }
        if zero_counts == 0 {return;}
        let rand_num = rand::thread_rng().gen_range(0, zero_counts);
        for i in 0..16 {
            zero_counts -= (self.status[i] == 0) as i32;
            if zero_counts == rand_num {
                let mut max_num = self.status[0];
                for j in 0..16 {
                    max_num = max(self.status[j], max_num);
                }
                let temp: i32 = rand::thread_rng().gen();
                match max_num {
                    64 => self.status[i] = if temp%8 > 0 {2} else {4},
                    128 => self.status[i] = if temp%5 > 0 {2} else {4},
                    256 => self.status[i] = if temp%3 > 0 {2} else {4},
                    512 => self.status[i] = if temp%2 > 0 {2} else {4},
                    1024 => self.status[i] = if temp%3 > 1 {2} else {4},
                    _ => self.status[i] = 2,
                }
                break;
            }
        }
    }

    fn step(&mut self) -> bool {
        let ch = getch();
        if ch == 'w' as u8 || ch == 72 || ch == 65 {
            self.change_status(Direction::Up);
        } else if ch == 's' as u8 || ch == 80 || ch == 66 {
            self.change_status(Direction::Down);
        } else if ch == 'a' as u8 || ch == 75 || ch == 68 {
            self.change_status(Direction::Left);
        } else if ch == 'd' as u8 || ch == 77 || ch == 67 {
            self.change_status(Direction::Right);
        } else if ch == 'q' as u8 || ch == 'Q' as u8 {
            println!("You quit!");
            return false;
        } else {
            return true;
        }
        match self.check_status() {
            0 => (),
            1 => {
                self.print_frame();
                println!("You win!!!");
                return false;
            },
            -1 => {
                self.print_frame();
                println!("You fail!");
                return false;
            },
            _ => {
                panic!("invalid status code");
            },
        }
        self.add_new_number();
        self.print_frame();
        return true;
    }

    fn check_status(&self) -> i32 {
        for i in 0..16 {
            if self.status[i] == 2048 {
                return 1;
            }
        }
        for i in 0..16 {
            if self.status[i] == 0 {
                return 0;
            }
        }
        for i in 0..4 {
            for j in 0..3 {
                if self.status[i*4+j] == self.status[i*4+j+1] {
                    return 0;
                }
            }
        }
        for j in 0..4 {
            for i in 0..3 {
                if self.status[i*4+j] == self.status[(i+1)*4+j] {
                    return 0;
                }
            }
        }
        return -1;
    }
}


fn clean_console(){
    std::io::stdout().write("\x1b[2J".as_bytes()).unwrap();
}

fn getch() -> u8 {
    let oldt = termios::tcgetattr(0).unwrap();
    let mut newt = termios::tcgetattr(0).unwrap();
    newt.local_flags.remove(termios::LocalFlags::ICANON);
    newt.local_flags.remove(termios::LocalFlags::ECHO);
    termios::tcsetattr(0, termios::SetArg::TCSANOW, &newt).unwrap();
    let mut buf: [u8; 1] = [0];
    io::stdin().read(&mut buf).expect("failed to getch");
    termios::tcsetattr(0, termios::SetArg::TCSANOW, &oldt).unwrap();
    buf[0]
    // rand::thread_rng().gen_range(65, 69)
}

fn set_cursor_potition(x: i32, y: i32) {
    print!("\x1b[{};{}H", y + 1, x + 1);
}
