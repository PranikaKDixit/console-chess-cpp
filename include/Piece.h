// include/Piece.h
#pragma once

#include <vector>

enum class Color {
    WHITE,
    BLACK,
    NONE
};

enum class PieceType {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    NONE
};

struct Move {
    int fromRow, fromCol;
    int toRow, toCol;
    
    enum class MoveType {
        NORMAL,
        CAPTURE,
        CASTLING_KINGSIDE,
        CASTLING_QUEENSIDE,
        EN_PASSANT,
        PAWN_DOUBLE_MOVE,
        PROMOTION
    };
    
    MoveType moveType;
    bool isPromotion;
    PieceType promotionPiece;
    
    // For en passant - actual position of captured pawn
    int capturedPawnRow;
    int capturedPawnCol;
    
    Move() : fromRow(-1), fromCol(-1), toRow(-1), toCol(-1),
             moveType(MoveType::NORMAL), isPromotion(false),
             promotionPiece(PieceType::NONE),
             capturedPawnRow(-1), capturedPawnCol(-1) {}
    
    Move(int fr, int fc, int tr, int tc, MoveType mt = MoveType::NORMAL,
         int capRow = -1, int capCol = -1)
        : fromRow(fr), fromCol(fc), toRow(tr), toCol(tc),
          moveType(mt), isPromotion(false), promotionPiece(PieceType::NONE),
          capturedPawnRow(capRow), capturedPawnCol(capCol) {}
    
    // FIXED: Include promotion fields in comparison
    bool operator==(const Move& other) const {
        return fromRow == other.fromRow && fromCol == other.fromCol &&
               toRow == other.toRow && toCol == other.toCol &&
               moveType == other.moveType &&
               isPromotion == other.isPromotion &&
               promotionPiece == other.promotionPiece &&
               capturedPawnRow == other.capturedPawnRow &&
               capturedPawnCol == other.capturedPawnCol;
    }
    
    // Helper for validation
    bool isValid() const {
        return fromRow >= 0 && fromRow < 8 && fromCol >= 0 && fromCol < 8 &&
               toRow >= 0 && toRow < 8 && toCol >= 0 && toCol < 8;
    }
};

// Forward declaration
class Board;

class Piece {
protected:
    int row, col;
    Color color;
    bool hasMoved;
    
public:
    explicit Piece(Color color, int row = -1, int col = -1);
    virtual ~Piece() = default;
    
    Color getColor() const;
    int getRow() const;
    int getCol() const;
    bool getHasMoved() const;
    void setHasMoved(bool moved);
    void setPosition(int r, int c);
    
    virtual PieceType getType() const = 0;
    virtual char getSymbol() const = 0;
    virtual std::vector<Move> generateMoves(const Board& board) const = 0;
};