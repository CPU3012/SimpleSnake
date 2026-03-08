#include "game.hpp"

#include <raylib.h>
#include <vector>
#include <algorithm>
#include <cstdlib> 
#include <ctime>
#include <cmath>
#include <string>
#include <memory>

#include <iostream>

#define PRINT_DEBUG_INFO

#ifdef PRINT_DEBUG_INFO
    DebugInfo debug;
#endif

std::string DebugInfo::getSnakeReport(Snake& snake){
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
    snake.m_bodyColour = SKYBLUE;
    snake.m_headColour = BLUE;
    snake.m_length = 0;
    snake.speed = 4.0;
    snake.multiColourBody = true;

    snake.m_direction = 90; // 0 = up, 90 = right, 180 = down, 270 = left
    snake.m_currentDirectionofTravel = 90;

    snake.m_headPosition.x = 0; snake.m_headPosition.y = 0;
    snake.oldSnakePosition.x = 0; snake.oldSnakePosition.y = 0;

    snake.bodyColours.clear();
    snake.bodyColours = {
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
    Tile tiles[NUMBER_OF_TILES][NUMBER_OF_TILES];
    
    calculateSquareDimensions(m_squareSize, m_offsetX, m_offsetY);
    recalcTiles(tiles);

    // Initialize body parts
    bodyParts.clear();


    // Spawn apples at random positions
    apples.clear();
    int numApples = 3; // Set how many apples you want initially
    for (int i = 0; i < numApples; ++i) {
        Vector2 pos;
        pos.x = floor(rand() % NUMBER_OF_TILES);
        pos.y = floor(rand() % NUMBER_OF_TILES);
        apples.emplace_back(pos, CollisionObject::Apple, RED);
    }

    // placeholder
    stateStack.insert(stateStack.begin(), std::make_unique<PlayingState>());

    // ------------------------------------------------------------------------- Main game loop -----------------------------------------------------------------
    while (!WindowShouldClose()) {

        // Get input and update the top state
        stateStack.front()->getInput(snake);
        stateStack.front()->update(snake, tiles, *this);

        // Iterate through all states and draw them, drawing the bottom states first and the top states last so that they render on top of each other correctly
        for (auto state = stateStack.rbegin(); state != stateStack.rend(); ++state) {
            (*state)->draw(snake, tiles, *this);
        }

    }

    
}

void PlayingState::update(Snake& snake, Tile (&tiles)[NUMBER_OF_TILES][NUMBER_OF_TILES], Game& game) {

    double frameTime = GetFrameTime();
    game.m_screenWidth = GetScreenWidth();
    game.m_screenHeight = GetScreenHeight();

    static int bodyPartColourCounter = 0;

    snake.oldSnakePosition = snake.m_headPosition;

    switch (snake.m_currentDirectionofTravel) {
        case 0:
            snake.m_headPosition.y -= (snake.speed) * double(frameTime);
            game.headPosOverflow(snake);

            break;

        case 90:
            snake.m_headPosition.x += (snake.speed) * double(frameTime);
            game.headPosOverflow(snake);

            break;

        case 180:
            snake.m_headPosition.y += (snake.speed) * double(frameTime);
            game.headPosOverflow(snake);

            break;
            
        case 270:
            snake.m_headPosition.x -= (snake.speed) * double(frameTime);
            game.headPosOverflow(snake);

            break;
    }

    
    snake.m_currentDirectionofTravel = snake.m_direction;

    if (floor(snake.oldSnakePosition.x) != floor(snake.m_headPosition.x) || floor(snake.oldSnakePosition.y) != floor(snake.m_headPosition.y)) {
        
        if(!snake.multiColourBody && snake.m_length > 0 || snake.m_length == 1) {
            game.bodyParts.emplace_back(snake.oldSnakePosition, CollisionObject::Body, snake.m_bodyColour);

        } else if (snake.multiColourBody && snake.m_length > 0) {

            if (!snake.bodyColours.empty()) {
                int index = snake.m_length % (snake.bodyColours.size() - 1);
                game.bodyParts.emplace_back(snake.oldSnakePosition, CollisionObject::Body, snake.bodyColours[index + bodyPartColourCounter]);

                bodyPartColourCounter++;
                if (bodyPartColourCounter >= snake.bodyColours.size()) {
                    bodyPartColourCounter = 0;
                }

            } else {
                game.bodyParts.emplace_back(snake.oldSnakePosition, CollisionObject::Body, snake.m_bodyColour);

            }

        }

        if (game.bodyParts.size() > snake.m_length) {
            game.bodyParts.erase(game.bodyParts.begin());
        }

        snake.m_tileDirectionofTravel = snake.m_currentDirectionofTravel;

        #ifdef PRINT_DEBUG_INFO
            std::cout << debug.getSnakeReport(snake) << std::endl;
        #endif
    }

    if (IsWindowResized()) {
        game.calculateSquareDimensions(game.m_squareSize, game.m_offsetX, game.m_offsetY);
        game.recalcTiles(tiles);
    }
    
    game.handleCollisions(snake, tiles);
}

bool seenMidpoint(const std::vector<Vector2>& midpoints, const Vector2& point) {
    for (const auto& mp : midpoints) {
        if (mp.x == point.x && mp.y == point.y) {
            return true;
        }
    }
    return false;
}
void PlayingState::draw(Snake& snake, Tile (&tiles)[NUMBER_OF_TILES][NUMBER_OF_TILES], Game& game) {

    BeginDrawing();
    ClearBackground(BLACK);


    Vector2 playerPostion;
    playerPostion.x = std::clamp(int(snake.m_headPosition.x), 0, (NUMBER_OF_TILES - 1));
    playerPostion.y = std::clamp(int(snake.m_headPosition.y), 0, (NUMBER_OF_TILES - 1));

    float bodyPartSizeFactor = 0.8f;
    float headSizeFactor = 0.9f;

    // Draw tile pattern
    Color tileColour;
    for(int i = 0; i < NUMBER_OF_TILES; i++){
        for(int i2 = 0; i2 < NUMBER_OF_TILES; i2++){

            if (((i + 1) + (i2 + 1)) % 2 == 0)
            {
                tileColour = BEIGE;
            } else {
                tileColour = BROWN;
            }
            
            DrawRectangle(

                int(tiles[i][i2].position.x),
                int(tiles[i][i2].position.y),

                (game.m_squareSize / NUMBER_OF_TILES),
                (game.m_squareSize / NUMBER_OF_TILES),

                tileColour

            );

        }
    }

    // Draw apples
    for (const auto& apple : game.apples) {
    // Clamp apple position to board limits, idk if this is necessary but I did it anyway
    int x = std::clamp(int(apple.position.x), 0, NUMBER_OF_TILES - 1);
    int y = std::clamp(int(apple.position.y), 0, NUMBER_OF_TILES - 1);

    DrawRectangle(
        int(tiles[x][y].position.x),
        int(tiles[x][y].position.y),
        (game.m_squareSize / NUMBER_OF_TILES),
        (game.m_squareSize / NUMBER_OF_TILES),
        apple.colour
    );
}
    // Draw a seamless box between body parts
    if (!game.bodyParts.empty()) {
        for (auto &bp : game.bodyParts) bp.numConnections = 0;

        std::vector<Vector2> seenMidpoints;

        for (auto& part1 : game.bodyParts) { 
            for(auto& part2 : game.bodyParts) {
                int maxConnections1 = 2;
                int maxConnections2 = 2;

                if (&part1 == &game.bodyParts.front() ) {
                    maxConnections1 = 1; // Head can only have one connection
                }
                if (&part2 == &game.bodyParts.front() ) {
                    maxConnections2 = 1; // Head can only have one connection
                }

                if (&part1 == &game.bodyParts.back() ) {
                    maxConnections1 = 1; // Tail can only have one connection
                }
                if (&part2 == &game.bodyParts.back() ) {
                    maxConnections2 = 1; // Tail can only have one connection
                }


                if (game.isAdjacent(part1.position, part2.position) 
                    && &part1 != &part2
                    && part1.numConnections < maxConnections1 
                    && part2.numConnections < maxConnections2
                    && !seenMidpoint(seenMidpoints, {
                        (tiles[int(part1.position.x)][int(part1.position.y)].position.x + tiles[int(part2.position.x)][int(part2.position.y)].position.x) / 2,
                        (tiles[int(part1.position.x)][int(part1.position.y)].position.y + tiles[int(part2.position.x)][int(part2.position.y)].position.y) / 2
                    })
                    ) {

                    part1.numConnections++;
                    part2.numConnections++;

                    Vector2 midPoint = {
                        (tiles[int(part1.position.x)][int(part1.position.y)].position.x + tiles[int(part2.position.x)][int(part2.position.y)].position.x) / 2,
                        (tiles[int(part1.position.x)][int(part1.position.y)].position.y + tiles[int(part2.position.x)][int(part2.position.y)].position.y) / 2
                    };
                    seenMidpoints.emplace_back(midPoint);

                    DrawRectangle(
                        midPoint.x + (game.m_squareSize / NUMBER_OF_TILES) * float(1.0f - bodyPartSizeFactor) / 2.0f,
                        midPoint.y + (game.m_squareSize / NUMBER_OF_TILES) * float(1.0f - bodyPartSizeFactor) / 2.0f,
                        (game.m_squareSize / NUMBER_OF_TILES) * bodyPartSizeFactor,
                        (game.m_squareSize / NUMBER_OF_TILES) * bodyPartSizeFactor,
                        part1.colour
                    );

                }
            }
        }
    }


    // Draw body parts
    for(const auto& part : game.bodyParts) {
        // Clamp body part position to board limits
        int x = std::clamp(int(part.position.x), 0, NUMBER_OF_TILES - 1);
        int y = std::clamp(int(part.position.y), 0, NUMBER_OF_TILES - 1);

        DrawRectangle(
            int(tiles[x][y].position.x + (game.m_squareSize / NUMBER_OF_TILES) * float(1.0f - bodyPartSizeFactor) / 2.0f),
            int(tiles[x][y].position.y + (game.m_squareSize / NUMBER_OF_TILES) * float(1.0f - bodyPartSizeFactor) / 2.0f),
            (game.m_squareSize / NUMBER_OF_TILES) * bodyPartSizeFactor,
            (game.m_squareSize / NUMBER_OF_TILES) * bodyPartSizeFactor,
            part.colour
        );
    }


    // Draw a seamless box between head and first body part
    if (!game.bodyParts.empty() && game.bodyParts.size() > 0) {
        if (!(snake.m_headPosition.x - game.bodyParts.back().position.x > 1 || game.bodyParts.back().position.x - snake.m_headPosition.x > 1 ||
              snake.m_headPosition.y - game.bodyParts.back().position.y > 1 || game.bodyParts.back().position.y - snake.m_headPosition.y > 1)) {

            Vector2 headToBodyMidPoint = { 
                (tiles[int(snake.m_headPosition.x)][int(snake.m_headPosition.y)].position.x + tiles[int(game.bodyParts.back().position.x)][int(game.bodyParts.back().position.y)].position.x) / 2,
                (tiles[int(snake.m_headPosition.x)][int(snake.m_headPosition.y)].position.y + tiles[int(game.bodyParts.back().position.x)][int(game.bodyParts.back().position.y)].position.y) / 2
            };
            DrawRectangle(
                headToBodyMidPoint.x + (game.m_squareSize / NUMBER_OF_TILES) * float(1.0f - bodyPartSizeFactor) / 2.0f,
                headToBodyMidPoint.y + (game.m_squareSize / NUMBER_OF_TILES) * float(1.0f - bodyPartSizeFactor) / 2.0f,
                (game.m_squareSize / NUMBER_OF_TILES) * bodyPartSizeFactor,
                (game.m_squareSize / NUMBER_OF_TILES) * bodyPartSizeFactor,
                snake.m_headColour
            );
        }
    }

    // Draw player
    DrawRectangle(

        int(tiles[int(playerPostion.x)][int(playerPostion.y)].position.x + (game.m_squareSize / NUMBER_OF_TILES) * float(1.0f - headSizeFactor) / 2.0f), 
        int(tiles[int(playerPostion.x)][int(playerPostion.y)].position.y + (game.m_squareSize / NUMBER_OF_TILES) * float(1.0f - headSizeFactor) / 2.0f),
        (game.m_squareSize / NUMBER_OF_TILES) * headSizeFactor,
        (game.m_squareSize / NUMBER_OF_TILES) * headSizeFactor,

        snake.m_headColour

    );
    EndDrawing();
}


void preventOppositeDirection(int& newDirection, int oldDirection) {
    if (oldDirection == newDirection + 180 || oldDirection == newDirection - 180) {
        newDirection = oldDirection; // Revert to old direction if the new direction is opposite
    }
}

void PlayingState::getInput(Snake& snake) {

    int oldDirection = snake.m_direction;

    
    struct KeyDir { int key; int dir; };
    static const KeyDir keyDirs[] = {
        { KEY_W, 0 },
        { KEY_D, 90 },
        { KEY_S, 180 },
        { KEY_A, 270 }
    };

    for (const auto& kd : keyDirs) {
        if (IsKeyDown(kd.key)) {
            int newDirection = kd.dir;
            preventOppositeDirection(newDirection, snake.m_tileDirectionofTravel);

            snake.m_direction = newDirection;
            break;
        }
    }
    
}

void Game::headPosOverflow(Snake& snake) {

    if(snake.m_headPosition.x > (NUMBER_OF_TILES - 1)) {
        snake.m_headPosition.x -= NUMBER_OF_TILES;
    }
    if(snake.m_headPosition.y > (NUMBER_OF_TILES - 1)) {
        snake.m_headPosition.y -= NUMBER_OF_TILES;
    }

    if(snake.m_headPosition.x < 0) {
        snake.m_headPosition.x += NUMBER_OF_TILES;
    }
    if(snake.m_headPosition.y < 0) {
        snake.m_headPosition.y += NUMBER_OF_TILES;
    }

}

void Game::recalcTiles(Tile (&tiles)[NUMBER_OF_TILES][NUMBER_OF_TILES]) {

    for(int i = 0; i < NUMBER_OF_TILES; i++) {
        for(int i2 = 0; i2 < NUMBER_OF_TILES; i2++) {

        tiles[i][i2].position.x = double(m_squareSize / NUMBER_OF_TILES * i) + m_offsetX;
        tiles[i][i2].position.y = double(m_squareSize / NUMBER_OF_TILES * i2) + m_offsetY;

        }
    }
}

// Calculate and store the dimensions of a perfect square that fits inside the window
void Game::calculateSquareDimensions(int& squareSize, int& offsetX, int& offsetY) {

    // Determine the size of the square (smallest dimension)
    squareSize = std::min(m_screenWidth, m_screenHeight);

    // Calculate offsets to center the square in the window
    offsetX = (m_screenWidth - squareSize) / 2;
    offsetY = (m_screenHeight - squareSize) / 2;
}

bool Game::handleCollisions(Snake& snake, Tile (&tiles)[NUMBER_OF_TILES][NUMBER_OF_TILES]) {

    Vector2 playerPostion;
    playerPostion.x = floor(snake.m_headPosition.x);
    playerPostion.y = floor(snake.m_headPosition.y);

    for (const auto& bomb : bombs) {
        if (bomb.isColliding(playerPostion)) {
            // Handle bomb collision, e.g., end game or reduce snake length
            return true; // Collision detected
        }
    }
    for (const auto& bodyPart : bodyParts) {
        if (bodyPart.isColliding(playerPostion)) {
            // Handle collision with snake's own body
            return true; // Collision detected
        }
    }


    // Apple collision detection
    for (int i = 0; i < apples.size(); ++i) {
        
        if (apples[i].isColliding(playerPostion)) {
        
            apples.erase(apples.begin() + i);
            snake.m_length++;
            
            //srand((unsigned int)time(nullptr));
            Vector2 pos;
            pos.x = floor(rand() % NUMBER_OF_TILES);
            pos.y = floor(rand() % NUMBER_OF_TILES);

            apples.emplace_back(pos, CollisionObject::Apple, RED);

            #ifdef PRINT_DEBUG_INFO
                std::cout << "Apple "<< i << " at position (" << playerPostion.x << ", " << playerPostion.y << ") eaten. New length: " << snake.m_length << std::endl;
            #endif

            return true;
        }
    }

    return false;
}

bool Game::isAdjacent(const Vector2& pos1, const Vector2& pos2) {
    int dx = std::abs(floor(pos1.x) - floor(pos2.x));
    int dy = std::abs(floor(pos1.y) - floor(pos2.y));
    return (dx + dy) == 1;
}