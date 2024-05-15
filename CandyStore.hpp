#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <sstream>
#include "Board.hpp"
#include "Player.hpp"
using namespace std;


#ifndef CANDY_STORE_H
#define CANDY_STORE_H

class Candy_Store
{
    private:
        Candy _candy_store_candies[3];
    public:
        Candy_Store();
        Candy_Store(Candy available_candy[3]);
        bool setCandies(Candy available_candy[3]);
        string lowerString(string);
        bool buyCandy(Player& player);
        // bool addCandy(Candy candy);
        // bool removeCandy(string candy);
        void display_candies();
};
#endif