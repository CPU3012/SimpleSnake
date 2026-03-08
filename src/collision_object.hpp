#pragma once

#include <raylib.h>
#include <vector>

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

    
    bool isColliding(const Vector2& otherPos) const {
        return (int)this->position.x == (int)otherPos.x && (int)this->position.y == (int)otherPos.y;
    }
};