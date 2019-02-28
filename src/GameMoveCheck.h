#ifndef GAMEMOVECHECK
#define GAMEMOVECHECK
/*
************************************************************
* Name:  Rojan Shrestha                                    *
* Project:  Casino_Project3					            *
* Class:  CMPS 366 01				                        *
* Date:  Feb 5th, 2019			                        *
************************************************************
*/


//import java.util.Collections;
//import java.util.Comparator;
//import java.util.Iterator;
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <math.h>
#include "Human.h"
#include "Computer.h"
#include "Table.h"
#include "Deck.h"

class GameMoveCheck 
{

public:
	//Constructor:
	GameMoveCheck(std::vector<std::string> movesStorage, Table gameTable, Player player);
	//Overload constructor for computer logic and human help
	GameMoveCheck(bool humanTurn,Table gameTable, Player player);

	//calls respective action move Check function
	bool moveCheck();

	//returns the handposition
	int const getHandPosition() { return handPosition; }
	
	//returns moveType
	std::string const getMoveType() { return moveType; }
	
	//returns the table postions as vector
	std::vector<std::string> const getTablePosition() { return tablePosition; }

	//returns the numeric value of the card
	int const cardStringToValue(std::string key);

	//returns the current BuildPairds
	std::vector<std::vector<std::string>> const getCurrentBuildPairds(){ return currentBuildPairs;}

	//buffer function to check if the set of user given table and hand positions
	//is a valid set of move
	//calls recursiveSumChecker
	bool captureBuildSumChecker(int valueCompare);

	//utility function for captureBuildSumChecker that recursively checks for valid sets of card for given move
	std::vector<int> recursiveSumChecker(int sum , int valueCompare, std::vector<int> selectedPosValue);

	//generates string info of build in format --> [ [DX] [H9 SA] ] Human
	void generateBuildPairs(int buildValue);

	//compare utility function sorts the cards in ascending order based on the card value
	static bool compareInterval(std::vector<std::string> cardA, std::vector<std::string> cardB); 

	//returns power set of all possible combination of table cards for a given hand card
	std::vector<std::vector<std::string>> tablePositionPowerSet();

	//returns a list of all the valid moves for action Capture
	std::vector<std::vector<std::string>> getValidCaptureList();

	//returns a list of all the valid moves for action Build
	std::vector<std::vector<std::string>> getValidBuildList();

	//returns the best capture move
	std::vector<std::string> getBestValidMove(bool isCapture);

	//Check the card for aces and DX and S2 and calculates the score
	int const static scoreCalculator(std::string card);

	//sets the table and hand position
	void setPosTableHand(int handPosition, std::vector<std::string> tablePosition);

private:
	//Memeber Variables:
	std::vector<std::string> tablePosition;
	std::string moveType;
	int handPosition;
	//stores the string instruction of valid build move
	std::vector<std::vector<std::string>> currentBuildPairs;

	//all the objects of required class
	Table gameTable;
	Player player;

	bool humanTurn;
	int valueHandPosition;

	//also holds the info about player and value of build
	//returns the vector with the card info from the buildCards, required by the build action 
	//to generate pairs for build 
	std::vector<std::string> addPairdsBuild(std::vector<std::vector<std::string>> buildCards, int index);


};

#endif