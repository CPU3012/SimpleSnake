#pragma once

#include "ui/ui.hpp"


#define START_BUTTON_INDEX 0
#define ABOUT_BUTTON_INDEX 1

class MenuUI : public UI {
    public:
        void draw(GameContext& context) override;

        MenuUI();
};

#define TITLE_TEXT "Simple Snake"

#define START_BUTTON_TEXT "Start Game"
#define ABOUT_BUTTON_TEXT "About"