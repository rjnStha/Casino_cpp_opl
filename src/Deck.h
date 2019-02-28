#include <vector>
#include <string>

class Deck
{
public:
	
	//Constructor:
	Deck(bool isNewRound);

	//Selectors:
	//returns the first card of the cardDeck
	std::string const getNewCard();

	//get veector of cards in deck
	std::vector<std::string> const getDeck(){ return cardDeck;}

	//returns the size of the cardDeck vector(number of cards)
	int const getDeckSize(){ return cardDeck.size(); }

	//Mutators:
	//stores the given card in Deck
	void storeDeck(std::string card) { cardDeck.push_back(card); }
	
	//Utilities:
	//prints all the cards in the deck
	void const printDeckCards();

private:

	//Member Variables:
	//vector of Card struct
	std::vector<std::string> cardDeck;
	
	//Mutators:
	//removes the first card on deck
	void removeFirstCard() { cardDeck.erase(cardDeck.begin()); }

	//Utilities:
	//shuffles the deck
	void shuffle();

};