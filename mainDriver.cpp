#include "Board.hpp"
#include "CandyStore.hpp"
#include "Player.hpp"
#include "Game.hpp"
using namespace std;

int main(void)
{
    Game game;
    //cout << "game initialized" << endl;
    // cout << "candy stores created" << endl;
    game.loadCandies("candies.txt");
    //cout << "loaded candies" << endl;
    game.createCandyStores();
    game.generateCandyStores();
    game.loadCharacters("characters.txt");
    //cout << "loaded characters" << endl;
    game.loadRiddles("riddles.txt");
    //cout << "loaded riddles" << endl;
    cout << "Welcome to Candyland!" << endl;
    cin.clear();
    game.displayIntro(1);
    cin.clear();
    cin.ignore(10000,'\n');
    game.displayIntro(2);
    int player_to_play = 1;
    bool endGame = false;
    do
    {
        switch(player_to_play)
        {
            case 1:
                game.displayMainMenu(1);
                endGame = game.endGame();
                player_to_play++;
                break;
            case 2:
                game.displayMainMenu(2);
                endGame = game.endGame();
                player_to_play--;
                break;
        }
    } while (!endGame);
}