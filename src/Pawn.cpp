// src/Pawn.cpp
#include "../include/Pawn.h"
#include "../include/Board.h"

Pawn::Pawn(Color color, int row, int col)
    : Piece(color, row, col) {}

PieceType Pawn::getType() const {
    return PieceType::PAWN;
}

char Pawn::getSymbol() const {
    return (color == Color::WHITE) ? 'P' : 'p';
}

int Pawn::getDirection() const {
    return (color == Color::WHITE) ? -1 : 1;
}

int Pawn::getStartRank() const {
    return (color == Color::WHITE) ? 6 : 1;
}

int Pawn::getPromotionRank() const {
    return (color == Color::WHITE) ? 0 : 7;
}

std::vector<Move> Pawn::generateMoves(const Board& board) const {
    std::vector<Move> moves;
    
    int direction = getDirection();
    int startRank = getStartRank();
    int promotionRank = getPromotionRank();
    int forwardRow = row + direction;
    
    // ============================================
    // SINGLE FORWARD MOVE
    // ============================================
    if (forwardRow >= 0 && forwardRow < 8) {
        Piece* forwardSquare = board.getPiece(forwardRow, col);
        
        if (forwardSquare == nullptr) {
            Move move(row, col, forwardRow, col, Move::MoveType::NORMAL);
            
            // CHECK FOR PROMOTION
            if (forwardRow == promotionRank) {
                move.isPromotion = true;
                // Add four promotion moves (Q, R, B, N)
                for (PieceType promo : {PieceType::QUEEN, PieceType::ROOK, 
                                        PieceType::BISHOP, PieceType::KNIGHT}) {
                    Move promMove = move;
                    promMove.promotionPiece = promo;
                    moves.push_back(promMove);
                }
            } else {
                moves.push_back(move);
            }
        }
    }
    
    // ============================================
    // DOUBLE FORWARD MOVE (From starting position)
    // ============================================
    if (row == startRank) {
        int singleRow = row + direction;
        int doubleRow = row + 2 * direction;
        
        // Both squares must be empty
        if (board.getPiece(singleRow, col) == nullptr &&
            board.getPiece(doubleRow, col) == nullptr) {
            moves.push_back(Move(row, col, doubleRow, col, 
                               Move::MoveType::PAWN_DOUBLE_MOVE));
        }
    }
    
    // ============================================
    // DIAGONAL CAPTURES
    // ============================================
    for (int dcol : {-1, 1}) {
        int captureCol = col + dcol;
        int captureRow = row + direction;
        
        // Check bounds
        if (captureRow < 0 || captureRow >= 8 || 
            captureCol < 0 || captureCol >= 8) {
            continue;
        }
        
        Piece* targetPiece = board.getPiece(captureRow, captureCol);
        
        // Normal capture
        if (targetPiece != nullptr && targetPiece->getColor() != color) {
            Move move(row, col, captureRow, captureCol, Move::MoveType::CAPTURE);
            
            // CHECK FOR PROMOTION ON CAPTURE
            if (captureRow == promotionRank) {
                move.isPromotion = true;
                // Add four promotion moves
                for (PieceType promo : {PieceType::QUEEN, PieceType::ROOK, 
                                        PieceType::BISHOP, PieceType::KNIGHT}) {
                    Move promMove = move;
                    promMove.promotionPiece = promo;
                    moves.push_back(promMove);
                }
            } else {
                moves.push_back(move);
            }
        }
        
        // EN PASSANT CAPTURE
        if (board.canEnPassantCapture(row, col, captureRow, captureCol)) {
            Move move(row, col, captureRow, captureCol, Move::MoveType::EN_PASSANT);
            move.capturedPawnRow = row;
            move.capturedPawnCol = captureCol;
            moves.push_back(move);
        }
    }
    
    return moves;
}

// FIXED: Generate attack squares (for castling validation through check)
std::vector<Move> Pawn::generateAttackSquares(const Board& board) const {
    std::vector<Move> attacks;
    
    int direction = getDirection();
    
    // Diagonal attacks (even on empty squares)
    for (int dcol : {-1, 1}) {
        int attackCol = col + dcol;
        int attackRow = row + direction;
        
        // Check bounds
        if (attackRow >= 0 && attackRow < 8 && 
            attackCol >= 0 && attackCol < 8) {
            // Generate attack move regardless of what's on the square
            attacks.push_back(Move(row, col, attackRow, attackCol, Move::MoveType::CAPTURE));
        }
    }
    
    return attacks;
}