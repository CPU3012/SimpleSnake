#pragma once

#include <raylib.h>

#include "utilities/utilites.hpp"

class GameContext;

class UI {
    public:
        virtual ~UI() = default;

        virtual void draw(GameContext& context) = 0;

        Utilities::Vector2<int> applyMargin(Utilities::Vector2<int> position, int width, int height);
    
        int uiMargin = 0; // UI margin in pixels
        int defaultFontSize = 35;

        double uiMarginRatio = 0; // UI margin as a ratio of the square size
};