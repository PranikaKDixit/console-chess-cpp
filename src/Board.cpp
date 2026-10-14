// src/Board.cpp
#include "../include/Board.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

Board::Board()
    : sideToMove(Color::WHITE),
      whiteKingSideCastling(true),
      whiteQueenSideCastling(true),
      blackKingSideCastling(true),
      blackQueenSideCastling(true),
      enPassantActive(false),
      enPassantFile(-1),
      enPassantRank(-1),
      halfMoveClock(0),
      fullMoveNumber(1) {
    board.resize(8, std::vector<Piece*>(8, nullptr));
    initializeBoard();
}

Board::~Board() {
    deleteBoard();
}

void Board::deleteBoard() {
    for (std::vector<Piece*>& row : board) {
        for (Piece*& piece : row) {
            delete piece;
            piece = nullptr;
        }
    }
}

void Board::clearBoard() {
    deleteBoard();
}

Piece* Board::createPiece(PieceType type, Color color, int row, int col) {
    switch (type) {
        case PieceType::PAWN:
            return new Pawn(color, row, col);
        case PieceType::KNIGHT:
            return new Knight(color, row, col);
        case PieceType::BISHOP:
            return new Bishop(color, row, col);
        case PieceType::ROOK:
            return new Rook(color, row, col);
        case PieceType::QUEEN:
            return new Queen(color, row, col);
        case PieceType::KING:
            return new King(color, row, col);
        default:
            return nullptr;
    }
}

void Board::initializeBoard() {
    clearBoard();
    appliedMoves.clear();
    sideToMove = Color::WHITE;

    board[7][0] = new Rook(Color::WHITE, 7, 0);
    board[7][1] = new Knight(Color::WHITE, 7, 1);
    board[7][2] = new Bishop(Color::WHITE, 7, 2);
    board[7][3] = new Queen(Color::WHITE, 7, 3);
    board[7][4] = new King(Color::WHITE, 7, 4);
    board[7][5] = new Bishop(Color::WHITE, 7, 5);
    board[7][6] = new Knight(Color::WHITE, 7, 6);
    board[7][7] = new Rook(Color::WHITE, 7, 7);

    board[0][0] = new Rook(Color::BLACK, 0, 0);
    board[0][1] = new Knight(Color::BLACK, 0, 1);
    board[0][2] = new Bishop(Color::BLACK, 0, 2);
    board[0][3] = new Queen(Color::BLACK, 0, 3);
    board[0][4] = new King(Color::BLACK, 0, 4);
    board[0][5] = new Bishop(Color::BLACK, 0, 5);
    board[0][6] = new Knight(Color::BLACK, 0, 6);
    board[0][7] = new Rook(Color::BLACK, 0, 7);

    for (int col = 0; col < 8; ++col) {
        board[6][col] = new Pawn(Color::WHITE, 6, col);
        board[1][col] = new Pawn(Color::BLACK, 1, col);
    }

    whiteKingSideCastling = true;
    whiteQueenSideCastling = true;
    blackKingSideCastling = true;
    blackQueenSideCastling = true;
    enPassantActive = false;
    enPassantFile = -1;
    enPassantRank = -1;
    lastMove = Move();
    halfMoveClock = 0;
    fullMoveNumber = 1;
}

void Board::displayBoard() const {
    std::cout << "\n  a b c d e f g h\n";
    std::cout << "  ----------------\n";

    for (int row = 0; row < 8; ++row) {
        std::cout << (8 - row) << " ";
        for (int col = 0; col < 8; ++col) {
            const Piece* piece = board[row][col];
            std::cout << (piece == nullptr ? '.' : piece->getSymbol()) << ' ';
        }
        std::cout << (8 - row) << '\n';
    }

    std::cout << "  ----------------\n";
    std::cout << "  a b c d e f g h\n\n";
}

Piece* Board::getPiece(int row, int col) const {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        return nullptr;
    }
    return board[row][col];
}

void Board::setPiece(int row, int col, Piece* piece) {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) {
        return;
    }

    if (board[row][col] != piece) {
        delete board[row][col];
        board[row][col] = piece;
    }
    if (piece != nullptr) {
        piece->setPosition(row, col);
    }
}

