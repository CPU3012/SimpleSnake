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

        class RectangularButton {
            public:

            int width;
            int height;
            int fontSize;

            const char* text;

            Utilities::Vector2<int> position;

            Color textColour;
            Color buttonColour;
            Color BorderColour = GRAY;

            bool isMouseOver();
            bool isClicked();

            void initialise(const char* text, int fontSize, Utilities::Vector2<int> position, Color textColour, Color buttonColour);

            void draw();
        };
        std::vector<RectangularButton> buttons;
};