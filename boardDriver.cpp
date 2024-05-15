#include "Board.hpp"
#include <iostream>

using namespace std;

int main()
{
    Board board;
    // board.displayBoard();
    //cout << board.movePlayer(5,1) << endl;
    //board.movePlayer(5,1);
    board.movePlayer(2,1);
    board.movePlayer(5,2);
    cout << board.getPlayerPosition(2)%3 << endl;
    board.displayBoard();
    return 0;
};