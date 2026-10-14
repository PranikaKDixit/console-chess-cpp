// src/Knight.cpp
#include "../include/Knight.h"
#include "../include/Board.h"

Knight::Knight(Color color, int row, int col)
    : Piece(color, row, col) {}

PieceType Knight::getType() const {
    return PieceType::KNIGHT;
}

char Knight::getSymbol() const {
    return (color == Color::WHITE) ? 'N' : 'n';
}

std::vector<Move> Knight::generateMoves(const Board& board) const {
    std::vector<Move> moves;
    
    // All 8 possible L-shaped moves
    int knightOffsets[8][2] = {
        {-2, -1}, {-2, +1},
        {-1, -2}, {-1, +2},
        {+1, -2}, {+1, +2},
        {+2, -1}, {+2, +1}
    };
    
    for (const auto& offset : knightOffsets) {
        int newRow = row + offset[0];
        int newCol = col + offset[1];
        
        // Bounds check
        if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) {
            continue;
        }
        
        Piece* targetPiece = board.getPiece(newRow, newCol);
        
        if (targetPiece == nullptr) {
            // Empty square
            moves.push_back(Move(row, col, newRow, newCol, Move::MoveType::NORMAL));
        } 
        else if (targetPiece->getColor() != color) {
            // Enemy piece - capture
            moves.push_back(Move(row, col, newRow, newCol, Move::MoveType::CAPTURE));
        }
    }
    
    return moves;
}