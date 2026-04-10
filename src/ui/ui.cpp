#include "ui/ui.hpp"
#include <raylib.h>

#include "utilities/utilites.hpp"

Utilities::Vector2<int> UI::applyMargin(Utilities::Vector2<int> position, int width, int height) {
    Utilities::Vector2<int> newPosition;

    if (position.y > height / 2) {
        newPosition.y = position.y - uiMargin;

    } else if (position.y < height / 2) {
        newPosition.y = position.y + uiMargin;

    } else {
        newPosition.y = position.y;
    }

    if (position.x > width / 2) {
        newPosition.x = position.x - uiMargin;

    } else if (position.x < width / 2) {
        newPosition.x = position.x + uiMargin;

    } else {
        newPosition.x = position.x;
    }

    return newPosition;
}