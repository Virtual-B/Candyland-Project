#include "Game.hpp"
using namespace std;
//RE EDIT FUNCTIONS TO USE GETLINES
//robber's repel stuff

Game::Game()
{
    // cout << _board.getHiddenTreasureCount() << endl;
    // cout << _board.getSpecialTileCount() << endl;
}

string Game::lowerString(string word)
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

Player Game::getPlayer(int player)//return the player based on parameter
{
    return _players[player-1];
}

bool Game::loadCharacters(string filename)//load characters from a given file
{
    ifstream input(filename);//file stream
    string temp,line;//variables to be used for streams
    Character empty_character;//temp character
    string candies[9] = {"","","","","","","","",""};
    int count = 0;
    vector<Character> characters;//vector to push back into and return
    int len = _available_candies.size();//variable to iterate and match candies
    if(len == 0)//if candies not loaded, prompt the user to try again
    {
        cout << "No Available Candies! Please load candies from an available file!" << endl;
        return false;
    }
    if(input.fail())//file validation
    {
        cout << "File not found!" << endl;
        return false;
    }
    while(getline(input,line))//read every line in the fle until no more lines to read
    {
        if(line.length() == 0)
        {
            continue;
        }
        stringstream ss(line);//turn each line into a ss
        count = 0;//count for iteration
        while(getline(ss,temp,'|'))//delimiter for each case/iteration
        {
            switch(count)//switch statement for each count and iteration
            {
                case 0:
                    empty_character.character_name = temp;
                    break;
                case 1:
                    empty_character.stamina = stoi(temp);
                    break;
                case 2:
                    empty_character.gold = stod(temp);
                    break;
                case 3:
                    stringstream ss2(temp);//new ss to read through candies
                    int iterator = 0;
                    while(getline(ss2,temp,','))
                    {
                        candies[iterator] = temp;
                        iterator++;
                    }
                    for(int i = 0;i < 9;i++)
                    {
                        for(int j = 0;j < len;j++)
                        {
                            if(lowerString(candies[i]) == lowerString(_available_candies[j].name))
                            {
                                empty_character.candy_inventory[i] = _available_candies[j];
                            }
                        }
                    }
                    _available_characters.push_back(empty_character);
                    break;
            }
            count++;
        }
    }
    input.close();
    return true;
}

bool Game::charactertoPlayer(int player,string choice)//Take a player(int) based on player 1 or 2, then their character of choice
{
    vector<Character> temp_characters;
    for(int i = 0;i < _available_characters.size();i++)//loop
    {
        if(lowerString(choice) == lowerString(_available_characters.at(i).character_name))//if the lower case version of input matches an element in the vector
        {
            _players[player-1].setCharacter(_available_characters.at(i).character_name);//set player stats to character
            _players[player-1].setGold(_available_characters.at(i).gold);
            _players[player-1].setStamina(_available_characters.at(i).stamina);
            _players[player-1].setEffect(0);
            for(int j = 0;j < 9;j++)
            {
                _players[player-1].addCandy(_available_characters.at(i).candy_inventory[j]);
            }
            for(int c = 0;c < _available_characters.size();c++)//to get rid of chosen character
            {
                if(lowerString(_available_characters.at(c).character_name) != lowerString(choice))//if no match, push back
                {
                    temp_characters.push_back(_available_characters.at(c));
                }
                else
                {
                    continue;
                }
            }
            _available_characters = {};//make empty to push back into
            for(int k = 0;k < temp_characters.size();k++)
            {
                _available_characters.push_back(temp_characters.at(k));//push back to update available characters with new values
            }
            //cout << "Player candy size : " << _players[player-1].getCandyAmount() << endl;
            return true;
        }
        else
        {
            continue;//next iteration if no match
        }
    }
    return false;// if match not found then return false
}

bool Game::loadCandies(string filename)//load candies from a file
{
    ifstream file(filename);//ifstream for the file parameter
    string temp,line;//string variables to read
    Candy empty_candy;//temp candy to copy
    int count = 0; //for iteration
    if(file.fail())//if file not found
    {
        cout << "Failed to open file" << endl;
        return false;
    }
    while(getline(file,line))//while there are lines to read
    {
        if(line.length() == 0)//if line is empty, continue to the next iteration
        {
            continue;
        }
        stringstream ss(line);
        count = 0;//count for iteration/switch cases, resets with each line
        while(getline(ss,temp,'|'))//using | as a delimiter, read a string stream
        {
            switch(count)
            {
                case 0://candy name
                    empty_candy.name = temp;
                    break;
                case 1://candy desc
                    empty_candy.description = temp;
                    break;
                case 2://candy effect type
                    empty_candy.effect = temp;
                    break;
                case 3://candy effect value
                    empty_candy.effect_value = stoi(temp);
                    break;
                case 4://candy type
                    empty_candy.candy_type = temp;
                    break;
                case 5://candy price
                    empty_candy.price = stod(temp);
                    break;
            } 
            count++;
        }
        _available_candies.push_back(empty_candy);//push back the temp candy
    }
    for(int i = 0;i < _available_candies.size();i++)//loop to match the new candies into appropiate genres for candy store
    {
        if(_available_candies[i].candy_type == "magical")
        {
            _magical_candies.push_back(_available_candies[i]);
        }
        else if(_available_candies[i].candy_type == "poison")
        {
            _poison_candies.push_back(_available_candies[i]);
        }
        else if(_available_candies[i].candy_type == "immunity")
        {
            _immunity_candies.push_back(_available_candies[i]);
        }
        else if(_available_candies[i].candy_type == "gummy")
        {
            _gummy_candies.push_back(_available_candies[i]);
        }
        else
        {
            continue;
        }
    }
    Candy robber_repel;
    robber_repel.name = "Robber's Repel";
    robber_repel.candy_type = "immunity";
    robber_repel.description = "Prevents swiping";
    robber_repel.price = 0;
    robber_repel.effect = "tile";
    robber_repel.effect_value = -1;
    _immunity_candies.push_back(robber_repel);
    file.close();
    return true;
}

bool Game::loadRiddles(string filename)//load riddles from a given file
{
    ifstream file(filename);//ifstream the file
    string temp,line;//temp variables to use for riddles
    Riddle empty_riddle;//temp riddle variable
    if(file.fail())//if file does not open 
    {
        cout << "Failed to open file" << endl;
        return false;
    }
    while(getline(file,line))//while lines can be taken
    {
        if(line.length() == 0)//if empty line, continue
        {
            continue;
        }
        int count = 0;// count for iteration
        stringstream ss(line);
        while(getline(ss,temp,'|'))//using a delimiter for the line to get question and answer
        {
            switch(count)
            {
                case 0:
                    empty_riddle.question = temp;
                    break;
                case 1:
                    empty_riddle.answer = temp;
                    break;
            }
            count++;
        }
        _available_riddles.push_back(empty_riddle);//push back each riddle before taking a new line
    }
    file.close();
    return true;
}  

