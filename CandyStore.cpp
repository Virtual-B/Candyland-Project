#include "CandyStore.hpp"
#include "Game.hpp"
using namespace std;

//display candies helper function
void printStore(Candy candy)
{
    cout << "Name: " << candy.name << endl << "Description: " << candy.description << endl << "Effect: " << candy.effect << endl << "Effect value: " << candy.effect_value
    << endl << "Candy type: " << candy.candy_type << endl << "Price: " << candy.price << endl;
}

Candy_Store::Candy_Store()
{
    _candy_store_candies;
}
// Candy_Store::Candy_Store(Candy available_candy[3])
// {
//     for(int i = 0;i < 3;i++)
//     {
//         _candy_store_candies[i] = available_candy[i];
//     }
// }

bool Candy_Store::setCandies(Candy candies[3])
{
    for(int i = 0;i < 3;i++)
    {
        _candy_store_candies[i] = candies[i];
    }
    return true;
}

string Candy_Store::lowerString(string word)
{
    string new_string = word;
    int len = word.length();
    for(int i = 0;i < len;i++)
    {
        if(isupper(word[i]))
        {
            new_string[i] = tolower(word[i]);
        }
    }
    return new_string;
}

bool Candy_Store::buyCandy(Player& player)//not done, need some way to test first
{
    char choice;
    string selection;
    cout << "Do you want to visit the candy store?(y/n)" << endl;
    cin >> choice;
    while(cin.fail() || (choice != 'y' && choice != 'n'))
    {
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(10000,'\n');
        }
        cout << "Invalid input! Please try again." << endl;
        cin >> choice;
    }
    switch(choice)
    {
        case 'y':
            display_candies();
            cout << "Which candy would you like to buy?" << endl;
            cin.clear();
            cin.ignore(10000,'\n');
            getline(cin,selection);
            while(lowerString(selection) != lowerString(_candy_store_candies[0].name) && lowerString(selection) != lowerString(_candy_store_candies[1].name) && lowerString(selection) != lowerString(_candy_store_candies[2].name))
            {
                cout << "Invalid selection! Please try again" << endl;
                cin.clear();
                getline(cin,selection);
            }
            for(int i = 0;i < 3;i++)
            {
                if((lowerString(selection) == lowerString(_candy_store_candies[i].name)) && (player.getGold() >= _candy_store_candies[i].price))
                {
                    // if(player.getCandyAmount() == 9)
                    // {
                    //     //player.substituteCandy(_candy_store_candies[i]);
                    //     break;
                    // }
                    player.addCandy(_candy_store_candies[i]);
                }
                else if((lowerString(selection) == lowerString(_candy_store_candies[i].name)) && (player.getGold() < _candy_store_candies[i].price))
                {
                    cout << "Not enough gold!" << endl;
                }
            }
            return true;
        case 'n':
            cout << "Declined offer!" << endl;
            cin.clear();
            break;
    }
    return false;
}

// bool Candy_Store::addCandy(Candy candy)
// {
//     for(int i = 0;i < 3;i++)
//     {
//         if(_candy_store_candies[i].name == "")
//         {
//             _candy_store_candies[i] = candy;
//             return true;
//         }
//     }
//     return false;
// }
// bool Candy_Store::removeCandy(string candy)
// {
//     Candy empty_candy;
//     empty_candy.name = "";
//     empty_candy.description = "";
//     empty_candy.price = 0.0;
//     empty_candy.candy_type = "";
//     int index;
//     for(int i = 0;i < 4;i++)
//     {
//         //if lower equal then set index == i for the next loop, to shift all elements to the left by 1
//         if(lowerString(_candy_store_candies[i].name) == lowerString(candy))
//         {
//             index = i;
//             break;
//         }
//     }
//     //if index is a value between 0 to 3
//     if(index >=0 && index <=2)
//     {
//         //loop to 3 bc of i+1
//         for(int i = index;i < 2;i++)
//         {
//             _candy_store_candies[i] = _candy_store_candies[i+1];
//         }
//         return true;
//     }
//     return false;
// }

void Candy_Store::display_candies()
{
    cout << "Here is a list of candies in the candy store:" << endl;
    for(int i = 0;i < 3;i++)
    {
        printStore(_candy_store_candies[i]);
        cout << "--------------------------------------------------------" << endl;
    }
}