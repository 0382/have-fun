pub mod game2048;

use game2048::Game2048;

fn main() {
    let mut game = Game2048::new();
    game.start();
}
