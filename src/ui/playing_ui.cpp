#include "playing_ui.hpp"
#include "game_context.hpp"
#include "ui/ui.hpp"


PlayingUI::PlayingUI() {
    uiMargin = 10;
}

void PlayingUI::draw(GameContext& context) {
    int squareLeftWall = context.offsetX;
    int squareTopWall = context.offsetY;

    Utilities::Vector2<int> scoreTextLocation = {squareLeftWall, squareTopWall};
    scoreTextLocation = applyMargin(scoreTextLocation, context.squareSize, context.squareSize);

    DrawText(
        ("Score: " + std::to_string(context.snake.m_length)).c_str(),
        scoreTextLocation.x,
        scoreTextLocation.y,
        defaultFontSize,
        WHITE
    );
}