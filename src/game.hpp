#pragma once

#include <raylib.h>
#include <vector>
#include <string>
#include <memory>

#define NUMBER_OF_TILES 16

class Tile;
struct bodyPart;
struct CollisionObject;
class Snake;
class GameState;

class DebugInfo{
    public:
        std::string getSnakeReport(Snake& snake);
};

class Snake{
 
    public:

        double speed; //The rate of change of distance in tiles per second

        int m_length;
        int m_direction;
        int m_currentDirectionofTravel;
        int m_tileDirectionofTravel; // Updates only when the snake moves a full tile

        bool multiColourBody;

        Color m_headColour;
        Color m_bodyColour;

        Vector2 m_headPosition;
        Vector2 oldSnakePosition;
        
        std::vector<bodyPart> m_body;
        std::vector<Color> bodyColours;
        
        
};

class Game{
    
    public:
    
        Game();
        ~Game();

        void init(int WINDOW_WIDTH, int WINDOW_HEIGHT);
        void play();
        //void update(Snake& snake, Tile (&tiles)[NUMBER_OF_TILES][NUMBER_OF_TILES]);
        //void draw(Snake& snake, Tile (&tiles)[NUMBER_OF_TILES][NUMBER_OF_TILES]);
        //void getInput(Snake& snake);
        void headPosOverflow(Snake& snake); 
        void recalcTiles(Tile (&tiles)[NUMBER_OF_TILES][NUMBER_OF_TILES]);
        void calculateSquareDimensions(int& squareSize, int& offsetX, int& offsetY);
        bool isAdjacent(const Vector2& pos1, const Vector2& pos2);
        
        bool handleCollisions(Snake& snake, Tile (&tiles)[NUMBER_OF_TILES][NUMBER_OF_TILES]);

        Vector2 getMaxSquareSize();
        int getScore() const { return snake.m_length; }

        
        //std::unique_ptr<GameState> currentState;
        //vector of state stacks
        std::vector<std::unique_ptr<GameState>> stateStack;

        Snake snake;

        std::vector<CollisionObject> apples;
        std::vector<CollisionObject> bombs;
        std::vector<CollisionObject> bodyParts;

        int m_squareSize = 0;
        int m_offsetX = 0;   
        int m_offsetY = 0;

        int m_screenWidth = GetScreenWidth();
        int m_screenHeight = GetScreenHeight();


    };


 struct bodyPart
{
    bodyPart() {
        colour = SKYBLUE;
    };

   Vector2 position;

   Color colour;

};

struct CollisionObject
{
    enum Type {
        Head, // currently unused
        Body,
        Apple,
        Bomb,
        Tile
    };

    Vector2 position;
    Type type;
    Color colour;
    int numConnections = 0;

    CollisionObject(Vector2 pos, Type t, Color c)
        : position(pos), type(t), colour(c) {}

    
    bool isColliding(const Vector2& otherPos) const {
        return (int)position.x == (int)otherPos.x && (int)position.y == (int)otherPos.y;
    }
};
class Tile{

    public:
        
        Vector2 position;

      
};



class GameState {
    public:
        virtual ~GameState() {}

        virtual void getInput(Snake& snake) = 0;
        virtual void update(Snake& snake, Tile (&tiles)[NUMBER_OF_TILES][NUMBER_OF_TILES], Game& game) = 0;
        virtual void draw(Snake& snake, Tile (&tiles)[NUMBER_OF_TILES][NUMBER_OF_TILES], Game& game) = 0;
};


class MainMenuState : public GameState {
    public:
        void getInput(Snake& snake) override;
        void update(Snake& snake, Tile (&tiles)[NUMBER_OF_TILES][NUMBER_OF_TILES], Game& game) override;
        void draw(Snake& snake, Tile (&tiles)[NUMBER_OF_TILES][NUMBER_OF_TILES], Game& game) override;
};

class PlayingState : public GameState {
    public:
        void getInput(Snake& snake) override;
        void update(Snake& snake, Tile (&tiles)[NUMBER_OF_TILES][NUMBER_OF_TILES], Game& game) override;
        void draw(Snake& snake, Tile (&tiles)[NUMBER_OF_TILES][NUMBER_OF_TILES], Game& game) override;
};

class GameOverState : public GameState {
    public:
        void getInput(Snake& snake) override;
        void update(Snake& snake, Tile (&tiles)[NUMBER_OF_TILES][NUMBER_OF_TILES], Game& game) override;
        void draw(Snake& snake, Tile (&tiles)[NUMBER_OF_TILES][NUMBER_OF_TILES], Game& game) override;
};