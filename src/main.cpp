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

The loop retries when an apple tries to spawn on an occupied tile, but it doesn't check if the new apple spawns has any tiles that are free which could cause the game to hang if all tiles are occupied.

Todo:
    - UI
    - Body collision (low priority)
    - Wall collision (low priority)
    - Bombs (low priority)
*/