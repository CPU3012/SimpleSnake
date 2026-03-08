#include "game.hpp"

int main() {
    Game game;
    game.play();
}

/*
Notes:

The head position is moved by float values, but is always rounded down before rendering. This means 
the player will only visually move a full tile at a time. 

The collision detection function returns as soon as a collision is detected. Which may lead to overlapping objects colliding with weired priorities.

States should not directly  modify the stack, states can return requests (like pop me) which game can process


Todo:
    - UI
    - Body collision (low priority)
    - Wall collision (low priority)
    - Bombs (low priority)
*/