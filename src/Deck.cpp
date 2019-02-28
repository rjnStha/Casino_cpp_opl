#include <iostream>
#include <time.h>
#include "Deck.h"
    	
//creates 52 cards and shuffles the cards
Deck::Deck(bool isNewRound)
{	
	//check if it is new round else return without creating new cards
	//required when loading a game
    if(!isNewRound) return;

	//assign values and suit to each of the 52 cards
	for(int i = 0;i < 52;i++)
	{
		std::string temp;
		
		//first character suit
		if(i<13) temp = 'S';
		else if(i<26) temp = 'H';
		else if(i<39) temp = 'C';
		else temp = 'D';
		
		//second character value
		int tempValue = (i % 13)+1;
		if(tempValue == 1) temp += 'A';
		else if(tempValue < 10) temp += std::to_string(tempValue);
		else if(tempValue == 10) temp += 'X';
		else if(tempValue == 11) temp += 'J';
		else if(tempValue == 12) temp += 'Q';
		else temp += 'K';

		//push the temp into the deck
		cardDeck.push_back(temp);
	}

	//shuffle
	shuffle();
}

/* *********************************************************************
Function Name: getNewCard
Purpose: returns the first card of the deck vector
		also calls Deck::removeFirstCard() to remove the card from deck
Parameters: none
Return Value: the first card from the deck, a string value
Local Variables: none
Algorithm:
            1) make copy of the first card of deck vector
            2) call removeFirstCard();
            3) return the copy
Assistance Received: none
********************************************************************* */ 
std::string const Deck::getNewCard () 
{
	//returning the copof the card
	std::string temp = cardDeck.at(0);
	//call function to erase the first card
	removeFirstCard();
	return temp;
}

/* *********************************************************************
Function Name: printCards
Purpose: prints all the cards in the deck
Parameters: none
Return Value: void
Local Variables: none
Algorithm:
            1) Access all vector elements and print
Assistance Received: none
********************************************************************* */ 
void const Deck::printDeckCards()
{
	std::cout<<"Deck: \n";
	for(int i = 0;i < getDeckSize();i++){ std::cout<<cardDeck.at(i)<<" "; }
	std::cout<<"\n\n";
}

/* *********************************************************************
Function Name: shuffle
Purpose: shuffles the deck
Parameters: none
Return Value: void
Local Variables: none
Algorithm:
            1) loop counter equals
            2) Swap one by one
Assistance Received: none
********************************************************************* */ 
void Deck::shuffle()
{
	//different seed value 
	srand(time(NULL));
	
	// Start from the last element and swap one by one. We don't 
    // need to run for the first element that's why i > 0 
	for (int i = getDeckSize()-1; i > 0; i--)
    { 
        // Pick a random index from 0 to i 
        int j = rand() % (i+1);

        // Swap i with the element at random index j 
        std::string temp = cardDeck.at(i);
        cardDeck.at(i) = cardDeck.at(j);
        cardDeck.at(j) = temp;
    } 
}


