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

bool UI::RectangularButton::isMouseOver() {
    Vector2 mousePos = GetMousePosition();

    int x = mousePos.x;
    int y = mousePos.y;

    int minX = position.x;
    int minY = position.y;

    int maxX = position.x + width;
    int maxY = position.y + height;

    if (minX <= x && x <= maxX && minY <= y && y <= maxY) {
        return true;
    }

    return false;
}


bool UI::RectangularButton::isClicked() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isMouseOver()) {
        return true;
    }

    return false;
}

void UI::RectangularButton::draw() {
    DrawRectangle(position.x, position.y, width, height, BorderColour);

    int borderThickness = (float)height * 0.1;
    DrawRectangle(
        position.x + borderThickness, 
        position.y + borderThickness, 
        width - (2 * borderThickness), 
        height - (2 * borderThickness), 
        buttonColour
    );

    int textWidth = MeasureText(text, fontSize);
    int textX = position.x + (width - textWidth) / 2;
    int textY = position.y + (height - fontSize) / 2;

    DrawText(text, textX, textY, fontSize, textColour);
}

void UI::RectangularButton::initialise(const char* text, int fontSize, Utilities::Vector2<int> position, Color textColour, Color buttonColour) {
    this->text = text;
    this->fontSize = fontSize;
    this->position = position;
    this->textColour = textColour;
    this->buttonColour = buttonColour;
    
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 0);
    this->width = textSize.x * 1.3;
    this->height = textSize.y * 1.3;
}