// include/Knight.h
#pragma once

#include "Piece.h"

class Knight : public Piece {
public:
    Knight(Color color, int row, int col);
    
    PieceType getType() const override;
    char getSymbol() const override;
    std::vector<Move> generateMoves(const Board& board) const override;
};