#include "states/game_over_state.hpp"
#include "ui/gameover_ui.hpp"


GameState::StateRequest GameOverState::getInput(GameContext& context) {

    if (IsKeyDown(KEY_ESCAPE) && (GetTime() - timeCreated) > 0.5) {
        // Do the same thing as the play again button
    }
    return GameState::StateRequest::none; 
}

void GameOverState::draw(GameContext& context) {
    return; //Nothing really to draw here, as it is done in UI

    
}

GameState::StateRequest GameOverState::update(GameContext& context){
    double frameTime = GetFrameTime();

    if (IsWindowResized()) {
        Utilities::calculateSquareDimensions(context.squareSize, context.offsetX, context.offsetY);
        Utilities::recalcTiles(context);
    }

    return GameState::StateRequest::none; 
}