bool Board::isKingInCheck(Color kingColor) const {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            const Piece* piece = board[row][col];
            if (piece != nullptr && piece->getType() == PieceType::KING &&
                piece->getColor() == kingColor) {
                return isSquareAttacked(row, col, kingColor);
            }
        }
    }
    return false;
}

bool Board::isSquareAttacked(int row, int col, Color colorBeingAttacked) const {
    if (row < 0 || row >= 8 || col < 0 || col >= 8 ||
        colorBeingAttacked == Color::NONE) {
        return false;
    }

    const Color attackerColor =
        colorBeingAttacked == Color::WHITE ? Color::BLACK : Color::WHITE;

    for (int fromRow = 0; fromRow < 8; ++fromRow) {
        for (int fromCol = 0; fromCol < 8; ++fromCol) {
            const Piece* attacker = board[fromRow][fromCol];
            if (attacker == nullptr || attacker->getColor() != attackerColor) {
                continue;
            }

            const int rowDelta = row - fromRow;
            const int colDelta = col - fromCol;
            const int absRowDelta = std::abs(rowDelta);
            const int absColDelta = std::abs(colDelta);

            switch (attacker->getType()) {
                case PieceType::PAWN: {
                    const int direction =
                        attackerColor == Color::WHITE ? -1 : 1;
                    if (rowDelta == direction && absColDelta == 1) {
                        return true;
                    }
                    break;
                }
                case PieceType::KNIGHT:
                    if ((absRowDelta == 2 && absColDelta == 1) ||
                        (absRowDelta == 1 && absColDelta == 2)) {
                        return true;
                    }
                    break;
                case PieceType::KING:
                    if (absRowDelta <= 1 && absColDelta <= 1 &&
                        (absRowDelta != 0 || absColDelta != 0)) {
                        return true;
                    }
                    break;
                case PieceType::BISHOP:
                case PieceType::ROOK:
                case PieceType::QUEEN: {
                    const bool diagonal =
                        absRowDelta == absColDelta && absRowDelta != 0;
                    const bool straight =
                        (rowDelta == 0) != (colDelta == 0);
                    const PieceType type = attacker->getType();
                    const bool directionAllowed =
                        (type == PieceType::QUEEN && (diagonal || straight)) ||
                        (type == PieceType::BISHOP && diagonal) ||
                        (type == PieceType::ROOK && straight);
                    if (!directionAllowed) {
                        break;
                    }

                    const int stepRow = (rowDelta > 0) - (rowDelta < 0);
                    const int stepCol = (colDelta > 0) - (colDelta < 0);
                    int checkRow = fromRow + stepRow;
                    int checkCol = fromCol + stepCol;
                    bool blocked = false;
                    while (checkRow != row || checkCol != col) {
                        if (board[checkRow][checkCol] != nullptr) {
                            blocked = true;
                            break;
                        }
                        checkRow += stepRow;
                        checkCol += stepCol;
                    }
                    if (!blocked) {
                        return true;
                    }
                    break;
                }
                case PieceType::NONE:
                    break;
            }
        }
    }

    return false;
}

bool Board::canCastleKingSide(Color color) const {
    return color == Color::WHITE ? whiteKingSideCastling
                                 : color == Color::BLACK && blackKingSideCastling;
}

bool Board::canCastleQueenSide(Color color) const {
    return color == Color::WHITE ? whiteQueenSideCastling
                                 : color == Color::BLACK && blackQueenSideCastling;
}

