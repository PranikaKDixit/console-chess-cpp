// include/Queen.h
#pragma once

#include "Piece.h"

class Queen : public Piece {
public:
    Queen(Color color, int row, int col);
    
    PieceType getType() const override;
    char getSymbol() const override;
    std::vector<Move> generateMoves(const Board& board) const override;
};