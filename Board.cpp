#include "Board.hpp"

Board::Board()
{
    resetBoard();
}

void Board::resetBoard()
{
    srand(time(0));
    const int COLOR_COUNT = 3;
    const string COLORS[COLOR_COUNT] = {MAGENTA, GREEN, BLUE};
    Tile new_tile;
    string current_color;
    for (int i = 0; i < _BOARD_SIZE - 1; i++)
    {
        current_color = COLORS[i % COLOR_COUNT];
        new_tile = {current_color,"regular tile"};
        int random_chance = (rand()%100)+1;
        if(random_chance >= 1 && random_chance <= 100)
        {
            if(_special_tile_count == 10)
            {
                _tiles[i] = new_tile;
                continue;
            }
            new_tile.tile_type = "special tile";
            _special_tile_position.push_back(i);
            _special_tile_count++;
            //cout << "Test: " << isSpecialTile(i) << endl;
            //cout << "Size: " << _special_tile_count << endl;
        }
        else if(random_chance >= 26 && random_chance <= 31)
        {
            if(_hidden_treasure_count == 5)
            {
                _tiles[i] = new_tile;
                continue;
            }
            //cout << "Hidden treasure created at: " << i << endl;
            new_tile.tile_type = "hidden treasure";
            _hidden_treasure_position.push_back(i);
            _hidden_treasure_count++;
            //cout << "Test: " << isHiddenTreasure(i) << endl;
            //cout << "Size: " << _hidden_treasure_count << endl;
        }
        _tiles[i] = new_tile;
    }
    while(_special_tile_count < 4)
    {
        int random_position = (rand()%80)+1;
        while(isHiddenTreasure(random_position) || isPositionCandyStore(random_position))
        {
            random_position = (rand()%80)+1;
        }
        _special_tile_position.push_back(random_position);
        _special_tile_count++;
        //cout << "Special tile created at: " << random_position << endl;
    }
    while(_hidden_treasure_count < 3)
    {
        int random_position = (rand()%80)+1;
        while(isPositionCandyStore(random_position) || isSpecialTile(random_position))
        {
            random_position = (rand()%80)+1;
        }
        _hidden_treasure_position.push_back(random_position);
        _hidden_treasure_count++;
        //cout << "Hidden treasure created at: " << random_position << endl;
    }
    new_tile = {ORANGE, "regular tile"};
    _tiles[_BOARD_SIZE - 1] = new_tile;

    _candy_store_count = 0;
    for (int i = 0; i < _MAX_CANDY_STORE; i++)
    {
        _candy_store_position[i] = -1;
    }

    _player_position[0] = 0;
    _player_position[1] = 0;
}

void Board::displayTile(int position)
{
    if (position < 0 || position >= _BOARD_SIZE)
    {
        return;
    }
    Tile target = _tiles[position];
    cout << target.color << " ";
    if (position == _player_position[0])
    {
        cout << "1";
    }
    else if(position == _player_position[1])
    {
        cout << "2";
    }
    else
    {
        cout << " ";
    }
    cout << " " << RESET;
}

void Board::displayBoard()
{
    // First horizontal segment
    for (int i = 0; i <= 23; i++)
    {
        displayTile(i);
    }
    cout << endl;
    // First vertical segment
    for (int i = 24; i <= 28; i++)
    {
        for (int j = 0; j < 23; j++)
        {
            cout << "   ";
        }
        displayTile(i);
        cout << endl;
    }
    // Second horizontal segment
    for (int i = 52; i > 28; i--)
    {
        displayTile(i);
    }
    cout << endl;
    // Second vertical segment
    for (int i = 53; i <= 57; i++)
    {
        displayTile(i);
        for (int j = 0; j < 23; j++)
        {
            cout << "   ";
        }
        cout << endl;
    }
    // Third horizontal segment
    for (int i = 58; i < _BOARD_SIZE; i++)
    {
        displayTile(i);
    }
    cout << ORANGE << "Castle" << RESET << endl;
}

bool Board::setPlayerPosition(int new_position,int player)
{
    if (new_position >= 0 && new_position < _BOARD_SIZE)
    {
        _player_position[player-1] = new_position;
        return true;
    }
    return false;
}

int Board::getBoardSize() const
{
    return _BOARD_SIZE;
}

int Board::getCandyStoreCount() const
{
    return _candy_store_count;
}

int Board::getSpecialTileCount() const
{
    return _special_tile_count;
}

int Board::getHiddenTreasureCount() const
{
    return _hidden_treasure_count;
}

int Board::getPlayerPosition(int player) const
{
    return _player_position[player-1];
}

bool Board::addCandyStore(int position)
{
    if (_candy_store_count >= _MAX_CANDY_STORE)
    {
        return false;
    }
    _candy_store_position[_candy_store_count] = position;
    _candy_store_count++;
    return true;
}

bool Board::isPositionCandyStore(int board_position)
{
    for (int i = 0; i < _candy_store_count; i++)
    {
        if(_candy_store_position[i] == board_position)
        {
            return true;
        }
    }
    return false;
}

bool Board::addHiddenTreasure(int position)
{
    if(isPositionCandyStore(position) || isSpecialTile(position))
    {
        return false;
    }
    _hidden_treasure_position.push_back(position);
    _hidden_treasure_count++;
    return true;
}

bool Board::isHiddenTreasure(int board_position)
{
    for(int i = 0;i < _hidden_treasure_count;i++)
    {
        if(_hidden_treasure_position[i] == board_position)
        {
            return true;
        }
    }
    return false;
}

bool Board::isSpecialTile(int board_position)
{
    for(int i = 0;i < _special_tile_count;i++)
    {
        if(_special_tile_position[i] == board_position)
        {
            return true;
        }
    }
    return false;
}

bool Board::movePlayer(int tile_to_move_forward,int player)
{
    int new_player_position = tile_to_move_forward + _player_position[player-1];
    if(new_player_position < 0 )
    {
        return false;
    }
    else if (new_player_position >= _BOARD_SIZE)
    {
        _player_position[player-1] = _BOARD_SIZE-1;
    }
    
    _player_position[player-1] = new_player_position;
    // cout << _player_position[player-1];
    return true;
}