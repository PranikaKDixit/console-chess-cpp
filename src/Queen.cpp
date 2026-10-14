// src/Queen.cpp
#include "../include/Queen.h"
#include "../include/Board.h"

Queen::Queen(Color color, int row, int col)
    : Piece(color, row, col) {}

PieceType Queen::getType() const {
    return PieceType::QUEEN;
}

char Queen::getSymbol() const {
    return (color == Color::WHITE) ? 'Q' : 'q';
}

std::vector<Move> Queen::generateMoves(const Board& board) const {
    std::vector<Move> moves;
    
    // 8 directions (4 straight + 4 diagonal)
    int queenDirections[8][2] = {
        {-1, 0}, {+1, 0}, {0, -1}, {0, +1},    // Straight
        {-1, -1}, {-1, +1}, {+1, -1}, {+1, +1} // Diagonal
    };
    
    for (const auto& direction : queenDirections) {
        for (int distance = 1; distance < 8; distance++) {
            int newRow = row + direction[0] * distance;
            int newCol = col + direction[1] * distance;
            
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