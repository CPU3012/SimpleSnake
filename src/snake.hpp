#pragma once

#include <raylib.h>
#include <vector>

struct bodyPart;

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

 struct bodyPart
{
    bodyPart() {
        colour = SKYBLUE;
    };

   Vector2 position;

   Color colour;

};
