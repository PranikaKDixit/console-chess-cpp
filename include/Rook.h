// include/Rook.h
#pragma once

#include "Piece.h"

class Rook : public Piece {
public:
    Rook(Color color, int row, int col);
    
    PieceType getType() const override;
    char getSymbol() const override;
    std::vector<Move> generateMoves(const Board& board) const override;
};