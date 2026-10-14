// include/Player.h
#pragma once

#include "Piece.h"
#include <string>

class Player {
private:
    Color color;
    std::string name;
    
public:
    Player(Color color, const std::string& name);
    
    Color getColor() const;
    std::string getName() const;
};