void Game::displayCharacters()//display the available characters that are stored in the game class
{
    cout << "Awesome! Here is a list of charaters a player can select from:" << endl;
    int length = _available_characters.size();
    for(int i = 0;i < length;i++)//for each character in available characters
    {
        cout << "Name: " <<  _available_characters[i].character_name << endl;//print stats of each character
        cout << "Stamina: " << _available_characters[i].stamina << endl;
        cout << "Gold: " << _available_characters[i].gold << endl;
        cout << "Candies: " << endl;
        for(int j = 0;j < 9;j++)
        {
            if(_available_characters.at(i).candy_inventory[j].name == "")
            {
                cout << "[Empty] ";
            }
            else
            {
                cout << "[" << _available_characters.at(i).candy_inventory[j].name << "] ";
            }
            if(j == 2 || j == 5 || j == 8)
            {
                cout << endl;
            }
        }
        cout << "--------------------------------------------" << endl;
    }
}

void Game::displayMainMenu(int player)//complete
{
    int choice = 0;//choice to cin 
    bool check_condition = false;
    int new_effect = 0;
    cout << endl << "It's " << _players[player-1].getName() << "'s turn" << endl;
    if(_players[player-1].getStamina() == 0)
    {
        cout << "Player " << _players[player-1].getName() << " is out of stamina and must be skipped for two turns!" << endl;
        _players[player-1].setEffect(100);
        return;
    }
    if(_players[player-1].getEffect() == 100)
    {
        cout << "Player " << _players[player-1].getName() << " is out of stamina and must be skipped for one more turn!" << endl;
        _players[player-1].setEffect(0);
        return;
    }
    if(_players[player-1].getEffect() > 0)
    {
        cout << "Player " << _players[player-1].getName() << " must be skipped because of a previous effect!" << endl;
        new_effect = _players[player-1].getEffect() - 1;
        _players[player-1].setEffect(new_effect);
        return;
    }
    do
    {
        cout << "Please select a menu option:" << endl << "1. Draw card" << endl << "2. Use candy" << endl << "3. Show player stats" << endl;
        cin >> choice;
        while(cin.fail() || choice < 1 || choice > 3)//choice validation
        {
            if(cin.fail())
            {
                cin.clear();
                cin.ignore(10000,'\n');
            }
            cout << "Invalid choice, please enter a valid option!" << endl;
            cin >> choice;
        }
        switch(choice)//switch statement for the menu
        {
            case 1:
                cout << "Draw a card!" << endl;
                drawCard(player);//draw card
                check_condition = true;
                break;
            case 2:
                cout << "Use a candy" << endl;//use candy
                if(useCandy(player))
                {
                    check_condition = true;
                    _board.displayBoard();
                }
                else
                {
                    check_condition = false;
                }
                break;
            case 3:
                cout << "Show player stats!" << endl;
                _players[player-1].printStats();//printing stats
                break;
        }    
    }while(!check_condition);//make sure turn occurs correctly
    _players[player-1].setStamina(_players[player-1].getStamina()-1);
}

void Game::displayIntro(int player)//displaying intro to set names
{
    string input;
    Candy_Store store;
    bool randomized = false;
    int random_all = rand()%_available_candies.size();//random numbers
    int random_all2 = rand()%_available_candies.size();
    int random_all3 = rand()%_available_candies.size();
    while(!randomized)//while loop to keep checking if the values are going to be the same
    {
        if(random_all != random_all2 && random_all != random_all3 && random_all2 != random_all3)//check for repeats
        {
            randomized = true;
        }
        else if(random_all == random_all2 || random_all == random_all3)
        {
            random_all = rand()%_available_candies.size();
        }
        else if(random_all2 == random_all3)
        {
            random_all2 = rand()%_available_candies.size();
        }
    }
    Candy candy_to_use[3] = {_available_candies[random_all],_available_candies[random_all2],_available_candies[random_all3]};
    store.setCandies(candy_to_use);
    cout << "Enter Player Name: " << endl;
    cin.clear();
    //cin.ignore(10000,'\n');
    getline(cin,input);
    //cin.ignore(10000,'\n');
    _players[player-1].setName(input);
    displayCharacters();
    cout << "The selected character is " << endl;
    cin.clear();
    getline(cin,input);
    //cin.ignore(10000,'\n');
    bool matched = false;
    while(!matched)
    {
        for(int i = 0;i < _available_characters.size();i++)
        {
            if(lowerString(input) == lowerString(_available_characters.at(i).character_name))
            {
                matched = true;
                break;
            }
        }
        if(matched)
        {
            break;
        }
        else
        {
            cout << "Invalid selection! Please enter a valid player!" << endl;
            cin.clear();
            getline(cin,input);
        }
    }
    charactertoPlayer(player,input);
    store.buyCandy(_players[player-1]);
    return;
}

void Game::createCandyStores()//creating candy stores into the array
{
    srand(time(0));//srand to get random assigned variables for the assigned candies
    for(int i = 0;i < 3;i++)//loop 3 times to create all 3
    {
        bool randomized = false;
        int random_all = rand()%_available_candies.size();//random numbers
        int random_all2 = rand()%_available_candies.size();
        int random_all3 = rand()%_available_candies.size();
        while(!randomized)//while loop to keep checking if the values are going to be the same
        {
            if(random_all != random_all2 && random_all != random_all3 && random_all2 != random_all3)//check for repeats
            {
                randomized = true;
            }
            else if(random_all == random_all2 || random_all == random_all3)
            {
                random_all = rand()%_available_candies.size();
            }
            else if(random_all2 == random_all3)
            {
                random_all2 = rand()%_available_candies.size();
            }
        }
        Candy candy_to_use[3] = {_available_candies[random_all],_available_candies[random_all2],_available_candies[random_all3]};
        _candyStores[i].setCandies(candy_to_use);
        //cout << "Candy store created!" << endl;
    }
    // for(int i = 0;i < 3;i++)
    // {
    //     _candyStores[i].display_candies();
    // }
    return;
}

bool Game::generateCandyStores()//generating candy stores on the board tiles
{
    srand(time(0));
    int random_tile1 = (rand()%27);//0-26 tiles
    bool random1 = false;
    int random_tile2 = (rand()%27)+28;//27-53
    bool random2 = false;
    int random_tile3 = (rand()%27)+55;//54-81
    bool random3 = false;
    //using while loops and booleans to check if tile color matches up
    while(!random1)
    {
        if(random_tile1%3 != 0 || _board.isSpecialTile(random_tile1))
        {
            random_tile1 = rand()%27;
        }
        else
        {
            random1 = true;
            _board.addCandyStore(random_tile1);
            //cout << "Candy Store at " << random_tile1 << endl;
        }
    }
    while(!random2)
    {
        if(random_tile2%3 != 1 || _board.isSpecialTile(random_tile2))
        {
            random_tile2 = (rand()%27)+28;
        }
        else
        {
            random2 = true;
            _board.addCandyStore(random_tile2);
            //cout << "Candy store at " << random_tile2 << endl;
        }
    }
    while(!random3)
    {
        if(random_tile3%3 != 2 || _board.isSpecialTile(random_tile3))
        {
            random_tile3 = (rand()%27)+55;
        }
        else
        {
            random3 = true;
            _board.addCandyStore(random_tile3);
            //cout << "Candy store at " << random_tile3 << endl;
        }
    }
    return true;
}

