// src/Piece.cpp
#include "../include/Piece.h"

Piece::Piece(Color color, int row, int col)
    : row(row), col(col), color(color), hasMoved(false) {}

Color Piece::getColor() const {
    return color;
}

int Piece::getRow() const {
    return row;
}

int Piece::getCol() const {
    return col;
}

bool Piece::getHasMoved() const {
    return hasMoved;
}

void Piece::setHasMoved(bool moved) {
    hasMoved = moved;
}

void Piece::setPosition(int r, int c) {
    row = r;
    col = c;
}