bool Board::isCastleLegal(const Move& move, Color kingColor) {
    if (!move.isValid() ||
        (move.moveType != Move::MoveType::CASTLING_KINGSIDE &&
         move.moveType != Move::MoveType::CASTLING_QUEENSIDE)) {
        return false;
    }

    const int homeRow = kingColor == Color::WHITE ? 7 : 0;
    const int rookCol =
        move.moveType == Move::MoveType::CASTLING_KINGSIDE ? 7 : 0;
    const int kingDestinationCol =
        move.moveType == Move::MoveType::CASTLING_KINGSIDE ? 6 : 2;
    const int rookDestinationCol =
        move.moveType == Move::MoveType::CASTLING_KINGSIDE ? 5 : 3;

    if (move.fromRow != homeRow || move.fromCol != 4 ||
        move.toRow != homeRow || move.toCol != kingDestinationCol ||
        (move.moveType == Move::MoveType::CASTLING_KINGSIDE
             ? !canCastleKingSide(kingColor)
             : !canCastleQueenSide(kingColor))) {
        return false;
    }

    const Piece* king = board[homeRow][4];
    const Piece* rook = board[homeRow][rookCol];
    if (king == nullptr || king->getType() != PieceType::KING ||
        king->getColor() != kingColor || king->getHasMoved() ||
        rook == nullptr || rook->getType() != PieceType::ROOK ||
        rook->getColor() != kingColor || rook->getHasMoved()) {
        return false;
    }

    if (move.moveType == Move::MoveType::CASTLING_KINGSIDE) {
        if (board[homeRow][5] != nullptr || board[homeRow][6] != nullptr) {
            return false;
        }
        return !isSquareAttacked(homeRow, 4, kingColor) &&
               !isSquareAttacked(homeRow, 5, kingColor) &&
               !isSquareAttacked(homeRow, 6, kingColor);
    }

    if (board[homeRow][1] != nullptr || board[homeRow][2] != nullptr ||
        board[homeRow][3] != nullptr) {
        return false;
    }
    return !isSquareAttacked(homeRow, 4, kingColor) &&
           !isSquareAttacked(homeRow, 3, kingColor) &&
           !isSquareAttacked(homeRow, 2, kingColor);
}

void Board::executeCastling(const Move& move) {
    const int row = move.fromRow;
    const bool kingSide =
        move.moveType == Move::MoveType::CASTLING_KINGSIDE;
    const int rookFromCol = kingSide ? 7 : 0;
    const int kingToCol = kingSide ? 6 : 2;
    const int rookToCol = kingSide ? 5 : 3;

    Piece* king = board[row][4];
    Piece* rook = board[row][rookFromCol];
    board[row][4] = nullptr;
    board[row][rookFromCol] = nullptr;
    board[row][kingToCol] = king;
    board[row][rookToCol] = rook;
    king->setPosition(row, kingToCol);
    rook->setPosition(row, rookToCol);
    king->setHasMoved(true);
    rook->setHasMoved(true);
}

void Board::updateCastlingRights(const Move& move) {
    const Piece* movingPiece = board[move.fromRow][move.fromCol];
    if (movingPiece == nullptr) {
        return;
    }

    const Color movingColor = movingPiece->getColor();
    if (movingPiece->getType() == PieceType::KING) {
        if (movingColor == Color::WHITE) {
            whiteKingSideCastling = false;
            whiteQueenSideCastling = false;
        } else {
            blackKingSideCastling = false;
            blackQueenSideCastling = false;
        }
    } else if (movingPiece->getType() == PieceType::ROOK) {
        const int homeRow = movingColor == Color::WHITE ? 7 : 0;
        if (move.fromRow == homeRow && move.fromCol == 0) {
            if (movingColor == Color::WHITE) {
                whiteQueenSideCastling = false;
            } else {
                blackQueenSideCastling = false;
            }
        } else if (move.fromRow == homeRow && move.fromCol == 7) {
            if (movingColor == Color::WHITE) {
                whiteKingSideCastling = false;
            } else {
                blackKingSideCastling = false;
            }
        }
    }

    if (move.moveType == Move::MoveType::EN_PASSANT) {
        return;
    }

    const Piece* capturedPiece = board[move.toRow][move.toCol];
    if (capturedPiece == nullptr || capturedPiece->getType() != PieceType::ROOK) {
        return;
    }

    const Color capturedColor = capturedPiece->getColor();
    const int capturedHomeRow = capturedColor == Color::WHITE ? 7 : 0;
    if (move.toRow != capturedHomeRow) {
        return;
    }
    if (move.toCol == 0) {
        if (capturedColor == Color::WHITE) {
            whiteQueenSideCastling = false;
        } else {
            blackQueenSideCastling = false;
        }
    } else if (move.toCol == 7) {
        if (capturedColor == Color::WHITE) {
            whiteKingSideCastling = false;
        } else {
            blackKingSideCastling = false;
        }
    }
}

