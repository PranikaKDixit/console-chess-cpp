// src/King.cpp
#include "../include/King.h"
#include "../include/Board.h"

King::King(Color color, int row, int col)
    : Piece(color, row, col) {}

PieceType King::getType() const {
    return PieceType::KING;
}

char King::getSymbol() const {
    return (color == Color::WHITE) ? 'K' : 'k';
}

std::vector<Move> King::generateMoves(const Board& board) const {
    std::vector<Move> moves;
    
    // ============================================
    // NORMAL KING MOVES (One square any direction)
    // ============================================
    int kingMoves[8][2] = {
        {-1, -1}, {-1, 0}, {-1, +1},
        {0, -1},           {0, +1},
        {+1, -1}, {+1, 0}, {+1, +1}
    };
    
    for (const auto& offset : kingMoves) {
        int newRow = row + offset[0];
        int newCol = col + offset[1];
        
        // Bounds check
        if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) {
            continue;
        }
        
        Piece* targetPiece = board.getPiece(newRow, newCol);
        
        if (targetPiece == nullptr) {
            moves.push_back(Move(row, col, newRow, newCol, Move::MoveType::NORMAL));
        }
        else if (targetPiece->getColor() != color) {
            moves.push_back(Move(row, col, newRow, newCol, Move::MoveType::CAPTURE));
        }
    }
    
    // ============================================
    // CASTLING MOVES
    // ============================================
    
    // King-side castling
    if (board.canCastleKingSide(color)) {
        int castleRank = (color == Color::WHITE) ? 7 : 0;
        moves.push_back(Move(row, col, castleRank, 6, Move::MoveType::CASTLING_KINGSIDE));
    }
    
    // Queen-side castling
    if (board.canCastleQueenSide(color)) {
        int castleRank = (color == Color::WHITE) ? 7 : 0;
        moves.push_back(Move(row, col, castleRank, 2, Move::MoveType::CASTLING_QUEENSIDE));
    }
    
    return moves;
}