#include "Player.hpp"
#include "Board.hpp"
#include "CandyStore.hpp"

#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <vector>
#include <array>
using namespace std;

#ifndef GAME_H
#define GAME_H

struct Riddle
{
    string question;
    string answer;
};

struct Character
{
    string character_name;
    int stamina;
    int gold;
    Candy candy_inventory[9];
};

struct Card
{
    string color[3] = {"Marvelous Magenta","Bubblegum Blue","Minty Green"};
    string type = "single";
};
class Game
{
    private:
        Board _board;
        Candy_Store _candyStores[3];
        Player _players[2];
        vector<Candy> _available_candies;
        vector<Character> _available_characters;
        vector<Candy> _magical_candies;
        vector<Candy> _poison_candies;
        vector<Candy> _immunity_candies;
        vector<Candy> _gummy_candies;
        vector<int> _gummy_tiles1;
        vector<int> _gummy_tiles2;
        Card _cards;
        vector<Riddle> _available_riddles;
    public:
        Game();
        Player getPlayer(int player);
        string lowerString(string);

        bool loadCandies(string filename);
        bool loadCharacters(string filename);
        bool loadRiddles(string filename);
        bool charactertoPlayer(int player,string choice);
        void createCandyStores();
        bool generateCandyStores();

        void displayIntro(int player);
        void displayCharacters();
        void displayMainMenu(int player);
       
        bool specialTile(int player,int change);
        void isGummyTile(int player);
        bool isEqual(int player);
        bool hiddenTreasure(int player);
        bool isCalamities(int player);
        bool answerRiddle(int player);
        bool rockPaperScissors(int player);
        bool endGame();
        bool drawCard(int player);
        bool useCandy(int player);
};

#endif