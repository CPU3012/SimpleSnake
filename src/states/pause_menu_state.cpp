#include "pause_menu_state.hpp"
#include "utilities/utilites.hpp"

PauseMenuState::StateRequest PauseMenuState::getInput(GameContext& context) {

    if (IsKeyDown(KEY_ESCAPE) && (GetTime() - timeCreated) > 0.5) {
        return GameState::StateRequest::popMe;
    }
    return GameState::StateRequest::none; 
}

PauseMenuState::StateRequest PauseMenuState::update(GameContext& context) {

    double frameTime = GetFrameTime();

    if (IsWindowResized()) {
        Utilities::calculateSquareDimensions(context.squareSize, context.offsetX, context.offsetY);
        Utilities::recalcTiles(context);
    }

    return GameState::StateRequest::none; 
}

void PauseMenuState::draw(GameContext& context) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    Utilities::Vector2<int> screenMidPoint = { screenWidth / 2, screenHeight / 2 };

    int pausedTextSize = 80;
    int textWidth = MeasureText("PAUSED", pausedTextSize);

    Utilities::Vector2<int> pauseTextBackgroundSize = { (int)floor(textWidth * 1.2), (int)floor(pausedTextSize * 1.3) };
    
    DrawRectangle(
        screenMidPoint.x - (pauseTextBackgroundSize.x / 2),
        screenMidPoint.y - (pauseTextBackgroundSize.y / 2.5),
        pauseTextBackgroundSize.x,
        pauseTextBackgroundSize.y*1.1f,
        Fade(BLACK, (float)0.5)
    );

    DrawText("PAUSED", (screenMidPoint.x - textWidth / 2) + 4, (screenMidPoint.y - 20) + 4, pausedTextSize, GRAY);
    DrawText("PAUSED", screenMidPoint.x - textWidth / 2, screenMidPoint.y - 20, pausedTextSize, WHITE);
    
} 