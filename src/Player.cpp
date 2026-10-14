// src/Player.cpp
#include "../include/Player.h"

Player::Player(Color color, const std::string& name)
    : color(color), name(name) {}

Color Player::getColor() const {
    return color;
}

std::string Player::getName() const {
    return name;
}