bool Game::specialTile(int player,int change)//if special tile call this fcn
{
    srand(time(0));
    int random_event = (rand()%100)+1;
    if(random_event >= 1 && random_event <= 25)//even chance for all
    {
        cout << "You encountered Shortcut Tile! You advance four tiles!" << endl;
        _board.movePlayer(4,player);
        _board.displayBoard();
        return true;
    }
    else if(random_event >= 26 && random_event <= 50)//with another turn display menu again
    {
        cout << "You encountered Ice Cream Stop Tile! You get another turn!" << endl;
        displayMainMenu(player);
        return true;
    }
    else if(random_event >= 51 && random_event <= 75)//gumdrop forest tile
    {
        int random_stamina = -1 * ((rand()%6)+5);//multiply by -1 to get negative value
        _board.movePlayer(-4,player);
        cout << "You encountered Gumdrop Forest Tile! You lose four tiles of progress and " << random_stamina << " stamina!" << endl;
        random_stamina = _players[player-1].getStamina() + random_stamina;
        _players[player-1].setStamina(random_stamina);
        _board.displayBoard();
        return true;
    }
    else if(random_event >= 76 && random_event <= 100)
    {
        change *= -1;//multiply by -1 to get a negative value
        _board.movePlayer(change,player);
        cout << "You landed on a Gingerbread House Tile! You will move back " << change << " tiles and lose one of your available immunity candies!" << endl;
        for(int i = 0;i < _immunity_candies.size();i++)
        {
            if(_players[player-1].findCandy(_immunity_candies[i].name).name != "")//if an immunity candy is found
            {
                cout << "You have lost " << _immunity_candies[i].name << endl;
                _players[player-1].removeCandy(_immunity_candies[i].name);
                break;
            }
            else if(i == _immunity_candies.size()-1 && _players[player-1].findCandy(_immunity_candies[i].name).name == "")
            {
                cout << "You have no immunity candies to lose!" << endl;
            }
        }
        _board.displayBoard();
        return true;
    }
    return false;
}

bool Game::hiddenTreasure(int player)//not complete
{
    srand(time(0));//seeded rand
    // int random_chance = (rand()%100)+1;//chance from 0-100
    // cout << random_chance << endl;
    int random_event = (rand()%100)+1;//chance from 0 -100
    //cout << random_event << endl;
    cout << "You found a hidden treasure! Answer this riddle to unlock its secrets!" << endl;
    if(!answerRiddle(player))
    {
        return false;
    }
    if(random_event >=1 && random_event <= 30)//30% chance of stamina refill
    {
        int random_stamina = (rand()%20)+11;
        cout << "Your stamina has been increased by " << random_stamina << " thanks to Stamina Refill!" << endl;
        random_stamina = _players[player-1].getStamina() + random_stamina;
        _players[player-1].setStamina(random_stamina);
        return true;
    }
    else if(random_event >= 31 && random_event <= 40)//10% chance of gold windfall
    {
        int random_gold = (rand()%20)+21;
        cout << "Your gold has been increased by " << random_gold << " thanks to Gold Windfall!" << endl;
        return true;
    }
    else if(random_event >= 41 && random_event <= 70)//30% chance of robber repel
    {
        Candy robber_repel;
        robber_repel.name = "Robber's Repel";
        robber_repel.candy_type = "immunity";
        robber_repel.description = "Prevents swiping";
        robber_repel.price = 0;
        robber_repel.effect = "tile";
        robber_repel.effect_value = -1;
        cout << "You earned a Robber's Repel candy!" << endl;
        _players[player-1].addCandy(robber_repel);
        return true;
    }
    else if(random_event >= 71 && random_event <= 100)//30% chance of random candy
    {
        int random_candy = (rand()%100)+1;
        if(random_candy >= 1 && random_candy <= 70)//70% chance of jellybean of vigor
        {
            Candy jellybean;
            jellybean.name = "Jellybean of Vigor";
            jellybean.candy_type = "magical";
            jellybean.description = "flat 50 increase of stamina";
            jellybean.price = 0;
            jellybean.effect = "stamina";
            jellybean.effect_value = 50;
            cout << "You earned a Jellybean of Vigor Candy!" << endl;
            _players[player-1].addCandy(jellybean);
            return true;
        }
        else if(random_candy >= 71 && random_candy <= 100)//30% chance of truffle
        {
            Candy truffle;
            truffle.name = "Treasure Hunter's Truffle";
            truffle.candy_type = "special";
            truffle.description = "Chance to unlock a hidden treasure";
            truffle.price = 0;
            truffle.effect = "treasure";
            truffle.effect_value = 0;
            cout << "Treasure Hunter's Truffle" << endl;
            _players[player-1].addCandy(truffle);
            return true;
        }
    }
    return false;
}

bool Game::isCalamities(int player)//completed calamities checker for whenever player moves
{
    srand(time(0));
    int calamity_chance = (rand()%100)+1;//1-100 random for chance
    if(calamity_chance >= 1 && calamity_chance <= 40)
    {
        int calamity = (rand()%100)+1;
        if(calamity >= 1 && calamity <= 30)//30% chance
        {
            cout << "Oh no! Candy bandits have swiped your gold coins!" << endl;
            int gold_to_take = (rand()%10)+1;
            int new_gold = _players[player-1].getGold() - gold_to_take;
            _players[player-1].setGold(new_gold);
        }
        else if(calamity >= 31 && calamity <= 65)//35% chance
        {
            cout << "Oh dear! You got lost in the lollipop labyrinth!" << endl;
            if(!rockPaperScissors(player))
            {
                int turns_to_skip = _players[player-1].getEffect() + 1;
                _players[player-1].setEffect(turns_to_skip);
                cout << "Lost! You miss a turn!" << endl;
            }
            else
            {
                cout << "Won! You regained your lost turn!" << endl;
            }
        }
        else if(calamity >= 66 && calamity <= 80)//15% chance
        {
            cout << "Watch out! A candy avalanche has struck!" << endl;
            int stamina_to_take = (rand()%6)+5;
            int new_stamina = _players[player-1].getStamina() - stamina_to_take;
            if(!rockPaperScissors(player))
            {
                int turns_to_skip = _players[player-1].getEffect() + 1;
                _players[player-1].setEffect(turns_to_skip);
                _players[player-1].setStamina(new_stamina);
                cout << "Lost! You miss a turn and lost " << stamina_to_take << " stamina!"  << endl;
            }
            else
            {
                cout << "Won! You regained your lost turn and stamina!" << endl;
            }
        }
        else if(calamity >= 81 && calamity <= 100)//20% chance
        {
            cout << "Oops you're stuck in a sticky taffy trap!" << endl;
            for(int i = 0;i < _magical_candies.size();i++)
            {
                if(_players[player-1].findCandy(_magical_candies.at(i).name).name != "")
                {
                    _players[player-1].removeCandy(_magical_candies.at(i).name);
                    cout << "You used " << _magical_candies.at(i).name << " to escape the trap" << endl;
                    break;
                }
                else if(_players[player-1].findCandy(_magical_candies.at(i).name).name == "" && i == _magical_candies.size()-1)\
                {
                    int turns_to_skip = _players[player-1].getEffect() + 1;
                    _players[player-1].setEffect(turns_to_skip);
                    cout << "You do not have a magical candy to escape! You have lost a turn!" << endl;
                    break;
                }
            }

        }
        return true;
    }
    return false;
}

