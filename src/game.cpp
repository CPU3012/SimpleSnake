#include "game.hpp"

#include "states/game_state.hpp"
#include "states/main_menu_state.hpp"
#include "states/playing_state.hpp"
#include "states/pause_menu_state.hpp"
#include "states/game_over_state.hpp"
#include "configuration.hpp"

#include <raylib.h>
#include <vector>
#include <algorithm>
#include <cstdlib> 
#include <ctime>
#include <cmath>
#include <string>
#include <memory>

#include <iostream>

#ifdef PRINT_DEBUG_INFO
    DebugInfo debug;
    
#else
  
#endif


std::string DebugInfo::getSnakeReport(Snake& snake){
    static double lastReportTime = 0.0;
    if (GetTime() - lastReportTime < 1) {
        return ""; 
    }
    lastReportTime = GetTime();

    return  
        "Current Direction of Travel: " + std::to_string(snake.currentDirectionofTravel) +
        "Anticipated Direction: " + std::to_string(snake.anticipatedDirection) +
        " Head Position: (" + std::to_string(snake.headPosition.x) + ", " + std::to_string(snake.headPosition.y) + ")" +
        " Score: " + std::to_string(snake.m_length) +
        "\n";
}

Game::Game() {
    init(700, 700); // Initial Window Dimentions
}
Game::~Game(){
    CloseWindow(); 
}

void Game::init(int WINDOW_WIDTH, int WINDOW_HEIGHT){

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Simple Snake");

    SetExitKey(KEY_NULL);

    windowIcon = LoadImage("assets/icons/app_icon.png");
    SetWindowIcon(windowIcon);
    UnloadImage(windowIcon);

    if (FPS_LIMIT > 0) {
        SetTargetFPS(FPS_LIMIT);
    }

    // Initialize snake properties, should probably be moved
    //context.snake.m_bodyColour = SKYBLUE;
    context.snake.m_bodyColour = Color{ 109, 163, 230, 255 };
    context.snake.m_headColour = BLUE;
    context.snake.m_length = 0;
    context.snake.speed = 4.0f;
    context.snake.multiColourBody = false;

    // 0 = up, 90 = right, 180 = down, 270 = left
    context.snake.currentDirectionofTravel = 90;
    context.snake.m_tileDirectionofTravel = 90;
    context.snake.anticipatedDirection = 90;

    context.snake.headPosition.x = 0; context.snake.headPosition.y = 0;
    context.snake.oldSnakePosition.x = 0; context.snake.oldSnakePosition.y = 0;

    context.snake.bodyColours.clear();
    context.snake.bodyColours = {
        Color{ 102, 191, 255, 255 },
        Color{ 109, 163, 230, 255 }
    };

    // Initialize random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    context.bodyParts.clear();
}

void Game::play() {

    m_screenWidth = GetScreenWidth();
    m_screenHeight = GetScreenHeight();
    srand((unsigned int)time(nullptr));

    
    Utilities::calculateSquareDimensions(context.squareSize, context.offsetX, context.offsetY);
    Utilities::recalcTiles(context);


    stateStack.insert(stateStack.begin(), std::make_unique<MainMenuState>());

    // Main game loop
    while (!WindowShouldClose()) {
        GameState::StateRequest request = stateStack.front()->getInput(context);

        if (request == GameState::StateRequest::none) request = stateStack.front()->update(context);

        processRequest(request);

        BeginDrawing();
        ClearBackground(GRAY);
        for (auto state = stateStack.rbegin(); state != stateStack.rend(); ++state) {
            (*state)->draw(context);
        }
        EndDrawing();

    }

    
}






void Game::processRequest(GameState::StateRequest request) {
    if (request == GameState::StateRequest::none) {
        return;
    }

    switch (request) {
        case GameState::StateRequest::popMe:
            if (!stateStack.empty()) {
                stateStack.erase(stateStack.begin());
            }
            break;

        case GameState::StateRequest::pushMainMenu:
            stateStack.insert(stateStack.begin(), std::make_unique<MainMenuState>());
            break; 

        case GameState::StateRequest::pushPlaying:
            stateStack.insert(stateStack.begin(), std::make_unique<PlayingState>());
            break;

        case GameState::StateRequest::pushPauseMenu:
            stateStack.insert(stateStack.begin(), std::make_unique<PauseMenuState>());
            break;

        case GameState::StateRequest::pushGameOver:
            stateStack.insert(stateStack.begin(), std::make_unique<GameOverState>());
            break; 

        /*case GameState::StateRequest::clearAndPushMainMenu:
            stateStack.clear();
            stateStack.insert(stateStack.begin(), std::make_unique<MainMenuState>());
            break; */

        case GameState::StateRequest::clearAndPushPlaying:
            stateStack.clear();
            stateStack.insert(stateStack.begin(), std::make_unique<PlayingState>());
            break;

        default:
            break;
    }
}