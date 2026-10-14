// include/Game.h
#pragma once

#include "Board.h"
#include <vector>
#include <string>

class Game {
private:
    Board board;
    Color currentPlayer;
    std::vector<Move> moveHistory;
    bool gameOver;
    Color winner;  // NONE if draw
    
public:
    Game();
    
    void start();
    void displayStatus() const;
    Move getPlayerMove();
    bool executeMove(const Move& move);
    
    bool isCheckmate();
    bool isStalemate();
    bool isCheck() const;
    bool isDraw() const;
    bool isGameOver() const { return gameOver; }
    
    void endGame(Color winner);
    std::string getMoveHistory() const;
    
    Color getCurrentPlayer() const { return currentPlayer; }
    void switchTurns();
};