bool Game::answerRiddle(int player)//answering a riddle to unlock a hidden treasure
{
    srand(time(0));
    // cout << "Test before rand" << endl;
    // int len = _available_riddles.size();
    int randomRiddle = (rand()%_available_riddles.size());
    // cout << "Test after rand" << endl;
    // cout << _available_riddles.size() << endl;
    Riddle riddle = _available_riddles.at(randomRiddle);
    string answer;
    cout << riddle.question << endl;
    cin.clear();
    cin.ignore(10000,'\n');
    getline(cin,answer);
    if(answer != riddle.answer)
    {
        cout << "You answered incorrectly! You lose your chance at a hidden treasure!" << endl;
        return false;
    }
    cout << "You answered correctly! You win a chance at a hidden treasure!" << endl;
    return true;
}

bool Game::rockPaperScissors(int player)//rps for calamities
{
    srand(time(0));//seeded random
    char choice,choice_1;
    int winner = -1;
    cout << "Player " << player << ": Enter r, p, or s" << endl;//Player choice
    cin >> choice;
    while(cin.fail() || (choice != 's' && choice != 'r' && choice != 'p'))
    {
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(10000,'\n');
        }
        cout << "Invalid selection!" << endl;
        cin.clear();
        cin >> choice;
    }
    //CPU s,r,p
    int cpu_choice = rand()%3;
    if(cpu_choice == 0)
    {
        choice_1 = 'r';
    }
    else if(cpu_choice == 1)
    {
        choice_1 = 's';
    }
    else if(cpu_choice == 2)
    {
        choice_1 = 'p';
    }
    //cout << choice_1 << endl;
    //check if tie
    while(choice == choice_1)
    {
        cout << "Tie! Play again" << endl;
        cout << "Player " << player << ": Enter r, p, or s" << endl;
        cin >> choice;
        while(cin.fail() || !(choice == 's' || choice == 'r' || choice == 'p'))
        {
            if(cin.fail())
            {
                cin.clear();
                cin.ignore(10000,'\n');
            }
            cout << "Invalid selection!" << endl;
            cin >> choice;
        }
        cpu_choice = rand()%3;
        if(cpu_choice == 0)
        {
            choice_1 = 'r';
        }
        else if(cpu_choice == 1)
        {
            choice_1 = 's';
        }
        else if(cpu_choice == 2)
        {
            choice_1 = 'p';
        }
    }
    //cout << choice_1 << endl;
    //Switch cases for win condition
    switch(choice)
    {
        case 'r':
            switch(choice_1)
            {
                case 'p':
                    winner = 1;
                    break;
                case 's':
                    winner = 0;
                    break;
            }
            break;
        case 's':
            switch(choice_1)
            {
                case 'r':
                    winner = 1;
                    break;
                case 'p':
                    winner = 0;
                    break;
            }
            break;
        case 'p':
            switch(choice_1)
            {
                case 'r':
                    winner = 0;
                    break;
                case 's':
                    winner = 1;
                    break;
            }
            break;
    }
    //outcome
    if(winner == 0)
    {
        return true;
    }
    return false;
}

void Game::isGummyTile(int player)//will check if the player lands on a gummy tile
{
    for(int i = 0;i < _gummy_tiles1.size();i++)
    {
        if(_board.getPlayerPosition(player) == _gummy_tiles1[i])
        {
            int turns_to_skip = _players[player-1].getEffect() + 1;
            _players[player-1].setEffect(turns_to_skip);
            cout << _players[player-1].getName() << endl;
            cout << "You landed on a gummy tile! You have to skip " << turns_to_skip << " turns!" << endl;
            return;
        }
    }
    for(int i = 0;i < _gummy_tiles2.size();i++)
    {
        int turns_to_skip = _players[player-1].getEffect() + 2;
        _players[player-1].setEffect(turns_to_skip);
        cout << _players[player-1].getName() << endl;
        cout << "You landed on a gummy tile! You have to skip " << turns_to_skip << " turns!" << endl;
        return;
    }
    return;
}

bool Game::isEqual(int player)//check if player positions are equal if statement with 2, nested loop with multi
{
    int position1 = _board.getPlayerPosition(1);
    int position2 = _board.getPlayerPosition(2);
    int random_gold1;
    int random_gold;
    if(position1 == position2)
    {
        cout << "You landed on a tile with a player already on it!" << endl;
        switch(player)
        {
            case 1:
                random_gold1 = -1 * (rand()%26)+5;
                if(_players[player-1].findCandy("Robber's Repel").name != "")
                {
                    cout << "You used a Robber's Repel to send your opponent back one tile and prevent being robbed!" << endl;
                    _players[player-1].removeCandy("Robber's Repel");
                    _board.movePlayer(-1,2);
                }
                else
                {
                    int gold_sub = _players[player-1].getStamina() + random_gold1;
                    int gold_add = _players[1].getStamina() + (-1*random_gold1);
                    _players[player-1].setStamina(gold_sub);
                    _players[1].setStamina(gold_add);
                    cout << "You were robbed of " << random_gold1 << " gold!" << endl;
                }
                break;
            case 2:
                random_gold = -1 * (rand()%26)+5;
                if(_players[player-1].findCandy("Robber's Repel").name != "")
                {
                    cout << "You used a Robber's Repel to send your opponent back one tile and prevent being robbed!" << endl;
                    _players[player-1].removeCandy("Robber's Repel");
                    _board.movePlayer(-1,2);
                }
                else
                {
                    int gold_sub = _players[player-1].getStamina() + random_gold;
                    int gold_add = _players[0].getStamina() + (-1*random_gold);
                    _players[player-1].setStamina(gold_sub);
                    _players[0].setStamina(gold_add);
                    cout << "You were robbed of " << random_gold << " gold!" << endl;
                }
                break;
        }
        return true;
    }
    return false;
}

bool Game::endGame()//Checking if a player is at the castle tile
{
    for(int i = 1;i < 3;i++)
    {
        if(_board.getPlayerPosition(i) >= 82)
        {
            cout << "Congratulations! Player " << _players[i-1].getName() << " has won the game and reached the castle!" << endl; 
            ofstream results("results.txt");
            for(int j = 0;j < 2;j++)
            {
                results << "These are the stats for the game!" << endl;
                results << "Player name: " << _players[j].getName() << endl;
                results << "Character name: " << _players[j].getCharacter() << endl;
                results << "Gold: " << _players[j].getGold() << endl;
                results << "Stamina: " << _players[j].getStamina() << endl;
                _players[j].printToFile(results);
                results << "---------------------------------------------------" << endl;
            }
            return true;
        }
    }
    return false;
}

