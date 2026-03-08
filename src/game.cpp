#include "game.hpp"

#include "states/game_state.hpp"
#include "states/main_menu_state.hpp"
#include "states/playing_state.hpp"
#include "states/pause_menu_state.hpp"
#include "states/game_over_state.hpp"

#include <raylib.h>
#include <vector>
#include <algorithm>
#include <cstdlib> 
#include <ctime>
#include <cmath>
#include <string>
#include <memory>

#include <iostream>
//#define PRINT_DEBUG_INFO

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
        "Current Direction of Travel: " + std::to_string(snake.m_currentDirectionofTravel) +
        " Direction: " + std::to_string(snake.m_direction) +
        " Head Position: (" + std::to_string(snake.m_headPosition.x) + ", " + std::to_string(snake.m_headPosition.y) + ")" +
        " Score: " + std::to_string(snake.m_length) +
        "\n";
}

Game::Game() {
    init(800, 800); // Initial Window Dimentions
}
Game::~Game(){
    CloseWindow(); 
}

void Game::init(int WINDOW_WIDTH, int WINDOW_HEIGHT){

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Snake Game");
    SetExitKey(KEY_NULL);

    // Initialize snake properties
    context.snake.m_bodyColour = SKYBLUE;
    context.snake.m_headColour = BLUE;
    context.snake.m_length = 0;
    context.snake.speed = 4.0;
    context.snake.multiColourBody = true;

    context.snake.m_direction = 90; // 0 = up, 90 = right, 180 = down, 270 = left
    context.snake.m_currentDirectionofTravel = 90;

    context.snake.m_headPosition.x = 0; context.snake.m_headPosition.y = 0;
    context.snake.oldSnakePosition.x = 0; context.snake.oldSnakePosition.y = 0;

    context.snake.bodyColours.clear();
    context.snake.bodyColours = {
        Color{ 102, 191, 255, 255 },
        Color{ 109, 163, 230, 255 }
    };

    // Initialize random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    //SetTargetFPS(180);
}

void Game::play() {

    m_screenWidth = GetScreenWidth();
    m_screenHeight = GetScreenHeight();
    srand((unsigned int)time(nullptr));

    
    Utilities::calculateSquareDimensions(m_screenWidth, m_screenHeight, context.squareSize, context.offsetX, context.offsetY);
    Utilities::recalcTiles(context);

    context.bodyParts.clear();


    // Spawn apples at random positions
    context.apples.clear();
    int numApples = 3; // Set how many apples you want initially
    for (int i = 0; i < numApples; ++i) {
        Vector2 pos;
        pos.x = floor(rand() % NUMBER_OF_TILES);
        pos.y = floor(rand() % NUMBER_OF_TILES);
        context.apples.emplace_back(pos, CollisionObject::Apple, RED);
    }

    // placeholder
    stateStack.insert(stateStack.begin(), std::make_unique<PlayingState>());


    
    // ------------------------------------------------------------------------- Main game loop -----------------------------------------------------------------
    while (!WindowShouldClose()) {
        // Update global game properties that states may need
        globalUpdate(context.snake, context.tiles);

        // Get input and update the top state
        GameState::StateRequest request = stateStack.front()->getInput(context);

        if (request == GameState::StateRequest::none) request = stateStack.front()->update(context);

        processRequest(request);

        // Iterate through all states and draw them, drawing the bottom states first and the top states last so that they render on top of each other correctly
        BeginDrawing();
        ClearBackground(BLACK);
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

        /*case GameState::StateRequest::pushMainMenu:
            stateStack.insert(stateStack.begin(), std::make_unique<MainMenuState>());
            break; */

        case GameState::StateRequest::pushPlaying:
            stateStack.insert(stateStack.begin(), std::make_unique<PlayingState>());
            break;

        case GameState::StateRequest::pushPauseMenu:
            stateStack.insert(stateStack.begin(), std::make_unique<PauseMenuState>());
            break;

        /*case GameState::StateRequest::pushGameOver:
            stateStack.insert(stateStack.begin(), std::make_unique<GameOverState>());
            break; 

        case GameState::StateRequest::clearAndPushMainMenu:
            stateStack.clear();
            stateStack.insert(stateStack.begin(), std::make_unique<MainMenuState>());
            break; */

        default:
            break;
    }
}

void Game::globalUpdate(Snake& snake, Tile (&tiles)[NUMBER_OF_TILES][NUMBER_OF_TILES]) {
    Utilities::recalcTiles(context);
    m_screenWidth = GetScreenWidth();
    m_screenHeight = GetScreenHeight();
}