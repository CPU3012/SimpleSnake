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
        Utilities::calculateSquareDimensions(GetScreenWidth(), GetScreenHeight(), context.squareSize, context.offsetX, context.offsetY);
        Utilities::recalcTiles(context);
    }

    return GameState::StateRequest::none; 
}

void PauseMenuState::draw(GameContext& context) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    DrawText("PAUSED", screenWidth / 2 - MeasureText("PAUSED", 80) / 2, screenHeight / 2 - 20, 80, WHITE);
    
} 