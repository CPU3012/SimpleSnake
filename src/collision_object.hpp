#pragma once

#include <raylib.h>
#include <vector>
#include <cmath>

#include "utilities/utilites.hpp"

struct CollisionObject
{
    enum Type {
        Head, // currently unused
        Body,
        Apple,
        Bomb,
        Tile
    };

    Vector2 position;
    Type type;
    Color colour;
    int numConnections = 0;

    CollisionObject(Vector2 pos, Type t, Color c)
        : position(pos), type(t), colour(c) {}

    
    bool isColliding(const Utilities::Vector2<int>& otherPos) const {
        return floor(position.x) == otherPos.x && floor(position.y) == otherPos.y;
    }
};