bool Game::drawCard(int player)//draw a card and move resepective player accordingly
{
    srand(time(0));//seeded random
    int random_color = rand()%3;//random int from 0 to 3 given 3 colors of card in struct
    int random_rare = rand()%100;//random chance given 0-99
    if(random_rare >= 0 && random_rare <= 24)//25% chance of double
    {
        _cards.type = "double";
    }
    _cards.color[random_color];
    //if statements given the card drawn and position of the player, and moving the player accordingly
    if(_board.getPlayerPosition(player)%3 == 0 && _cards.color[random_color] == "Marvelous Magenta")
    {
        if(_cards.type == "single")
        {
            cout << "You drew a single Marvelous magenta! Your game piece advances to Magenta tile. Here is the updated trail:" << endl;
            _board.movePlayer(3,player);
            _board.displayBoard();
            int position = _board.getPlayerPosition(player);
            isEqual(player);
            if(_board.isPositionCandyStore(position))
            {
                cout << "You landed on a candy store!" << endl;
                if(position >= 0 && position <= 26)
                {
                    _candyStores[0].buyCandy(_players[player-1]);
                }
                else if(position >= 27 && position <= 53)
                {
                    _candyStores[1].buyCandy(_players[player-1]);
                }
                else
                {
                    _candyStores[2].buyCandy(_players[player-1]);
                }
                //_candyStores
            }
            else if(_board.isSpecialTile(position))
            {
                specialTile(player,3);
            }
            else if(_board.isHiddenTreasure(position))
            {
                hiddenTreasure(player);
            }
            isCalamities(player);
            isGummyTile(player);
            return true;
        }
        else if(_cards.type == "double")
        {
            cout << "You drew a double Marvelous magenta! Your game piece advances two Magenta tile. Here is the updated trail:" << endl;
            _board.movePlayer(6,player);
            _board.displayBoard();
            int position = _board.getPlayerPosition(player);
            isEqual(player);
            if(_board.isPositionCandyStore(position))
            {
                cout << "You landed on a candy store!" << endl;   
                if(position >= 0 && position <= 26)
                {
                    _candyStores[0].buyCandy(_players[player-1]);
                }
                else if(position >= 27 && position <= 53)
                {
                    _candyStores[1].buyCandy(_players[player-1]);
                }
                else
                {
                    _candyStores[2].buyCandy(_players[player-1]);
                }
                //_candyStores
            }
            else if(_board.isSpecialTile(position))
            {
                specialTile(player,6);
            }
            else if(_board.isHiddenTreasure(position))
            {
                hiddenTreasure(player);
            }
            isCalamities(player);
            isGummyTile(player);
            return true;
        }
    }
    else if(_board.getPlayerPosition(player)%3 == 1 && _cards.color[random_color] == "Marvelous Magenta")
    {
        if(_cards.type == "single")
        {
            cout << "You drew a single Marvelous magenta! Your game piece advances to Magenta tile. Here is the updated trail:" << endl;
            _board.movePlayer(2,player);
            _board.displayBoard();
            int position = _board.getPlayerPosition(player);
            isEqual(player);
            if(_board.isPositionCandyStore(position))
            {
                cout << "You landed on a candy store!" << endl;
                if(position >= 0 && position <= 26)
                {
                    _candyStores[0].buyCandy(_players[player-1]);
                }
                else if(position >= 27 && position <= 53)
                {
                    _candyStores[1].buyCandy(_players[player-1]);
                }
                else
                {
                    _candyStores[2].buyCandy(_players[player-1]);
                }
                //_candyStores
            }
            else if(_board.isSpecialTile(position))
            {
                specialTile(player,2);
            }
            else if(_board.isHiddenTreasure(position))
            {
                hiddenTreasure(player);
            }
            isCalamities(player);
            isGummyTile(player);
            return true;
        }
        else if(_cards.type == "double")
        {
            cout << "You drew a double Marvelous magenta! Your game piece advances two Magenta tile. Here is the updated trail:" << endl;
            _board.movePlayer(5,player);
            _board.displayBoard();
            int position = _board.getPlayerPosition(player);
            isEqual(player);
            if(_board.isPositionCandyStore(position))
            {
                cout << "You landed on a candy store!" << endl;
                if(position >= 0 && position <= 26)
                {
                    _candyStores[0].buyCandy(_players[player-1]);
                }
                else if(position >= 27 && position <= 53)
                {
                    _candyStores[1].buyCandy(_players[player-1]);
                }
                else
                {
                    _candyStores[2].buyCandy(_players[player-1]);
                }
                //_candyStores
            }
            else if(_board.isSpecialTile(position))
            {
                specialTile(player,5);
            }
            else if(_board.isHiddenTreasure(position))
            {
                hiddenTreasure(player);
            }
            isCalamities(player);
            isGummyTile(player);
            return true;
        }
    }
    else if(_board.getPlayerPosition(player)%3 == 2 && _cards.color[random_color] == "Marvelous Magenta")
    {
        if(_cards.type == "single")
        {
            cout << "You drew a single Marvelous magenta! Your game piece advances to Magenta tile. Here is the updated trail:" << endl;
            _board.movePlayer(1,player);
            _board.displayBoard();
            int position = _board.getPlayerPosition(player);
            isEqual(player);
            if(_board.isPositionCandyStore(position))
            {
                cout << "You landed on a candy store!" << endl;
                if(position >= 0 && position <= 26)
                {
                    _candyStores[0].buyCandy(_players[player-1]);
                }
                else if(position >= 27 && position <= 53)
                {
                    _candyStores[1].buyCandy(_players[player-1]);
                }
                else
                {
                    _candyStores[2].buyCandy(_players[player-1]);
                }
                //_candyStores
            }
            else if(_board.isSpecialTile(position))
            {
                specialTile(player,1);
            }
            else if(_board.isHiddenTreasure(position))
            {
                hiddenTreasure(player);
            }
            isCalamities(player);
            isGummyTile(player);
            return true;
        }
        else if(_cards.type == "double")
        {
            cout << "You drew a double Marvelous magenta! Your game piece advances two Magenta tile. Here is the updated trail:" << endl;
            _board.movePlayer(4,player);
            _board.displayBoard();
            int position = _board.getPlayerPosition(player);
            isEqual(player);
            if(_board.isPositionCandyStore(position))
            {
                cout << "You landed on a candy store!" << endl;
                if(position >= 0 && position <= 26)
                {
                    _candyStores[0].buyCandy(_players[player-1]);
                }
                else if(position >= 27 && position <= 53)
                {
                    _candyStores[1].buyCandy(_players[player-1]);
                }
                else
                {
                    _candyStores[2].buyCandy(_players[player-1]);
                }
                //_candyStores
            }
            else if(_board.isSpecialTile(position))
            {
                specialTile(player,4);
            }
            else if(_board.isHiddenTreasure(position))
            {
                hiddenTreasure(player);
            }
            isCalamities(player);
            isGummyTile(player);
            return true;
        }
    }
    if(_board.getPlayerPosition(player)%3 == 0 && _cards.color[random_color] == "Minty Green")
    {
        if(_cards.type == "single")
        {
            cout << "You drew a single Minty Green! Your game piece advances to Green tile. Here is the updated trail:" << endl;
            _board.movePlayer(1,player);
            _board.displayBoard();
            int position = _board.getPlayerPosition(player);
            isEqual(player);
            if(_board.isPositionCandyStore(position))
            {
                cout << "You landed on a candy store!" << endl;
                if(position >= 0 && position <= 26)
                {
                    _candyStores[0].buyCandy(_players[player-1]);
                }
                else if(position >= 27 && position <= 53)
                {
                    _candyStores[1].buyCandy(_players[player-1]);
                }
                else
                {
                    _candyStores[2].buyCandy(_players[player-1]);
                }
                //_candyStores
            }
            else if(_board.isSpecialTile(position))
            {
                specialTile(player,1);
            }
            else if(_board.isHiddenTreasure(position))
            {
                hiddenTreasure(player);
            }
            isCalamities(player);
            isGummyTile(player);
            return true;
        }
        else if(_cards.type == "double")
        {
            cout << "You drew a double Minty Green! Your game piece advances two Green tile. Here is the updated trail:" << endl;
            _board.movePlayer(4,player);
            _board.displayBoard();
            int position = _board.getPlayerPosition(player);
            isEqual(player);
            if(_board.isPositionCandyStore(position))
            {
                cout << "You landed on a candy store!" << endl;
                if(position >= 0 && position <= 26)
                {
                    _candyStores[0].buyCandy(_players[player-1]);
                }
                else if(position >= 27 && position <= 53)
                {
                    _candyStores[1].buyCandy(_players[player-1]);
                }
                else
                {
                    _candyStores[2].buyCandy(_players[player-1]);
                }
                //_candyStores
            }
            else if(_board.isSpecialTile(position))
            {
                specialTile(player,4);
            }
            else if(_board.isHiddenTreasure(position))
            {
                hiddenTreasure(player);
            }
            isCalamities(player);
            isGummyTile(player);
            return true;
        }
    }
    else if(_board.getPlayerPosition(player)%3 == 1 && _cards.color[random_color] == "Minty Green")
    {
        if(_cards.type == "single")
        {
            cout << "You drew a single Minty Green! Your game piece advances to Green tile. Here is the updated trail:" << endl;
            _board.movePlayer(3,player);
            _board.displayBoard();
            int position = _board.getPlayerPosition(player);
            isEqual(player);
            if(_board.isPositionCandyStore(position))
            {
                cout << "You landed on a candy store!" << endl;
                if(position >= 0 && position <= 26)
                {
                    _candyStores[0].buyCandy(_players[player-1]);
                }
                else if(position >= 27 && position <= 53)
                {
                    _candyStores[1].buyCandy(_players[player-1]);
                }
                else
                {
                    _candyStores[2].buyCandy(_players[player-1]);
                }
                //_candyStores
            }
            else if(_board.isSpecialTile(position))
            {
                specialTile(player,3);
            }
            else if(_board.isHiddenTreasure(position))
            {
                hiddenTreasure(player);
            }
            isCalamities(player);
            isGummyTile(player);
            return true;
        }
        else if(_cards.type == "double")
        {
            cout << "You drew a double Minty Green! Your game piece advances two Green tile. Here is the updated trail:" << endl;
            _board.movePlayer(6,player);
            _board.displayBoard();
            int position = _board.getPlayerPosition(player);
            isEqual(player);
            if(_board.isPositionCandyStore(position))
            {
                cout << "You landed on a candy store!" << endl;
                if(position >= 0 && position <= 26)
                {
                    _candyStores[0].buyCandy(_players[player-1]);
                }
                else if(position >= 27 && position <= 53)
                {
                    _candyStores[1].buyCandy(_players[player-1]);
                }
                else
                {
                    _candyStores[2].buyCandy(_players[player-1]);
                }
                //_candyStores
            }
            else if(_board.isSpecialTile(position))
            {
                specialTile(player,6);
            }
            else if(_board.isHiddenTreasure(position))
            {
                hiddenTreasure(player);
            }
            isCalamities(player);
            isGummyTile(player);
            return true;
        }
    }
    else if(_board.getPlayerPosition(player)%3 == 2 && _cards.color[random_color] == "Minty Green")
    {
        if(_cards.type == "single")
        {
            cout << "You drew a single Minty Green! Your game piece advances to Green tile. Here is the updated trail:" << endl;
            _board.movePlayer(2,player);
            _board.displayBoard();
            int position = _board.getPlayerPosition(player);
            isEqual(player);
            if(_board.isPositionCandyStore(position))
            {
                cout << "You landed on a candy store!" << endl;
                if(position >= 0 && position <= 26)
                {
                    _candyStores[0].buyCandy(_players[player-1]);
                }
                else if(position >= 27 && position <= 53)
                {
                    _candyStores[1].buyCandy(_players[player-1]);
                }
                else
                {
                    _candyStores[2].buyCandy(_players[player-1]);
                }
                //_candyStores
            }
            else if(_board.isSpecialTile(position))
            {
                specialTile(player,2);
            }
            else if(_board.isHiddenTreasure(position))
            {
                hiddenTreasure(player);
            }
            isCalamities(player);
            isGummyTile(player);
            return true;
        }
        else if(_cards.type == "double")
        {
            cout << "You drew a double Minty Green! Your game piece advances two Green tile. Here is the updated trail:" << endl;
            _board.movePlayer(5,player);
            _board.displayBoard();
            int position = _board.getPlayerPosition(player);
            isEqual(player);
            if(_board.isPositionCandyStore(position))
            {
                cout << "You landed on a candy store!" << endl;
                if(position >= 0 && position <= 26)
                {
                    _candyStores[0].buyCandy(_players[player-1]);
                }
                else if(position >= 27 && position <= 53)
                {
                    _candyStores[1].buyCandy(_players[player-1]);
                }
                else
                {
                    _candyStores[2].buyCandy(_players[player-1]);
                }
                //_candyStores
            }
            else if(_board.isSpecialTile(position))
            {
                specialTile(player,5);
            }
            else if(_board.isHiddenTreasure(position))
            {
                hiddenTreasure(player);
            }
            isCalamities(player);
            isGummyTile(player);
            return true;
        }
    }
    if(_board.getPlayerPosition(player)%3 == 0 && _cards.color[random_color] == "Bubblegum Blue")
    {
        if(_cards.type == "single")
        {
            cout << "You drew a single Bubblegum Blue! Your game piece advances to Blue tile. Here is the updated trail:" << endl;
            _board.movePlayer(2,player);
            _board.displayBoard();
            int position = _board.getPlayerPosition(player);
            isEqual(player);
            if(_board.isPositionCandyStore(position))
            {
                cout << "You landed on a candy store!" << endl;
                if(position >= 0 && position <= 26)
                {
                    _candyStores[0].buyCandy(_players[player-1]);
                }
                else if(position >= 27 && position <= 53)
                {
                    _candyStores[1].buyCandy(_players[player-1]);
                }
                else
                {
                    _candyStores[2].buyCandy(_players[player-1]);
                }
                //_candyStores
            }
            else if(_board.isSpecialTile(position))
            {
                specialTile(player,2);
            }
            else if(_board.isHiddenTreasure(position))
            {
                hiddenTreasure(player);
            }
            isCalamities(player);
            isGummyTile(player);
            return true;
        }
        else if(_cards.type == "double")
        {
            cout << "You drew a double Bubblegum Blue! Your game piece advances two Blue tile. Here is the updated trail:" << endl;
            _board.movePlayer(5,player);
            _board.displayBoard();
            int position = _board.getPlayerPosition(player);
            isEqual(player);
            if(_board.isPositionCandyStore(position))
            {
                cout << "You landed on a candy store!" << endl;
                if(position >= 0 && position <= 26)
                {
                    _candyStores[0].buyCandy(_players[player-1]);
                }
                else if(position >= 27 && position <= 53)
                {
                    _candyStores[1].buyCandy(_players[player-1]);
                }
                else
                {
                    _candyStores[2].buyCandy(_players[player-1]);
                }
                //_candyStores
            }
            else if(_board.isSpecialTile(position))
            {
                specialTile(player,5);
            }
            else if(_board.isHiddenTreasure(position))
            {
                hiddenTreasure(player);
            }
            isCalamities(player);
            isGummyTile(player);
            return true;
        }
    }
    else if(_board.getPlayerPosition(player)%3 == 1 && _cards.color[random_color] == "Bubblegum Blue")
    {
        if(_cards.type == "single")
        {
            cout << "You drew a single Bubblegum Blue! Your game piece advances to Blue tile. Here is the updated trail:" << endl;
            _board.movePlayer(1,player);
            _board.displayBoard();
            int position = _board.getPlayerPosition(player);
            isEqual(player);
            if(_board.isPositionCandyStore(position))
            {
                cout << "You landed on a candy store!" << endl;
                if(position >= 0 && position <= 26)
                {
                    _candyStores[0].buyCandy(_players[player-1]);
                }
                else if(position >= 27 && position <= 53)
                {
                    _candyStores[1].buyCandy(_players[player-1]);
                }
                else
                {
                    _candyStores[2].buyCandy(_players[player-1]);
                }
                //_candyStores
            }
            else if(_board.isSpecialTile(position))
            {
                specialTile(player,1);
            }
            else if(_board.isHiddenTreasure(position))
            {
                hiddenTreasure(player);
            }
            isCalamities(player);
            isGummyTile(player);
            return true;
        }
        else if(_cards.type == "double")
        {
            cout << "You drew a double Bubblegum Blue! Your game piece advances two Blue tile. Here is the updated trail:" << endl;
            _board.movePlayer(4,player);
            _board.displayBoard();
            int position = _board.getPlayerPosition(player);
            isEqual(player);
            if(_board.isPositionCandyStore(position))
            {
                cout << "You landed on a candy store!" << endl;
                if(position >= 0 && position <= 26)
                {
                    _candyStores[0].buyCandy(_players[player-1]);
                }
                else if(position >= 27 && position <= 53)
                {
                    _candyStores[1].buyCandy(_players[player-1]);
                }
                else
                {
                    _candyStores[2].buyCandy(_players[player-1]);
                }
                //_candyStores
            }
            else if(_board.isSpecialTile(position))
            {
                specialTile(player,4);
            }
            else if(_board.isHiddenTreasure(position))
            {
                hiddenTreasure(player);
            }
            isCalamities(player);
            isGummyTile(player);
            return true;
        }
    }
    else if(_board.getPlayerPosition(player)%3 == 2 && _cards.color[random_color] == "Bubblegum Blue")
    {
        if(_cards.type == "single")
        {
            cout << "You drew a single Bubblegum Blue! Your game piece advances to Blue tile. Here is the updated trail:" << endl;
            _board.movePlayer(3,player);
            _board.displayBoard();
            int position = _board.getPlayerPosition(player);
            isEqual(player);
            if(_board.isPositionCandyStore(position))
            {
                cout << "You landed on a candy store!" << endl;
                if(position >= 0 && position <= 26)
                {
                    _candyStores[0].buyCandy(_players[player-1]);
                }
                else if(position >= 27 && position <= 53)
                {
                    _candyStores[1].buyCandy(_players[player-1]);
                }
                else
                {
                    _candyStores[2].buyCandy(_players[player-1]);
                }
                //_candyStores
            }
            else if(_board.isSpecialTile(position))
            {
                specialTile(player,3);
            }
            else if(_board.isHiddenTreasure(position))
            {
                hiddenTreasure(player);
            }
            isCalamities(player);
            isGummyTile(player);
            return true;
        }
        else if(_cards.type == "double")
        {
            cout << "You drew a double Bubblegum Blue! Your game piece advances two Blue tile. Here is the updated trail:" << endl;
            _board.movePlayer(6,player);
            _board.displayBoard();
            int position = _board.getPlayerPosition(player);
            isEqual(player);
            if(_board.isPositionCandyStore(position))
            {
                cout << "You landed on a candy store!" << endl;
                if(position >= 0 && position <= 26)
                {
                    _candyStores[0].buyCandy(_players[player-1]);
                }
                else if(position >= 27 && position <= 53)
                {
                    _candyStores[1].buyCandy(_players[player-1]);
                }
                else
                {
                    _candyStores[2].buyCandy(_players[player-1]);
                }
                //_candyStores
            }
            else if(_board.isSpecialTile(position))
            {
                specialTile(player,6);
            }
            else if(_board.isHiddenTreasure(position))
            {
                hiddenTreasure(player);
            }
            isCalamities(player);
            isGummyTile(player);
            return true;
        }
    }
    return false;
    //cout << _cards.color[random_color] << " " << _cards.type << endl;
}

