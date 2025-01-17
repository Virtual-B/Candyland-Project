#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include <vector>
#define RED "\033[;41m"     /* Red */
#define GREEN "\033[;42m"   /* Green */
#define BLUE "\033[;44m"    /* Blue */
#define MAGENTA "\033[;45m" /* Magenta */
#define CYAN "\033[;46m"    /* Cyan */
#define ORANGE "\033[48;2;230;115;0m"  /* Orange (230,115,0)*/
#define RESET "\033[0m"

using namespace std;

struct Tile
{
    string color;
    string tile_type;
};


class Board
{
private:
    const static int _BOARD_SIZE = 83;
    Tile _tiles[_BOARD_SIZE];
    const static int _MAX_CANDY_STORE = 3;
    int _candy_store_position[_MAX_CANDY_STORE];
    vector<int> _special_tile_position;
    vector<int> _hidden_treasure_position;
    int _candy_store_count = 0;
    int _special_tile_count = 0;
    int _hidden_treasure_count = 0;
    int _player_position[2];

public:
    Board();

    void resetBoard();
    void displayTile(int);
    void displayBoard();

    bool setPlayerPosition(int,int player);

    int getBoardSize() const;
    int getCandyStoreCount() const;
    int getSpecialTileCount() const;
    int getHiddenTreasureCount() const;
    int getPlayerPosition(int player) const;

    bool addCandyStore(int);
    //bool addSpecialTile(int);
    bool isSpecialTile(int);
    bool addHiddenTreasure(int);
    bool isHiddenTreasure(int);
    bool isPositionCandyStore(int); 

    bool movePlayer(int tile_to_move_forward,int player);
};

#endif