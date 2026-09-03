#pragma once

#include "game_state.hpp"
#include "ui/playing_ui.hpp"
#include "game_context.hpp"

#define TILE_COLOUR_1 Color(230, 225, 197, 255)
#define TILE_COLOUR_2 Color(212, 203, 146, 255)

class PlayingState : public GameState {
    public:
        GameState::StateRequest getInput(GameContext& context) override;
        GameState::StateRequest update(GameContext& context) override;
        void draw(GameContext& context) override;

        PlayingState();
        bool handleCollisions(GameContext& context);

        void drawTilePattern(Color firstColour, Color secondColour, GameContext& context);
        void updateApples(GameContext& context);
        void updateSnakePosition(GameContext& context, double frameTime);
        void updateSnakeLength(GameContext& context);
        void resizeGame(GameContext& context);
};