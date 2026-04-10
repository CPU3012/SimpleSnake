#pragma once

#include <vector>
#include <raylib.h>

class GameContext;
class Snake;
class CollisionObject;

namespace Utilities {
    template <typename T>
    class Vector2 {
        public:
            T x;
            T y;
    };

    bool seenMidpoint(const std::vector<Utilities::Vector2<int>>& midpoints, const Utilities::Vector2<int>& point);

    void calculateSquareDimensions(int& squareSize, int& offsetX, int& offsetY);

    void recalcTiles(GameContext& context);

    bool isAdjacent(const ::Vector2& pos1, const ::Vector2& pos2);

    void headPosOverflow(Snake& snake);

    bool isOnBody(const ::Vector2& pos, const std::vector<CollisionObject>& bodyParts);

    double map(double value, double inputMin, double inputMax, double outputMin, double outputMax);

    bool samePositionV(::Vector2 firstVector, ::Vector2 secondVector); // Raylib Vector2 for compatability

    bool samePosition(Utilities::Vector2<int> firstVector, Utilities::Vector2<int> secondVector);
}