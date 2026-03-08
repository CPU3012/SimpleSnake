#include <raylib.h>
#include <vector>
#include <algorithm>
#include <cstdlib> 
#include <ctime>
#include <cmath>
#include <string>
#include <memory>

#include <iostream>

#include "playing_state.hpp"
#include "game.hpp"
#include "utilities/utilites.hpp"


void preventOppositeDirection(int& newDirection, int oldDirection) {
    if (oldDirection == newDirection + 180 || oldDirection == newDirection - 180) {
        newDirection = oldDirection; // Revert to old direction if the new direction is opposite
    }
}
GameState::StateRequest PlayingState::getInput(GameContext& context) {

    int oldDirection = context.snake.m_direction;

    
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
            preventOppositeDirection(newDirection, context.snake.m_tileDirectionofTravel);

            context.snake.m_direction = newDirection;
            break;
        }
    }

    if (IsKeyPressed(KEY_ESCAPE) && (GetTime() - timeCreated) > 0.5) {
        return GameState::StateRequest::pushPauseMenu;
    }
    return GameState::StateRequest::none;
    
}


GameState::StateRequest PlayingState::update(GameContext& context) {

    double frameTime = GetFrameTime();
    static int bodyPartColourCounter = 0;

    context.snake.oldSnakePosition = context.snake.m_headPosition;

    switch (context.snake.m_currentDirectionofTravel) {
        case 0:
            context.snake.m_headPosition.y -= (context.snake.speed) * double(frameTime);
            Utilities::headPosOverflow(context.snake);
            break;

        case 90:
            context.snake.m_headPosition.x += (context.snake.speed) * double(frameTime);
            Utilities::headPosOverflow(context.snake);

            break;

        case 180:
            context.snake.m_headPosition.y += (context.snake.speed) * double(frameTime);
            Utilities::headPosOverflow(context.snake);
            break;
            
        case 270:
            context.snake.m_headPosition.x -= (context.snake.speed) * double(frameTime);
            Utilities::headPosOverflow(context.snake);

            break;
    }

    
    context.snake.m_currentDirectionofTravel = context.snake.m_direction;

    if (floor(context.snake.oldSnakePosition.x) != floor(context.snake.m_headPosition.x) || floor(context.snake.oldSnakePosition.y) != floor(context.snake.m_headPosition.y)) {
        
        if(!context.snake.multiColourBody && context.snake.m_length > 0 || context.snake.m_length == 1) {
            context.bodyParts.emplace_back(context.snake.oldSnakePosition, CollisionObject::Body, context.snake.m_bodyColour);

        } else if (context.snake.multiColourBody && context.snake.m_length > 0) {

            if (!context.snake.bodyColours.empty()) {
                int index = context.snake.m_length % (context.snake.bodyColours.size() - 1);
                context.bodyParts.emplace_back(context.snake.oldSnakePosition, CollisionObject::Body, context.snake.bodyColours[index + bodyPartColourCounter]);

                bodyPartColourCounter++;
                if (bodyPartColourCounter >= context.snake.bodyColours.size()) {
                    bodyPartColourCounter = 0;
                }

            } else {
                context.bodyParts.emplace_back(context.snake.oldSnakePosition, CollisionObject::Body, context.snake.m_bodyColour);

            }

        }

        if (context.bodyParts.size() > context.snake.m_length) {
            context.bodyParts.erase(context.bodyParts.begin());
        }

        context.snake.m_tileDirectionofTravel = context.snake.m_currentDirectionofTravel;

        #ifdef PRINT_DEBUG_INFO
            std::cout << debug.getSnakeReport(context.snake) << std::endl;
        #endif
    }

    if (IsWindowResized()) {
        Utilities::calculateSquareDimensions(GetScreenWidth(), GetScreenHeight(), context.squareSize, context.offsetX, context.offsetY);
        Utilities::recalcTiles(context);
    }
    

    PlayingState::handleCollisions(context);
    return GameState::StateRequest::none;
}