bool Game::useCandy(int player)//use candy
{
    string candy;
    Candy candy_to_use;
    char input;
    int count = 0;
    if(_players[player-1].getCandyAmount() == 0)
    {
        cout << "No available candies to use! Please enter another option!" << endl;
        return false;
    }
    for(int i = 0;i < _immunity_candies.size();i++)//looping to check if player only has immunity candies
    {
        if(_immunity_candies.at(i).name == _players[player-1].findCandy(_immunity_candies.at(i).name).name)
        {
            count++;
        }
    }
    if(count == _players[player-1].getCandyAmount())//if only immunity candies, dontlet player use any
    {
        cout << "All available candies are immunity candies! Unable to use candy!" << endl;
        return true;
    }
    cout << "Here is a list of your candies" << endl;
    _players[player-1].printInventory();
    cout << "Enter a candy you wish to use: " << endl;
    cin.clear();
    cin.ignore(100000,'\n');
    getline(cin,candy);
    while(lowerString(_players[player-1].findCandy(lowerString(candy)).name) == "" || lowerString(_players[player-1].findCandy(lowerString(candy)).candy_type) == "immunity")//input validation
    {
        cout << "Invald input! Please try again" << endl;
        cin.clear();
        //cin.ignore(1000000,'\n');
        getline(cin,candy);
    }
    for(int i = 0;i < 9;i++)
    {
        if(lowerString(_players[player-1].findCandy(lowerString(candy)).name) != "")//looking in player inventory
        {
            candy_to_use = _players[player-1].findCandy(candy);
            break;
        }
    }
    if(candy_to_use.candy_type == "magical")//check if magical
    {
        if(candy_to_use.effect == "stamina")//check if stamina incase i add more effect types
        {
            int new_stamina = _players[player-1].getStamina() + candy_to_use.effect_value;
            cout << "You have used " << candy_to_use.name << " candy. This has increased your stamina by " << candy_to_use.effect_value << endl;
            _players[player-1].setStamina(new_stamina);
            _players[player-1].removeCandy(candy_to_use.name);
            return true;
        }
    }
    else if(candy_to_use.candy_type == "poison")//check if poison
    {
        int effect_val = candy_to_use.effect_value;
        int player_to_poison = player;
        cout << "Use " << candy_to_use.name << " to poison your opponent?(y/n)" << endl;
        cin >> input;
        while(cin.fail() || (tolower(input) != 'y' && tolower(input) != 'n'))
        {
            if(cin.fail())
            {
                cin.clear();
                cin.ignore(10000,'\n');
            }
            cout << "Invalid input! Please enter a valid input!" << endl;
            cin >> input;
        }
        if(tolower(input == 'n'))
        {
            cout << "You have decided not to use " << candy_to_use.name << " and have been sent back to the main menu!" << endl;
            return false;
        }
        switch(player)//switch case for opposite players
        {
            case 1:
                player_to_poison = 2;
                break;
            case 2:
                player_to_poison = 1;
                break;
        }
        for(int i = 0;i < _immunity_candies.size();i++)//matching effect values 
        {
            if(effect_val == -10)//if tier 1
            {
                if(_players[player_to_poison-1].findCandy(_immunity_candies[i].name).name != "")//if counter found
                {
                    if(_players[player_to_poison-1].findCandy(_immunity_candies[i].name).effect_value == 1 || _players[player_to_poison-1].findCandy(_immunity_candies[i].name).effect_value == 2 || _players[player_to_poison-1].findCandy(_immunity_candies[i].name).effect_value == 3)
                    {
                        cout << "Player " << _players[player_to_poison-1].getPlayerNumber() << " used " << _players[player_to_poison-1].findCandy(_immunity_candies[i].name).name
                        << " to counter the effect of your poison candy!" << endl;
                        _players[player-1].removeCandy(candy_to_use.name);
                        _players[player_to_poison-1].removeCandy(_immunity_candies[i].name);
                        return true;
                    }
                }
                else//poison if not found
                {
                    int new_stamina = _players[player_to_poison-1].getStamina() + effect_val;
                    _players[player_to_poison-1].setStamina(new_stamina);
                    cout << "You have used " << _players[player-1].findCandy(candy_to_use.name).name << " to poison your opponent! They have lost " 
                    << _players[player-1].findCandy(candy_to_use.name).effect_value << " stamina!" << endl;
                    _players[player-1].removeCandy(candy_to_use.name);
                    return true;
                }
            }
            else if(effect_val == -15)//if tier 2
            {
                if(_players[player_to_poison-1].findCandy(_immunity_candies[i].name).name != "")//if counter found
                {
                    if(_players[player_to_poison-1].findCandy(_immunity_candies[i].name).effect_value == 2 || _players[player_to_poison-1].findCandy(_immunity_candies[i].name).effect_value == 3)
                    {
                        cout << "Player " << _players[player_to_poison-1].getPlayerNumber() << " used " << _players[player_to_poison-1].findCandy(_immunity_candies[i].name).name
                        << " to counter the effect of your poison candy!" << endl;
                        _players[player-1].removeCandy(candy_to_use.name);
                        _players[player_to_poison-1].removeCandy(_immunity_candies[i].name);
                        return true;
                    }
                }
                else//poison if not found
                {
                    int new_stamina = _players[player_to_poison-1].getStamina() + effect_val;
                    _players[player_to_poison-1].setStamina(new_stamina);
                    cout << "You have used " << _players[player-1].findCandy(candy_to_use.name).name << " to poison your opponent! They have lost " 
                    << _players[player-1].findCandy(candy_to_use.name).effect_value << " stamina!" << endl;
                    _players[player-1].removeCandy(candy_to_use.name);
                    return true;
                }
            }
            else if(effect_val == -20)//if tier 3
            {
                if(_players[player_to_poison-1].findCandy(_immunity_candies[i].name).name != "")//if counter found
                {
                    if(_players[player_to_poison-1].findCandy(_immunity_candies[i].name).effect_value == 3)
                    {
                        cout << "Player " << _players[player_to_poison-1].getPlayerNumber() << " used " << _players[player_to_poison-1].findCandy(_immunity_candies[i].name).name
                        << " to counter the effect of your poison candy!" << endl;
                        _players[player-1].removeCandy(candy_to_use.name);
                        _players[player_to_poison-1].removeCandy(_immunity_candies[i].name);
                        return true;
                    }
                }
                else//poison if not found
                {
                    int new_stamina = _players[player_to_poison-1].getStamina() + effect_val;
                    _players[player_to_poison-1].setStamina(new_stamina);
                    cout << "You have used " << _players[player-1].findCandy(candy_to_use.name).name << " to poison your opponent! They have lost " 
                    << _players[player-1].findCandy(candy_to_use.name).effect_value << " stamina!" << endl;
                    _players[player-1].removeCandy(candy_to_use.name);
                    return true;
                }
            }
        }
    }
    else if (candy_to_use.candy_type == "gummy")//if gummy
    {
        int tile_to_place = -1;
        cout << "On which tile do you want to place your " << candy_to_use.name << " - gummy candy?" << endl;
        cin >> tile_to_place;
        while(cin.fail() || tile_to_place <= 0 || tile_to_place > 81)//input validation
        {
            if(cin.fail())
            {
                cin.clear();
                cin.ignore(10000,'\n');
            }
            cout << "Invalid input! Please enter a tile between 1 and 81" << endl;
            cin >> tile_to_place;
        }
        if(candy_to_use.effect_value == -1)//check if 1 turn gummy
        {
            _gummy_tiles1.push_back((tile_to_place-1));
        }
        else if(candy_to_use.effect_value == -2)//check if 2 turn gummy
        {
            _gummy_tiles2.push_back((tile_to_place-1));
        }
        cout << "You have successfully placed gummy candy on tile " << tile_to_place 
        << ". Any player that lands on the gummy tile will be obstructed from advancing past the tile for " 
        << candy_to_use.effect_value << " moves." << endl;
        _players[player-1].removeCandy(candy_to_use.name);
        return true;
    }
    else if(candy_to_use.candy_type == "special")//in case of truffle
    {
        if(candy_to_use.effect == "treasure")
        {
            cout << "Use " << candy_to_use.name << " to unlock a hidden treasure?(y/n)" << endl;
            cin >> input;
            while(cin.fail() || (tolower(input) != 'y' && tolower(input) != 'n'))
            {
                if(cin.fail())
                {
                    cin.clear();
                    cin.ignore(10000,'\n');
                }
                cout << "Invalid input! Please enter a valid input!" << endl;
                cin >> input;
            }
            if(tolower(input == 'n'))
            {
                cout << "You have decided not to use " << candy_to_use.name << " and have been sent back to the main menu!" << endl;
                return false;
            }
            else
            {
                hiddenTreasure(player);
                return true;
            }
        }
    }
    return false;
}