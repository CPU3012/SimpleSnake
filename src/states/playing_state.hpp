#pragma once

#include "game_state.hpp"
#include "ui/playing_ui.hpp"

class PlayingState : public GameState {
    public:
        GameState::StateRequest getInput(GameContext& context) override;
        GameState::StateRequest update(GameContext& context) override;
        void draw(GameContext& context) override;

        PlayingState();
        bool handleCollisions(GameContext& context);
};