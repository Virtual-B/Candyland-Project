#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include <cstring>
#include <string>
#include <cctype>
//#include "utilities.cpp"
using namespace std;

#ifndef PLAYER_H
#define PLAYER_H

struct Candy
{
    string name;
    string description;
    double price;
    string candy_type;
    string effect;
    int effect_value;
};

class Player
{
    private:
        const static int _MAX_CANDY_AMOUNT = 9;
        string _name;
        int _number;
        string _character_name;
        int _stamina;
        double _gold;
        int _effect;
        Candy _inventory[9];
        int _candy_amount;
    public:
        Player();
        Player(string name,int stamina,double gold, Candy candy_array[],const int CANDY_ARR_SIZE);
        string lowerString(string);
        int getPlayerNumber();
        void setPlayerNumber(int number);
        int getEffect();
        void setEffect(int effect);
        int getCandyAmount();
        void setStamina(int new_stamina);
        int getStamina();
        void setGold(double new_gold);
        double getGold();
        void printInventory();
        void printToFile(ofstream&);
        string getName();
        void setName(string);
        string getCharacter();
        void setCharacter(string name);
        Candy findCandy(string candy);
        bool addCandy(Candy candy);
        bool removeCandy(string candy);
        void printStats();
        //bool useCandy(Candy candy,bool poisoned,Game game);
        //bool substituteCandy(Candy candy);
};
#endif