bool seenMidpoint(const std::vector<Vector2>& midpoints, const Vector2& point) {
    for (const auto& mp : midpoints) {
        if (mp.x == point.x && mp.y == point.y) {
            return true;
        }
    }
    return false;
}
void PlayingState::draw(GameContext& context) {

    Vector2 playerPostion;
    playerPostion.x = std::clamp(int(context.snake.m_headPosition.x), 0, (NUMBER_OF_TILES - 1));
    playerPostion.y = std::clamp(int(context.snake.m_headPosition.y), 0, (NUMBER_OF_TILES - 1));

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

                int(context.tiles[i][i2].position.x),
                int(context.tiles[i][i2].position.y),

                (context.squareSize / NUMBER_OF_TILES),
                (context.squareSize / NUMBER_OF_TILES),

                tileColour

            );

        }
    }

    // Draw apples
    for (const auto& apple : context.apples) {
    // Clamp apple position to board limits, idk if this is necessary but I did it anyway
    int x = std::clamp(int(apple.position.x), 0, NUMBER_OF_TILES - 1);
    int y = std::clamp(int(apple.position.y), 0, NUMBER_OF_TILES - 1);

    DrawRectangle(
        int(context.tiles[x][y].position.x),
        int(context.tiles[x][y].position.y),
        (context.squareSize / NUMBER_OF_TILES),
        (context.squareSize / NUMBER_OF_TILES),
        apple.colour
    );
}
    // Draw a seamless box between body parts
    if (!context.bodyParts.empty()) {
        for (auto &bp : context.bodyParts) bp.numConnections = 0;

        std::vector<Vector2> seenMidpoints;

        for (auto& part1 : context.bodyParts) { 
            for(auto& part2 : context.bodyParts) {
                int maxConnections1 = 2;
                int maxConnections2 = 2;

                if (&part1 == &context.bodyParts.front() ) {
                    maxConnections1 = 1; // Head can only have one connection
                }
                if (&part2 == &context.bodyParts.front() ) {
                    maxConnections2 = 1; // Head can only have one connection
                }

                if (&part1 == &context.bodyParts.back() ) {
                    maxConnections1 = 1; // Tail can only have one connection
                }
                if (&part2 == &context.bodyParts.back() ) {
                    maxConnections2 = 1; // Tail can only have one connection
                }

                Vector2 midPoint = {
                    (context.tiles[int(part1.position.x)][int(part1.position.y)].position.x + context.tiles[int(part2.position.x)][int(part2.position.y)].position.x) / 2,
                    (context.tiles[int(part1.position.x)][int(part1.position.y)].position.y + context.tiles[int(part2.position.x)][int(part2.position.y)].position.y) / 2
                };

                if (Utilities::isAdjacent(part1.position, part2.position) 
                    && &part1 != &part2
                    && part1.numConnections < maxConnections1 
                    && part2.numConnections < maxConnections2
                    && !seenMidpoint(seenMidpoints, midPoint)
                    ) {

                    part1.numConnections++;
                    part2.numConnections++;

                    Vector2 midPoint = {
                        (context.tiles[int(part1.position.x)][int(part1.position.y)].position.x + context.tiles[int(part2.position.x)][int(part2.position.y)].position.x) / 2,
                        (context.tiles[int(part1.position.x)][int(part1.position.y)].position.y + context.tiles[int(part2.position.x)][int(part2.position.y)].position.y) / 2
                    };
                    seenMidpoints.emplace_back(midPoint);

                    DrawRectangle(
                        midPoint.x + (context.squareSize / NUMBER_OF_TILES) * float(1.0f - bodyPartSizeFactor) / 2.0f,
                        midPoint.y + (context.squareSize / NUMBER_OF_TILES) * float(1.0f - bodyPartSizeFactor) / 2.0f,
                        (context.squareSize / NUMBER_OF_TILES) * bodyPartSizeFactor,
                        (context.squareSize / NUMBER_OF_TILES) * bodyPartSizeFactor,
                        part1.colour
                    );

                }
            }
        }
    }


    // Draw body parts
    for(const auto& part : context.bodyParts) {
        // Clamp body part position to board limits
        int x = std::clamp(int(part.position.x), 0, NUMBER_OF_TILES - 1);
        int y = std::clamp(int(part.position.y), 0, NUMBER_OF_TILES - 1);

        DrawRectangle(
            int(context.tiles[x][y].position.x + (context.squareSize / NUMBER_OF_TILES) * float(1.0f - bodyPartSizeFactor) / 2.0f),
            int(context.tiles[x][y].position.y + (context.squareSize / NUMBER_OF_TILES) * float(1.0f - bodyPartSizeFactor) / 2.0f),
            (context.squareSize / NUMBER_OF_TILES) * bodyPartSizeFactor,
            (context.squareSize / NUMBER_OF_TILES) * bodyPartSizeFactor,
            part.colour
        );
    }


    // Draw a seamless box between head and first body part
    if (!context.bodyParts.empty() && context.bodyParts.size() > 0) {
        if (!(context.snake.m_headPosition.x - context.bodyParts.back().position.x > 1 || context.bodyParts.back().position.x - context.snake.m_headPosition.x > 1 ||
              context.snake.m_headPosition.y - context.bodyParts.back().position.y > 1 || context.bodyParts.back().position.y - context.snake.m_headPosition.y > 1)) {

            Vector2 headToBodyMidPoint = { 
                (context.tiles[int(context.snake.m_headPosition.x)][int(context.snake.m_headPosition.y)].position.x + context.tiles[int(context.bodyParts.back().position.x)][int(context.bodyParts.back().position.y)].position.x) / 2,
                (context.tiles[int(context.snake.m_headPosition.x)][int(context.snake.m_headPosition.y)].position.y + context.tiles[int(context.bodyParts.back().position.x)][int(context.bodyParts.back().position.y)].position.y) / 2
            };
            DrawRectangle(
                headToBodyMidPoint.x + (context.squareSize / NUMBER_OF_TILES) * float(1.0f - bodyPartSizeFactor) / 2.0f,
                headToBodyMidPoint.y + (context.squareSize / NUMBER_OF_TILES) * float(1.0f - bodyPartSizeFactor) / 2.0f,
                (context.squareSize / NUMBER_OF_TILES) * bodyPartSizeFactor,
                (context.squareSize / NUMBER_OF_TILES) * bodyPartSizeFactor,
                context.snake.m_headColour
            );
        }
    }

    // Draw player
    DrawRectangle(

        int(context.tiles[int(context.snake.m_headPosition.x)][int(context.snake.m_headPosition.y)].position.x + (context.squareSize / NUMBER_OF_TILES) * float(1.0f - headSizeFactor) / 2.0f), 
        int(context.tiles[int(context.snake.m_headPosition.x)][int(context.snake.m_headPosition.y)].position.y + (context.squareSize / NUMBER_OF_TILES) * float(1.0f - headSizeFactor) / 2.0f),
        (context.squareSize / NUMBER_OF_TILES) * headSizeFactor,
        (context.squareSize / NUMBER_OF_TILES) * headSizeFactor,

        context.snake.m_headColour

    );
}

