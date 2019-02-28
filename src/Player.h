#ifndef PLAYER
#define PLAYER

#include <vector>
#include <string>

class Player
{
public:

	//Constructor
	Player() { score = 0; }
	
	//Selectors:

	//returns the score of the player
	int const getScore() { return score; }

	//returns the size of the hand
	int const getHandSize() { return hand.size(); }

	//returns the card at given position
	std::string const getHandCard(int index) { return hand.at(index); };

	//returns the hand card of given index and erases the card
	std::string const popHandCard(int index);

	//returns copy of hand
    std::vector<std::string> const getAllHandCards() {return hand;}

    //returns copy of pile
	std::vector<std::string> const getAllPileCards() {return pile;}

	
	//Mutators:
	//stores the given card in player hand
	void storeHand(std::string card) { hand.push_back(card); }

	//stores the given card in player pile
	void storePile(std::string card) { pile.push_back(card); }

	//adds the value to the score of the player
	void setScore(int value) { this->score = value; }

	//clears the pile
	void clearPile(){ pile.clear();}

	//Utilities:
	//print cards in hand / pile
	//True for hand
	void const printHandOrPile(bool isHand);


private:

	//Member Variables:
	//score of the player
	int score;
	
	//vector to store the cards in the player's hand
	std::vector<std::string> hand;

	//vector to store the captured cards in pile
	std::vector<std::string> pile;
	
	//Mutators:
	//removes the first card of hand
	void removeHandCard(int index) { hand.erase(hand.begin() + index); }

};

#endif