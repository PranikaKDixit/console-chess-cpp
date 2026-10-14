// include/Board.h
#pragma once

#include "Piece.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"
#include <vector>
#include <string>
#include <memory>

class Board {
private:
    std::vector<std::vector<Piece*>> board;
    std::vector<Move> appliedMoves;
    Color sideToMove;
    
    // Castling rights
    bool whiteKingSideCastling;
    bool whiteQueenSideCastling;
    bool blackKingSideCastling;
    bool blackQueenSideCastling;
    
    // En passant
    bool enPassantActive;
    int enPassantFile;
    int enPassantRank;
    Move lastMove;
    
    // Move counters
    int halfMoveClock;
    int fullMoveNumber;
    
public:
    Board();
    ~Board();
    
    void initializeBoard();
    void displayBoard() const;
    
    Piece* getPiece(int row, int col) const;
    void setPiece(int row, int col, Piece* piece);
    
    bool isMoveLegal(const Move& move, Color playerColor);
    std::vector<Move> getLegalMoves(Color color);
    
    bool makeMove(const Move& move);
    bool undoMove(const Move& move);
    
    bool isKingInCheck(Color king) const;
    bool isSquareAttacked(int row, int col, Color colorBeingAttacked) const;
    
    // Castling
    bool canCastleKingSide(Color color) const;
    bool canCastleQueenSide(Color color) const;
    bool isCastleLegal(const Move& move, Color king);
    void executeCastling(const Move& move);
    void updateCastlingRights(const Move& move);
    
    // En passant
    bool canEnPassantCapture(int fromRow, int fromCol, int toRow, int toCol) const;
    void updateEnPassant(const Move& move);
    
    // Game state
    void updateHalfMoveClock(const Move& move);
    int getHalfMoveClock() const { return halfMoveClock; }
    int getFullMoveNumber() const { return fullMoveNumber; }
    
    std::string getFEN() const;
    
private:
    Piece* createPiece(PieceType type, Color color, int row, int col);
    void clearBoard();
    void deleteBoard();
};