bool PlayingState::handleCollisions(GameContext& context) {

    Vector2 playerPostion;
    playerPostion.x = floor(context.snake.m_headPosition.x);
    playerPostion.y = floor(context.snake.m_headPosition.y);

    for (const auto& bomb : context.bombs) {
        if (bomb.isColliding(playerPostion)) {
            // Handle bomb collision, e.g., end game or reduce snake length
            return true; // Collision detected
        }
    }
    for (const auto& bodyPart : context.bodyParts) {
        if (bodyPart.isColliding(playerPostion)) {
            // Handle collision with snake's own body
            return true; // Collision detected
        }
    }


    // Apple collision detection
    for (int i = 0; i < context.apples.size(); ++i) {
        
        if (context.apples[i].isColliding(playerPostion)) {
        
            context.apples.erase(context.apples.begin() + i);
            context.snake.m_length++;
            
            Vector2 pos;
            pos.x = floor(rand() % NUMBER_OF_TILES);
            pos.y = floor(rand() % NUMBER_OF_TILES);

            if (pos.x == playerPostion.x && pos.y == playerPostion.y) {
                // If the new apple spawns on the player's head, try again
                i--;
                continue;
            }

            for (const auto& bp : context.bodyParts) {
                if (bp.isColliding(pos)) {
                    #ifdef PRINT_DEBUG_INFO
                        std::cout << "Apple spawned on body part at (" << pos.x << ", " << pos.y << "). Retrying..." << std::endl;
                    #endif
                    i--;
                    continue;
                }
            }

            context.apples.emplace_back(pos, CollisionObject::Apple, RED);

            #ifdef PRINT_DEBUG_INFO
                std::cout << "Apple "<< i << " at position (" << playerPostion.x << ", " << playerPostion.y << ") eaten. New length: " << context.snake.m_length << std::endl;
            #endif

            return true;
        }
    }

    return false;
}