bool Board::canEnPassantCapture(int fromRow, int fromCol,
                                int toRow, int toCol) const {
    if (!enPassantActive || toRow != enPassantRank ||
        toCol != enPassantFile || getPiece(toRow, toCol) != nullptr) {
        return false;
    }

    const Piece* pawn = getPiece(fromRow, fromCol);
    if (pawn == nullptr || pawn->getType() != PieceType::PAWN ||
        lastMove.moveType != Move::MoveType::PAWN_DOUBLE_MOVE ||
        lastMove.toRow != fromRow || lastMove.toCol != toCol ||
        std::abs(fromCol - toCol) != 1) {
        return false;
    }

    const int direction = pawn->getColor() == Color::WHITE ? -1 : 1;
    if (toRow != fromRow + direction) {
        return false;
    }

    const Piece* capturedPawn = getPiece(lastMove.toRow, lastMove.toCol);
    return capturedPawn != nullptr &&
           capturedPawn->getType() == PieceType::PAWN &&
           capturedPawn->getColor() != pawn->getColor();
}

void Board::updateEnPassant(const Move& move) {
    enPassantActive = false;
    enPassantFile = -1;
    enPassantRank = -1;

    if (move.moveType != Move::MoveType::PAWN_DOUBLE_MOVE) {
        return;
    }

    const Piece* piece = board[move.toRow][move.toCol];
    if (piece == nullptr || piece->getType() != PieceType::PAWN ||
        std::abs(move.fromRow - move.toRow) != 2) {
        return;
    }

    enPassantActive = true;
    enPassantFile = move.toCol;
    enPassantRank = (move.fromRow + move.toRow) / 2;
    lastMove = move;
}

void Board::updateHalfMoveClock(const Move& move) {
    const Piece* movedPiece = board[move.toRow][move.toCol];
    if (move.moveType == Move::MoveType::CAPTURE ||
        move.moveType == Move::MoveType::EN_PASSANT ||
        move.isPromotion ||
        (movedPiece != nullptr && movedPiece->getType() == PieceType::PAWN)) {
        halfMoveClock = 0;
    } else {
        ++halfMoveClock;
    }
}

bool Board::isMoveLegal(const Move& move, Color playerColor) {
    if (!move.isValid() || playerColor == Color::NONE) {
        return false;
    }

    Piece* movingPiece = board[move.fromRow][move.fromCol];
    if (movingPiece == nullptr || movingPiece->getColor() != playerColor) {
        return false;
    }

    if (move.moveType == Move::MoveType::CASTLING_KINGSIDE ||
        move.moveType == Move::MoveType::CASTLING_QUEENSIDE) {
        return movingPiece->getType() == PieceType::KING &&
               isCastleLegal(move, playerColor);
    }

    const std::vector<Move> pseudoLegalMoves = movingPiece->generateMoves(*this);
    bool foundMove = false;
    for (const Move& pseudoMove : pseudoLegalMoves) {
        if (pseudoMove == move) {
            foundMove = true;
            break;
        }
    }
    if (!foundMove) {
        return false;
    }

    Piece* capturedPiece = board[move.toRow][move.toCol];
    if (capturedPiece != nullptr &&
        capturedPiece->getType() == PieceType::KING) {
        return false;
    }
    const int oldRow = movingPiece->getRow();
    const int oldCol = movingPiece->getCol();

    board[move.toRow][move.toCol] = movingPiece;
    board[move.fromRow][move.fromCol] = nullptr;
    movingPiece->setPosition(move.toRow, move.toCol);

    Piece* capturedEnPassant = nullptr;
    if (move.moveType == Move::MoveType::EN_PASSANT) {
        capturedEnPassant = board[move.capturedPawnRow][move.capturedPawnCol];
        board[move.capturedPawnRow][move.capturedPawnCol] = nullptr;
    }

    const bool kingSafe = !isKingInCheck(playerColor);

    if (move.moveType == Move::MoveType::EN_PASSANT) {
        board[move.capturedPawnRow][move.capturedPawnCol] = capturedEnPassant;
    }
    board[move.fromRow][move.fromCol] = movingPiece;
    board[move.toRow][move.toCol] = capturedPiece;
    movingPiece->setPosition(oldRow, oldCol);

    return kingSafe;
}

