#pragma once

#include <vector>
#include "snake.hpp"
#include "game.hpp"


namespace Utilities {

    bool seenMidpoint(const std::vector<Vector2>& midpoints, const Vector2& point);

    void calculateSquareDimensions(int screenWidth, int screenHeight, int& squareSize, int& offsetX, int& offsetY);

    void recalcTiles(GameContext& context);

    bool isAdjacent(const Vector2& pos1, const Vector2& pos2);

    void headPosOverflow(Snake& snake); 
}