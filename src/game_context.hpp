#pragma once

#include <raylib.h>
#include <vector>
#include <string>
#include <memory>

#include "snake.hpp"
#include "tile.hpp"
#include "collision_object.hpp"

struct GameContext {
    Snake snake;
    Tile tiles[NUMBER_OF_TILES][NUMBER_OF_TILES];

    int squareSize = 0;
    int offsetX = 0;   
    int offsetY = 0;

    std::vector<CollisionObject> apples;
    std::vector<CollisionObject> bombs;
    std::vector<CollisionObject> bodyParts;

};