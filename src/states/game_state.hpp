#pragma once

#include <raylib.h>
#include <vector>
#include <string>
#include <memory>

#include "game_context.hpp"

class GameState {
    public:
        virtual ~GameState() {}

        enum class StateRequest {
            none,
            popMe,
            pushMainMenu,
            pushPauseMenu,
            pushPlaying,
            pushGameOver,
            clearAndPushMainMenu
        };


        double timeCreated = GetTime();

        virtual GameState::StateRequest getInput(GameContext& context) = 0;
        virtual GameState::StateRequest update(GameContext& context) = 0;
        virtual void draw(GameContext& context) = 0;

};