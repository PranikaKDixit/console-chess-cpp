#include <iostream>
#include "board.cpp"

using namespace std;

class Board{
    public:
        char board[8][8];

        Board();
        void displayBoard();
};

int main(){
    Board chessBoard;
    chessBoard.displayBoard();

    return 0;
}