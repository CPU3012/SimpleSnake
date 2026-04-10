#include "states/main_menu_state.hpp"
#include "game.hpp"
#include "ui/menu_ui.hpp"


MainMenuState::MainMenuState() {
    ui = std::make_unique<MenuUI>();
}


GameState::StateRequest MainMenuState::getInput(GameContext& context) {
    return GameState::StateRequest::none;
}


GameState::StateRequest MainMenuState::update(GameContext& context) {
    if (IsWindowResized()) {
        Utilities::calculateSquareDimensions(context.squareSize, context.offsetX, context.offsetY);
        Utilities::recalcTiles(context);
    }

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int screenMid = screenWidth / 2;

    // Update button positions in case of screen resizing
    if (!ui->buttons.empty() || IsWindowResized()) {
        ui->buttons[0].position.y = screenHeight / 2;

        for (auto& button : ui->buttons) {
            button.position.x = screenMid - (button.width / 2);
        }
    } 

    for (auto& button : ui->buttons) {
        if (button.isMouseOver()) {
            button.BorderColour = YELLOW;
        } else {
            button.BorderColour = GRAY;
        }
    }



    if (!ui->buttons.empty() && ui->buttons[START_BUTTON_INDEX].isClicked()) {
        // Should probably also play a sound effect here
        return GameState::StateRequest::clearAndPushPlaying;
    }
    return GameState::StateRequest::none;
}


void MainMenuState::draw(GameContext& context) {
    ui->draw(context);
}