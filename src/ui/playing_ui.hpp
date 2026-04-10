#pragma once

#include "ui/ui.hpp"

class GameContext;

class PlayingUI : public UI {
    public:
        void draw(GameContext& context) override;

        PlayingUI();
};