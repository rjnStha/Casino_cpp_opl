/*
************************************************************
* Name:  Rojan Shrestha                                    *
* Project:  Casino_Project3					            *
* Class:  CMPS 366 01				                        *
* Date:  Feb 5th, 2019			                        *
************************************************************
*/

#include <iostream>
#include <time.h>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
// std::this_thread::sleep_for
#include <thread>
// std::chrono::seconds#include "Deck.h"      
#include <chrono>
#include <algorithm>
#include <sstream>
#include <dirent.h>
#include <sys/types.h>

#include "GameMoveCheck.h"
#include "Player.h"

class GameEngine
{
public:

	//Constructors:
	GameEngine();

	//Selectors:
	//returns game Table object
	Table getGameTable() { return gameTable; }

	//returns human object
	Human getHuman() { return human; }

	//returns computer object
	Computer getComputer() { return computer; }

	//returns the boolean value for human turn
	bool isHumanTurn() { return humanTurn; }
	
	//returns the boolean value for last capturer, ture for human
	bool isHumanLastCapturer() { return humanTurn; }

	//Utilities:
	//deals 4 new cards for each players, table if dealTable==true
	void newDealCards(bool dealTable);

	//prints all the cards in table and player's hand in systematic layout
	//user input for loading, exit or new game
	void const homeScreen();

	//prints all the cards in table and player's hand in systematic layout
	void const printGameBoard();

	//random coin toss and check the result to the human response and sets the turn
	void coinToss();

	//central function for executing human move
	int humanMove();

	//central function for executing computer move
	int computerMove();

	//user input for human move info
	std::vector<std::string> getHumanMoveInfo();

	//action trail - trails the given hand position
	bool trail(int handPosition);

	//carries out capture action with given hand position and table card position
	bool capture(int handPosition, std::vector<std::string> tableCardPosition);

	//carries out build action for human or computer with given hand position,table card position and build pairs
	bool build(int handPosition, std::vector<std::string> tableCardPosition, std::vector<std::vector<std::string>> buildPairs);

	//get the user selection from the list of files in the folder
	std::string getLoadGameFile();
	
	//loads the game with given file name
	bool loadGame(std::string fileName);

	//stores one build multi or single into the table for human and computer
	bool storeBuildTable(std::string data);

	//returns the numeric value of the card
	int cardStringToValue(std::string key);

	//saves game of given filename
	bool saveGame(std::string fileName);

	//check for changes for dealing cards, new round and end of the tournament
	bool checkGameChanges();

	//calculates and returns the score for given player pile, also finds the num of total cards and num of spade cards
	std::vector<int> calculatePlayerScore(Player player);


private:
	//Member Variables:
	//round of the game
	int round;
	//flag for human turn
	bool humanTurn;
	//flag for human as last capturer
	std::string lastCapturer;

	//Game inheriting all the class
	Table gameTable;
	Deck gameDeck = Deck(true);
	Human human;
	Computer computer;
};