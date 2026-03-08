#pragma once

#include <raylib.h>
#include <vector>
#include <string>
#include <memory>

#include "snake.hpp"
#include "states/game_state.hpp"
#include "utilities/utilites.hpp"
#include "game_context.hpp"
#include "tile.hpp"
#include "collision_object.hpp"

struct CollisionObject;
class GameState;

class DebugInfo{
    public:
        std::string getSnakeReport(Snake& snake);
};

class Game{
    
    public:
    
        Game();
        ~Game();

        void init(int WINDOW_WIDTH, int WINDOW_HEIGHT);
        void play();

        void processRequest(GameState::StateRequest request);
        void globalUpdate(Snake& snake, Tile (&tiles)[NUMBER_OF_TILES][NUMBER_OF_TILES]);

        bool handleCollisions(Snake& snake, Tile (&tiles)[NUMBER_OF_TILES][NUMBER_OF_TILES]);

        Vector2 getMaxSquareSize();


        std::vector<std::unique_ptr<GameState>> stateStack;

        int m_screenWidth = GetScreenWidth();
        int m_screenHeight = GetScreenHeight();

        GameContext context;

    };

