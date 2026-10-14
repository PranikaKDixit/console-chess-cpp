// include/King.h
#pragma once

#include "Piece.h"

class King : public Piece {
public:
    King(Color color, int row, int col);
    
    PieceType getType() const override;
    char getSymbol() const override;
    std::vector<Move> generateMoves(const Board& board) const override;
};