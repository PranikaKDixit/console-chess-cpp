// include/Pawn.h
#pragma once

#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(Color color, int row, int col);
    
    PieceType getType() const override;
    char getSymbol() const override;
    std::vector<Move> generateMoves(const Board& board) const override;
    
    // NEW: Generate attack squares for castling validation
    std::vector<Move> generateAttackSquares(const Board& board) const;
    
private:
    int getDirection() const;
    int getStartRank() const;
    int getPromotionRank() const;
};