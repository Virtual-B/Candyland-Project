#include "Player.hpp"
using namespace std;

Player::Player()
{
    Candy candy;
    _name = "";
    _stamina = 0;
    _gold = 0.0;
    _effect = 0;
    for(int i = 0;i < 9;i++)
    {
        candy.name = "";
        candy.description = "";
        candy.price = 0.0;
        candy.candy_type = "";
        _inventory[i] = candy;
    }
    _candy_amount = 0;
}

Player::Player(string name,int stamina,double gold, Candy candy_array[],const int CANDY_ARR_SIZE)
{
    _stamina = stamina;
    _name = name;
    _gold = gold;
    _candy_amount = 0;
    _effect = 0;
    for(int i = 0;i < CANDY_ARR_SIZE;i++)
    {
        if(_inventory[8].name != "" && _inventory[8].description != "" && _inventory[8].candy_type != "" && _inventory[3].price != 0.0)
        {
            break;
        }
        else
        {
            _inventory[i] = candy_array[i];
            _candy_amount++;
            if(candy_array[i].name == "" && candy_array[i].description == "" && candy_array[i].candy_type == "" && candy_array[i].price == 0.0)
            {
                _candy_amount--;
            }
        }
    }
}

int Player::getPlayerNumber()
{
    return _number;
}

void Player::setPlayerNumber(int number)
{
    _number = number;
}

int Player::getEffect()
{
    return _effect;
}

void Player::setEffect(int effect)
{
    _effect = effect;
}

string Player::lowerString(string word)
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

int Player::getCandyAmount()
{
    return _candy_amount;
}
void Player::setStamina(int new_stamina)
{
    if(new_stamina > 100)
    {
        _stamina = 100;
        return;
    }
    _stamina = new_stamina;
}
int Player::getStamina()
{
    return _stamina;
}
void Player::setGold(double new_gold)
{
    if(new_gold > 100)
    {
        _gold = 100;
        return;
    }
    _gold = new_gold;
}
double Player::getGold()
{
    return _gold;
}
void Player::setName(string name)
{
    _name = name;
}
string Player::getName()
{
    return _name;
}
string Player::getCharacter()
{
    return _character_name;
}
void Player::setCharacter(string name)
{
    _character_name = name;
}

void Player::printStats()
{
    cout << "Here are your stats:" << endl << "Player name: " << _name << endl << "Character name: " << _character_name << endl
    << "Stamina: " << _stamina << endl << "Gold: " << _gold << endl << "Candies: " << endl;
    printInventory();
}

void Player::printInventory()
{
    for(int i = 0;i < 9;i++)
    {
        if(_inventory[i].name == "")
        {
            cout << "[Empty] ";
        }
        else
        {
            cout << "[" << _inventory[i].name << "] ";
        }
        if(i == 2 || i == 5 ||  i == 8)
        {
            cout << endl;
        }
    }
}

void Player::printToFile(ofstream& file)
{
    for(int i = 0;i < 9;i++)
    {
        if(_inventory[i].name == "")
        {
            file << "[Empty] ";
        }
        else
        {
            file << "[" << _inventory[i].name << "] ";
        }
        if(i == 2 || i == 5 ||  i == 8)
        {
            file << endl;
        }
    }
}

// void Player::printStats()
// {
//     cout << "Here are your stats:" << endl << "Player name: " << _name << endl << "Character: " << 
// }

Candy Player::findCandy(string candy)
{
    Candy empty_candy;
    empty_candy.name = "";
    empty_candy.description = "";
    empty_candy.price = 0.0;
    empty_candy.candy_type = "";
    for(int i = 0;i < 9;i++)
    {
        if(lowerString(candy) == lowerString(_inventory[i].name))
        {
            return _inventory[i];
        }
    }
    return empty_candy;
}
bool Player::addCandy(Candy candy)
{
    if(getCandyAmount() == 9)
    {
        cout << "You do not have sufficient place in the candy inventory. Would you like to subsitute your candy with an existing candy?(y/n)" << endl;
        char choice;
        cin >> choice;
        while(cin.fail() || (tolower(choice) != 'y' && tolower(choice != 'n')))
        {
            if(cin.fail())
            {
                cin.clear();
                cin.ignore(10000,'\n');
            }
            cout << "Invallid choice, please try again!" << endl;
            cin >> choice;
        }
        printInventory();
        cout << "Which candy would you like to substitute?" << endl;
        string input;
        cin.clear();
        cin.ignore(10000,'\n');
        while(lowerString(findCandy(input).name) != lowerString(input))
        {
            cout << "Invalid candy option! Please try again." << endl;
            cin.clear();
            getline(cin,input);
        }
        removeCandy(input);
        addCandy(candy);
        cout << "Here is your updated inventory: " << endl;
        printInventory();
    }
    if(candy.name == "")
    {
        return true;
    }
    for(int i = 0;i < 9;i++)
    {
        if(_inventory[i].name == "")
        {
            _inventory[i] = candy;
            _candy_amount++;
            return true;
        }
    }
    return false;
}
bool Player::removeCandy(string candy)
{
    Candy empty_candy;
    empty_candy.name = "";
    empty_candy.description = "";
    empty_candy.price = 0.0;
    empty_candy.candy_type = "";
    int index;
    for(int i = 0;i < 9;i++)
    {
        //if lower equal then set index == i for the next loop, to shift all elements to the left by 1
        if(lowerString(_inventory[i].name) == lowerString(candy))
        {
            index = i;
            _candy_amount--;
            break;
        }
    }
    //if index is a value between 0 to 8
    if(index >=0 && index <=8)
    {
        //loop to 3 bc of i+1
        for(int i = index;i < 8;i++)
        {
            _inventory[i] = _inventory[i+1];
        }
        return true;
    }
    return false;
}

// bool Player::useCandy(Candy candy,bool poisoned,Game game)
// {
//     if(!poisoned)
//     {
//         if(candy.effect == "stamina" && candy.effect_value > 0)
//         {
//             _stamina += candy.effect_value;
//             //player.removeCandy(candy.name);
//             return true;
//         }
//         else if(candy.candy_type == "poison")
//         {
//             int new_stamina = candy.effect_value;
//             //player.removeCandy(candy.name);

//             return true;
//         }
//     }
//     else if(poisoned)
//     {
//         for(int i = 0;i < 9;i++)
//         {

//         }
//     }
//     // else if(candy.effect == "other" && candy.candy_type == "immunity")
//     // {
//     //     switch(candy.effect_value)
//     //     {

//     //     }
//     //     return true;
//     // }
//     return false;
// }

// bool Player::substituteCandy(Candy candy)
// {
    
// }