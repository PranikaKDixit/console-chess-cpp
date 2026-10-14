// include/Bishop.h
#pragma once

#include "Piece.h"

class Bishop : public Piece {
public:
    Bishop(Color color, int row, int col);
    
    PieceType getType() const override;
    char getSymbol() const override;
    std::vector<Move> generateMoves(const Board& board) const override;
};