std::vector<Move> Board::getLegalMoves(Color color) {
    std::vector<Move> legalMoves;
    if (color == Color::NONE) {
        return legalMoves;
    }

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece* piece = board[row][col];
            if (piece == nullptr || piece->getColor() != color) {
                continue;
            }
            for (const Move& move : piece->generateMoves(*this)) {
                if (isMoveLegal(move, color)) {
                    legalMoves.push_back(move);
                }
            }
        }
    }
    return legalMoves;
}

bool Board::makeMove(const Move& move) {
    if (!move.isValid()) {
        return false;
    }

    Piece* movingPiece = board[move.fromRow][move.fromCol];
    if (movingPiece == nullptr || movingPiece->getColor() != sideToMove ||
        !isMoveLegal(move, movingPiece->getColor())) {
        return false;
    }

    const Color movingColor = movingPiece->getColor();
    updateCastlingRights(move);

    if (move.moveType == Move::MoveType::CASTLING_KINGSIDE ||
        move.moveType == Move::MoveType::CASTLING_QUEENSIDE) {
        executeCastling(move);
        enPassantActive = false;
        enPassantFile = -1;
        enPassantRank = -1;
        ++halfMoveClock;
        if (movingColor == Color::BLACK) {
            ++fullMoveNumber;
        }
        sideToMove = movingColor == Color::WHITE ? Color::BLACK : Color::WHITE;
        appliedMoves.push_back(move);
        return true;
    }

    if (move.moveType == Move::MoveType::EN_PASSANT) {
        delete board[move.capturedPawnRow][move.capturedPawnCol];
        board[move.capturedPawnRow][move.capturedPawnCol] = nullptr;
    } else {
        delete board[move.toRow][move.toCol];
    }

    board[move.toRow][move.toCol] = movingPiece;
    board[move.fromRow][move.fromCol] = nullptr;
    movingPiece->setPosition(move.toRow, move.toCol);
    movingPiece->setHasMoved(true);

    if (move.isPromotion) {
        Piece* promotedPiece =
            createPiece(move.promotionPiece, movingColor, move.toRow, move.toCol);
        if (promotedPiece == nullptr) {
            return false;
        }
        delete movingPiece;
        board[move.toRow][move.toCol] = promotedPiece;
        promotedPiece->setHasMoved(true);
    }

    updateEnPassant(move);
    updateHalfMoveClock(move);
    if (movingColor == Color::BLACK) {
        ++fullMoveNumber;
    }
    sideToMove = movingColor == Color::WHITE ? Color::BLACK : Color::WHITE;
    appliedMoves.push_back(move);
    return true;
}

bool Board::undoMove(const Move& move) {
    if (appliedMoves.empty() || !(appliedMoves.back() == move)) {
        return false;
    }

    appliedMoves.pop_back();
    const std::vector<Move> remainingMoves = appliedMoves;
    initializeBoard();
    for (const Move& previousMove : remainingMoves) {
        if (!makeMove(previousMove)) {
            return false;
        }
    }
    return true;
}

std::string Board::getFEN() const {
    std::ostringstream fen;
    for (int row = 0; row < 8; ++row) {
        int emptySquares = 0;
        for (int col = 0; col < 8; ++col) {
            const Piece* piece = board[row][col];
            if (piece == nullptr) {
                ++emptySquares;
                continue;
            }
            if (emptySquares > 0) {
                fen << emptySquares;
                emptySquares = 0;
            }
            fen << piece->getSymbol();
        }
        if (emptySquares > 0) {
            fen << emptySquares;
        }
        if (row != 7) {
            fen << '/';
        }
    }

    fen << (sideToMove == Color::WHITE ? " w " : " b ");
    std::string castlingRights;
    if (whiteKingSideCastling) castlingRights += 'K';
    if (whiteQueenSideCastling) castlingRights += 'Q';
    if (blackKingSideCastling) castlingRights += 'k';
    if (blackQueenSideCastling) castlingRights += 'q';
    fen << (castlingRights.empty() ? "-" : castlingRights) << ' ';

    if (enPassantActive) {
        fen << static_cast<char>('a' + enPassantFile) << (8 - enPassantRank);
    } else {
        fen << '-';
    }
    fen << ' ' << halfMoveClock << ' ' << fullMoveNumber;
    return fen.str();
}
