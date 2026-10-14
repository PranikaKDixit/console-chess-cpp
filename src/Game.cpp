// src/Game.cpp - COMPLETE FILE (FIXED)
#include "../include/Game.h"
#include <cctype>
#include <iostream>
#include <sstream>
#include <algorithm>

static std::string moveToString(const Move& m) {
    auto sq = [](int r, int c) {
        std::string s;
        s += char('a' + c);
        s += char('0' + (8 - r));
        return s;
    };

    std::string out = sq(m.fromRow, m.fromCol) + sq(m.toRow, m.toCol);

    if (m.moveType == Move::MoveType::CASTLING_KINGSIDE) out += " (O-O)";
    if (m.moveType == Move::MoveType::CASTLING_QUEENSIDE) out += " (O-O-O)";
    if (m.moveType == Move::MoveType::EN_PASSANT) out += " (e.p.)";

    if (m.isPromotion) {
        out += "=";
        out += (m.promotionPiece == PieceType::QUEEN) ? 'Q' :
               (m.promotionPiece == PieceType::ROOK)  ? 'R' :
               (m.promotionPiece == PieceType::BISHOP)? 'B' : 'N';
    }
    return out;
}

Game::Game() : currentPlayer(Color::WHITE), gameOver(false), winner(Color::NONE) {
    board.initializeBoard();
}

