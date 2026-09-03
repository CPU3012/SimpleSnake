#include "ui/gameover_ui.hpp"
#include "utilities/utilites.hpp"
#include "game.hpp"

#include <string>
#include <raylib.h>


GameoverUI::GameoverUI() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    int buttonFontSize = defaultFontSize;
    int defaultButtonWidth = screenWidth / 4;
    int screenMid = screenWidth / 2;
    int buttonSeparation = screenHeight / 10;

    static const char *playagainButtonText = PLAYAGAIN_BUTTON_TEXT;

    Utilities::Vector2<int> playagainButtonLocation = {screenMid, screenMid + buttonSeparation };


    RectangularButton playagainButton = RectangularButton();
    playagainButton.initialise(
        playagainButtonText, 
        buttonFontSize, 
        playagainButtonLocation, 
        WHITE, 
        BLUE
    );
    buttons.push_back(playagainButton);
}

void GameoverUI::draw(GameContext &context) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    Utilities::Vector2<int> screenMidPoint = { screenWidth / 2, screenHeight / 2 };

    int gameoverTextSize = 80;
    int textWidth = MeasureText(GAMEOVER_TEXT, gameoverTextSize);

    Color backgroundColour = Color(50,50,50,30);
    DrawRectangle(screenMidPoint.x, screenMidPoint.y, (screenWidth/2), (screenHeight/2), backgroundColour);

    for (auto& button : buttons) {
        button.draw();
    }
}