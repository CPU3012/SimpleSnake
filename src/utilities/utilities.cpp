#include "utilites.hpp"
#include <cmath>

void Utilities::recalcTiles(GameContext& context) {

    for(int i = 0; i < NUMBER_OF_TILES; i++) {
        for(int i2 = 0; i2 < NUMBER_OF_TILES; i2++) {

        context.tiles[i][i2].position.x = double(context.squareSize / NUMBER_OF_TILES * i) + context.offsetX;
        context.tiles[i][i2].position.y = double(context.squareSize / NUMBER_OF_TILES * i2) + context.offsetY;

        }
    }
}


void Utilities::headPosOverflow(Snake& snake) {

    if(snake.m_headPosition.x > (NUMBER_OF_TILES - 1)) {
        snake.m_headPosition.x -= NUMBER_OF_TILES;
    }
    if(snake.m_headPosition.y > (NUMBER_OF_TILES - 1)) {
        snake.m_headPosition.y -= NUMBER_OF_TILES;
    }

    if(snake.m_headPosition.x < 0) {
        snake.m_headPosition.x += NUMBER_OF_TILES;
    }
    if(snake.m_headPosition.y < 0) {
        snake.m_headPosition.y += NUMBER_OF_TILES;
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



bool Utilities::isAdjacent(const Vector2& pos1, const Vector2& pos2) {
    int dx = std::abs(floor(pos1.x) - floor(pos2.x));
    int dy = std::abs(floor(pos1.y) - floor(pos2.y));
    return (dx + dy) == 1;
}