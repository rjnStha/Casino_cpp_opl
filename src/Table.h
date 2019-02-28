#include <vector>
#include <string>
#include <algorithm>

class Table
{
public:
	
	
	//Constructor
	Table(){}
	
	//stores the vector with the card in the table board
	//stores the value of card,string as first element of the card
	void storeTable(std::vector<std::string> card) { gameBoard.push_back(card); }

	//stores the given card in a vector 
	void storeTable(std::string card);

	//returns the table card of given index
	std::vector<std::string> popTableCard(int index);

	//returns the size of the cardDeck vector(number of cards)
	int const getTableSize(){ return gameBoard.size(); }

	//returns copy of board
    std::vector<std::vector<std::string>> const getAllCards() {return gameBoard;}

	//print cards in hand / pile
	void printTable();

private:

	//vector to store the cards on the table board
	/**
	* Rules for single card --> the first element, the value of the build
							the second, the card 
    * Rules for build --> the first element, the value of the build
    *  			the second, owner of the build: Human or Computer
    *  			the third, if the build is multi or single build: Single or Multi
    *  			the fourth, if single build then start inputting card else
    *  				if multi then "[" then start inputting card on fifth
    *  			nth, for multi "]" to end a single build inside multi
	*/
	std::vector<std::vector<std::string>> gameBoard;

};