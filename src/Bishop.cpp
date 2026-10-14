// src/Bishop.cpp
#include "../include/Bishop.h"
#include "../include/Board.h"

Bishop::Bishop(Color color, int row, int col)
    : Piece(color, row, col) {}

PieceType Bishop::getType() const {
    return PieceType::BISHOP;
}

char Bishop::getSymbol() const {
    return (color == Color::WHITE) ? 'B' : 'b';
}

std::vector<Move> Bishop::generateMoves(const Board& board) const {
    std::vector<Move> moves;
    
    // All 4 diagonal directions
    int diagonalDirections[4][2] = {
        {-1, -1},  // Up-left
        {-1, +1},  // Up-right
        {+1, -1},  // Down-left
        {+1, +1}   // Down-right
    };
    
    for (const auto& direction : diagonalDirections) {
        for (int distance = 1; distance < 8; distance++) {
            int newRow = row + direction[0] * distance;
            int newCol = col + direction[1] * distance;
            
            // Bounds check
            if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) {
                break;
            }
            
            Piece* targetPiece = board.getPiece(newRow, newCol);
            
            if (targetPiece == nullptr) {
                moves.push_back(Move(row, col, newRow, newCol, Move::MoveType::NORMAL));
            }
            else if (targetPiece->getColor() != color) {
                moves.push_back(Move(row, col, newRow, newCol, Move::MoveType::CAPTURE));
                break;
            }
            else {
                break;
            }
        }
    }
    
    return moves;
}