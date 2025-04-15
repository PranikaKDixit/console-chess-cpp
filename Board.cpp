#include <iostream>

using namespace std;

class Board{
    public:

    char board[8][8];

    Board(){

        for (int i=0;i<8;i++){
            for (int j=0;j<8;j++){
                if (i==1){
                    board[i][j]='P';//white pawn
                }
                else if (i==6){
                    board[i][j]='p';//black pawn
                }
                else if (i==0){
                    if (j==0 || j==7){
                        board[i][j]='R';//white rooks
                    }
                    else if (j==1 || j==6){
                        board[i][j]='N';//white knights
                    }
                    else if (j==2||j==5){
                        board[i][j]='B';//white bishops
                    }
                    else if (j==3){
                        board[i][j]='Q';//white queen
                    }
                    else if (j==4){
                        board[i][j]='K';//white king
                    }
                }

                else if (i==7){
                    if (j==0||j==7){
                        board[i][j]='r';//black rooks
                    }
                    else if (j==1||j==6){
                        board[i][j]='n';//black knights
                    }
                    else if (j==2||j==5){
                        board[i][j]='b';//black bishops
                    }
                    else if (j==3){
                        board[i][j]='q';//black queen
                    }
                    else if (j==4){
                        board[i][j]='k';//black king
                    }
                }

                else{
                    board[i][j]='.';//empty squares
                }
                
            }
        }

    }
    void displayBoard(){
        cout<<" a b c d e f g h \n";

        for (int i=0;i<8;i++){
            cout<<8-i<<" ";

            for (int j=0;j<8;j++){
                cout<<board[i][j]<<" ";
            }
            cout <<8-i<<endl;
        }
        cout<<" a b c d e f g h \n";
    }
};

int main(){
    Board chessBoard;
    chessBoard.displayBoard();

    return 0;
}