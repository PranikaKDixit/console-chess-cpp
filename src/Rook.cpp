// src/Rook.cpp
#include "../include/Rook.h"
#include "../include/Board.h"

Rook::Rook(Color color, int row, int col)
    : Piece(color, row, col) {}

PieceType Rook::getType() const {
    return PieceType::ROOK;
}

char Rook::getSymbol() const {
    return (color == Color::WHITE) ? 'R' : 'r';
}

std::vector<Move> Rook::generateMoves(const Board& board) const {
    std::vector<Move> moves;
    
    // All 4 straight directions
    int straightDirections[4][2] = {
        {-1, 0},   // Up
        {+1, 0},   // Down
        {0, -1},   // Left
        {0, +1}    // Right
    };
    
    for (const auto& direction : straightDirections) {
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