void Game::start() {
    std::cout << "\n========================================\n";
    std::cout << "        WELCOME TO CHESS ENGINE\n";
    std::cout << "========================================\n";
    std::cout << "Commands:\n";
    std::cout << " Move: enter 'e2e4' (from square to square)\n";
    std::cout << " Quit: enter 'quit'\n";
    std::cout << " Undo: enter 'undo'\n";
    std::cout << "========================================\n\n";

    while (!gameOver) {
        board.displayBoard();
        displayStatus();

        Move move = getPlayerMove();
        if (gameOver) break;              // player typed quit
        if (move.fromRow == -1) continue; // undo signal

        if (!executeMove(move)) {
            std::cout << "   Illegal move! Try again.\n\n";
            continue;
        }

        // IMPORTANT: after a successful move, the opponent is the side to check
        switchTurns();

        // Now currentPlayer is the side who must respond
        if (isCheckmate()) {
            board.displayBoard();
            std::cout << "\n╔══════════════════════════════════════╗\n";
            std::cout << "║              CHECKMATE!              ║\n";
            std::cout << (currentPlayer == Color::WHITE ?
                          "║            BLACK WINS!               ║\n" :
                          "║            WHITE WINS!               ║\n");
            std::cout << "╚══════════════════════════════════════╝\n";

            Color winnerColor = (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;
            endGame(winnerColor);
            break;
        }

        if (isStalemate()) {
            board.displayBoard();
            std::cout << "\n╔══════════════════════════════════════╗\n";
            std::cout << "║           STALEMATE - DRAW!          ║\n";
            std::cout << "╚══════════════════════════════════════╝\n";
            endGame(Color::NONE);
            break;
        }

        if (isDraw()) {
            board.displayBoard();
            std::cout << "\n╔══════════════════════════════════════╗\n";
            std::cout << "║                DRAW!                 ║\n";
            std::cout << "╚══════════════════════════════════════╝\n";
            endGame(Color::NONE);
            break;
        }

        if (isCheck()) {
            std::cout << "\n    CHECK!\n\n";
        }
    }

    board.displayBoard();
    std::cout << "\nGame Over!\n";
    std::cout << "Move History: " << getMoveHistory() << "\n";
}

void Game::displayStatus() const {
    std::cout << "═══════════════════════════════════════\n";
    std::cout << "Current Player: " 
              << (currentPlayer == Color::WHITE ? "WHITE" : "BLACK") << "\n";
    std::cout << "Moves Played: " << moveHistory.size() << "\n";
    
    if (isCheck()) {
        std::cout << "⚠️  Player is in CHECK!\n";
    }
    
    std::cout << "═══════════════════════════════════════\n\n";
}

Move Game::getPlayerMove() {
    std::string input;

    while (true) {
        std::cout << (currentPlayer == Color::WHITE ? "WHITE" : "BLACK")
                  << " move (e.g., e2e4): ";
        if (!std::getline(std::cin, input)) {
            gameOver = true;
            return Move();
        }

        // Special commands
        if (input == "quit") {
            std::cout << "Game ended.\n";
            gameOver = true;
            return Move();
        }

        if (input == "undo") {
            if (moveHistory.size() >= 2) {
                moveHistory.pop_back();
                moveHistory.pop_back();

                board.initializeBoard();
                for (const Move& m : moveHistory) {
                    board.makeMove(m);
                }

                // Recompute whose turn it is from history length (0 => WHITE)
                currentPlayer = (moveHistory.size() % 2 == 0) ? Color::WHITE : Color::BLACK;

                std::cout << "✓ Last full move undone.\n\n";
                return Move(-1, -1, -1, -1); // signal: skip normal flow
            } else if (moveHistory.size() == 1) {
                std::cout << "     Can only undo full moves (both players).\n";
            } else {
                std::cout << "     Cannot undo - no moves to undo.\n";
            }
            continue;
        }

        if (input == "list") {
            std::vector<Move> legalMoves = board.getLegalMoves(currentPlayer);

            if (legalMoves.empty()) {
                std::cout << "No legal moves available.\n";
                continue;
            }

            std::cout << "\nLegal moves:\n";
            for (size_t i = 0; i < legalMoves.size(); ++i) {
                std::cout << "  " << (i + 1) << ") " << moveToString(legalMoves[i]) << "\n";
            }

            std::cout << "\nChoose move number: ";
            std::string numInput;
            if (!std::getline(std::cin, numInput)) {
                gameOver = true;
                return Move();
            }

            try {
                size_t parsedCharacters = 0;
                const int idx = std::stoi(numInput, &parsedCharacters);
                if (parsedCharacters != numInput.length() ||
                    idx < 1 || idx > static_cast<int>(legalMoves.size())) {
                    std::cout << "Invalid move number.\n\n";
                    continue;
                }
                return legalMoves[idx - 1];
            } catch (const std::invalid_argument&) {
                std::cout << "Invalid number.\n\n";
                continue;
            } catch (const std::out_of_range&) {
                std::cout << "Invalid number.\n\n";
                continue;
            }
        }

        // Parse coordinate move like "e2e4"
        if (input.length() != 4) {
            std::cout << "Invalid format! Use 'e2e4' or 'list'\n";
            continue;
        }

        int fromCol = input[0] - 'a';
        int fromRow = 8 - (input[1] - '0');
        int toCol = input[2] - 'a';
        int toRow = 8 - (input[3] - '0');

        if (fromCol < 0 || fromCol >= 8 || fromRow < 0 || fromRow >= 8 ||
            toCol < 0 || toCol >= 8 || toRow < 0 || toRow >= 8) {
            std::cout << "Move out of bounds!\n";
            continue;
        }

        Piece* piece = board.getPiece(fromRow, fromCol);
        if (piece == nullptr) {
            std::cout << "No piece at that square!\n";
            continue;
        }

        if (piece->getColor() != currentPlayer) {
            std::cout << "That's not your piece!\n";
            continue;
        }

        // Keep generated move metadata for castling, en passant, and promotion.
        std::vector<Move> candidates;
        for (const Move& candidate : piece->generateMoves(board)) {
            if (candidate.toRow == toRow && candidate.toCol == toCol) {
                candidates.push_back(candidate);
            }
        }

        if (candidates.empty()) {
            std::cout << "Illegal move! Try again.\n";
            continue;
        }

        if (candidates.front().isPromotion) {
            std::cout << "Promote to (Q/R/B/N): ";
            std::string promotionInput;
            if (!std::getline(std::cin, promotionInput)) {
                gameOver = true;
                return Move();
            }
            if (!promotionInput.empty()) {
                promotionInput[0] = static_cast<char>(
                    std::toupper(static_cast<unsigned char>(promotionInput[0])));
            }

            PieceType promotionType = PieceType::NONE;
            if (promotionInput == "Q") promotionType = PieceType::QUEEN;
            else if (promotionInput == "R") promotionType = PieceType::ROOK;
            else if (promotionInput == "B") promotionType = PieceType::BISHOP;
            else if (promotionInput == "N") promotionType = PieceType::KNIGHT;
            else {
                std::cout << "Invalid promotion piece!\n";
                continue;
            }

            candidates.erase(
                std::remove_if(candidates.begin(), candidates.end(),
                    [promotionType](const Move& candidate) {
                        return !candidate.isPromotion ||
                               candidate.promotionPiece != promotionType;
                    }),
                candidates.end());
        }

        for (const Move& candidate : candidates) {
            if (board.isMoveLegal(candidate, currentPlayer)) {
                return candidate;
            }
        }

        std::cout << "Illegal move (would leave king in check). Try again.\n";
    }
}

bool Game::executeMove(const Move& move) {
    if (!board.makeMove(move)) {
        return false;
    }
    
    moveHistory.push_back(move);
    return true;
}

bool Game::isCheckmate() {
    if (!isCheck()) return false;
    
    std::vector<Move> legalMoves = board.getLegalMoves(currentPlayer);
    return legalMoves.empty();
}

bool Game::isStalemate() {
    if (isCheck()) return false;
    
    std::vector<Move> legalMoves = board.getLegalMoves(currentPlayer);
    return legalMoves.empty();
}

bool Game::isCheck() const {
    return board.isKingInCheck(currentPlayer);
}

bool Game::isDraw() const {
    // Fifty-move rule
    if (board.getHalfMoveClock() >= 100) {
        return true;
    }
    
    return false;
}

void Game::endGame(Color winnerColor) {
    gameOver = true;
    winner = winnerColor;
}

std::string Game::getMoveHistory() const {
    std::string history;
    for (size_t i = 0; i < moveHistory.size(); i++) {
        const Move& m = moveHistory[i];
        
        // Convert to algebraic notation
        char fromFile = 'a' + m.fromCol;
        int fromRank = 8 - m.fromRow;
        char toFile = 'a' + m.toCol;
        int toRank = 8 - m.toRow;
        
        history += fromFile;
        history += std::to_string(fromRank);
        history += toFile;
        history += std::to_string(toRank);
        
        if (m.isPromotion) {
            history += "=";
            history += m.promotionPiece == PieceType::QUEEN ? 'Q' :
                      m.promotionPiece == PieceType::ROOK ? 'R' :
                      m.promotionPiece == PieceType::BISHOP ? 'B' : 'N';
        }
        
        history += " ";
    }
    
    return history;
}

void Game::switchTurns() {
    currentPlayer = (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;
}