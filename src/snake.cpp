#include "snake.hpp"

char Snake::getAxisOfMovement() {
    if (currentDirectionofTravel == 0 || currentDirectionofTravel == 180) {
        return 'y';
    }
    return 'x';
}