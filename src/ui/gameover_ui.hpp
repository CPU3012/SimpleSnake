#pragma once

#include "ui/ui.hpp"

class GameoverUI : public UI {
    public:
        void draw(GameContext& context) override;

        GameoverUI();
};

#define GAMEOVER_TEXT "Game over"

#define PLAYAGAIN_BUTTON_TEXT "Play again??"