#include "utilites.hpp"
#include <cmath>

#include "tile.hpp"
#include "game.hpp"


void Utilities::recalcTiles(GameContext& context) {

    for(int i = 0; i < NUMBER_OF_TILES; i++) {
        for(int i2 = 0; i2 < NUMBER_OF_TILES; i2++) {

        context.tiles[i][i2].position.x = double(context.squareSize / NUMBER_OF_TILES * i) + context.offsetX;
        context.tiles[i][i2].position.y = double(context.squareSize / NUMBER_OF_TILES * i2) + context.offsetY;

        }
    }
}


void Utilities::headPosOverflow(Snake& snake) {

    if(snake.headPosition.x > (NUMBER_OF_TILES - 1)) {
        snake.headPosition.x -= NUMBER_OF_TILES;
    }
    if(snake.headPosition.y > (NUMBER_OF_TILES - 1)) {
        snake.headPosition.y -= NUMBER_OF_TILES;
    }

    if(snake.headPosition.x < 0) {
        snake.headPosition.x += NUMBER_OF_TILES;
    }
    if(snake.headPosition.y < 0) {
        snake.headPosition.y += NUMBER_OF_TILES;
    }

}


// Calculate and store the dimensions of a perfect square that fits exactly inside the window
void Utilities::calculateSquareDimensions(int screenWidth, int screenHeight, int& squareSize, int& offsetX, int& offsetY) {

    // Determine the size of the square (smallest dimension)
    squareSize = std::min(screenWidth, screenHeight);

    // Calculate offsets to center the square in the window
    offsetX = (screenWidth - squareSize) / 2;
    offsetY = (screenHeight - squareSize) / 2;
}



bool Utilities::isAdjacent(const ::Vector2& pos1, const ::Vector2& pos2) {
    int x1 = floor(pos1.x), y1 = floor(pos1.y);
    int x2 = floor(pos2.x), y2 = floor(pos2.y);
    return (abs(x1 - x2) + abs(y1 - y2)) == 1;
}


bool Utilities::isOnBody(const ::Vector2& pos, const std::vector<CollisionObject>& bodyParts) {
    Utilities::Vector2<int> intPos = { (int)floor(pos.x), (int)floor(pos.y) };
    for (const auto& bp : bodyParts) {
        if (bp.isColliding(intPos)) {
            return true;
        }
    }
    return false;
}


double Utilities::map(double value, double inputMin, double inputMax, double outputMin, double outputMax) {
    return outputMin + ((value - inputMin)/(inputMax - inputMin) * (outputMax - outputMin));
}

bool Utilities::seenMidpoint(const std::vector<Utilities::Vector2<int>>& midpoints, const Utilities::Vector2<int>& point) {
    for (const auto& mp : midpoints) {
        if (mp.x == point.x && mp.y == point.y) {
            return true;
        }
    }
    return false;
}

bool Utilities::samePositionV(::Vector2 firstVector, ::Vector2 secondVector) {
    firstVector.x = floor(firstVector.x);
    firstVector.y = floor(firstVector.y);

    secondVector.x = floor(secondVector.x);
    secondVector.y = floor(secondVector.y);


    if (firstVector.x != secondVector.x) return false;
    if (firstVector.y != secondVector.y) return false;

    return true;
}

bool Utilities::samePosition(Utilities::Vector2<int> firstVector, Utilities::Vector2<int> secondVector) {

    if (firstVector.x != secondVector.x) return false;
    if (firstVector.y != secondVector.y) return false;

    return true;
}