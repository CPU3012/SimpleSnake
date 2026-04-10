#include "game.hpp"

int main() {
    Game game;
    game.play();
}

/*
Notes:

The head position is moved by float values, but is always rounded down before rendering. This means the player will only visually move a full tile at a time. 

The collision detection function returns as soon as a collision is detected. Which may lead to overlapping objects colliding with weired priorities. Though there are no plans to have objects overlap.

The loop retries when an apple tries to spawn on an occupied tile, but it doesn't check if there are any free tiles which could cause the game to hang if all tiles are occupied.

Todo:
    - UI
    - Body collision (low priority)
    - Wall collision (low priority)
    - Bombs (low priority)
    - Textures
*/