#include "ui/menu_ui.hpp"
#include "utilities/utilites.hpp"
#include "game.hpp"

#include <string>
#include <raylib.h>



MenuUI::MenuUI() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    static const char* startButtonText = START_BUTTON_TEXT;

    int buttonFontSize = defaultFontSize;
    int defaultButtonWidth = screenWidth / 4;
  
    int screenMid = screenWidth / 2;

    int buttonSeparation = screenHeight / 10;

    Utilities::Vector2<int> startButtonLocation = {(MeasureText(startButtonText, buttonFontSize) / 2), screenHeight / 2 };
    Utilities::Vector2<int> aboutButtonLocation = {screenMid, screenMid + buttonSeparation };
    Utilities::Vector2<int> settingsButtonLocation = {screenMid, screenMid + (buttonSeparation * 2) };
    Utilities::Vector2<int> exitButtonLocation = {screenMid, screenMid + (buttonSeparation * 3) };
   

    RectangularButton startButton = RectangularButton();
    startButton.initialise(
        startButtonText, 
        buttonFontSize, 
        startButtonLocation, 
        WHITE, 
        BLUE
    );
    buttons.push_back(startButton);


    RectangularButton aboutButton = RectangularButton();
    aboutButton.initialise(
        ABOUT_BUTTON_TEXT, 
        buttonFontSize, 
        aboutButtonLocation, 
        WHITE, 
        BLUE
    );
    buttons.push_back(aboutButton);
}

void MenuUI::draw(GameContext& context) {
    ClearBackground(SKYBLUE);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    static const char* titleText = TITLE_TEXT;
    static const char* startButtonText = START_BUTTON_TEXT;

    int titleFontSize = defaultFontSize * 2;
    int titleSpacing = 15;

    int buttonFontSize = defaultFontSize;
    int defaultButtonWidth = screenWidth / 4;
  

    int screenMidX = screenWidth / 2;
    int screenMidY = screenHeight / 2;

    int buttonSeparation = screenHeight / 15;

    Utilities::Vector2<int> titleLocation = {screenMidX, screenHeight / 4 };

    Utilities::Vector2<int> startButtonLocation = {(MeasureText(startButtonText, buttonFontSize) / 2), screenHeight / 2 };
    Utilities::Vector2<int> aboutButtonLocation = {screenMidX, screenMidY + buttonSeparation };
    Utilities::Vector2<int> settingsButtonLocation = {screenMidX, screenMidY + (buttonSeparation * 2) };
    Utilities::Vector2<int> exitButtonLocation = {screenMidX, screenMidY + (buttonSeparation * 3) };

    int titleOffsetX = MeasureTextEx(GetFontDefault(), titleText, titleFontSize, titleSpacing).x / 2;
    DrawTextEx(GetFontDefault(), titleText, Vector2{(float)titleLocation.x - titleOffsetX + 5, (float)titleLocation.y + 5}, titleFontSize, titleSpacing, GRAY);
    DrawTextEx(GetFontDefault(), titleText, Vector2{(float)titleLocation.x - titleOffsetX, (float)titleLocation.y}, titleFontSize, titleSpacing, WHITE);

  

    for (auto& button : buttons) {
        button.draw();
    }

}