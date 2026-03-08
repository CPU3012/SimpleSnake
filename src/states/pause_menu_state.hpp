#pragma once

#include "game_state.hpp"


class PauseMenuState : public GameState {
    public:
        GameState::StateRequest getInput(GameContext& context) override;
        GameState::StateRequest update(GameContext& context) override;
        void draw(GameContext& context) override;
};