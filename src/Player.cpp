#include <iostream>
#include "Player.h"


/* *********************************************************************
Function Name: getHand
Purpose: returns the hand card of given index
Parameters: 
		index, integer. It holds the index as the position of card in the player hand.
Return Value: the value of indexed card, a string value
Local Variables: 
		tenp, string to store hand at index
Algorithm:
Assistance Received: none
********************************************************************* */
std::string const Player::popHandCard(int index)
{
	std::string temp = getHandCard(index);
	//erased the card and return
	//(index+1)th element
	removeHandCard(index);
	return temp;
}

/* *********************************************************************
Function Name: printHandOrPile
Purpose: print cards in hand / pile.
Parameters: 
		hp, boolean. true for hand
Return Value: void
Local Variables: 
		hpContainer, a vector of strings to store the hand or pile vector
Algorithm:
Assistance Received: none
********************************************************************* */
void const Player::printHandOrPile(bool isHand)
{

	std::vector<std::string> hpContainer;

	if(isHand)	{ 
		std::cout<<"Hand: ";
		hpContainer = hand; 

	} else {
		std::cout<<"Pile: "; 
		hpContainer = pile; 
	}
	
	for (int i = 0; i < hpContainer.size(); i++) 
	{
		std::cout << hpContainer.at(i) << ' ';
	}

	std::cout<<std::endl;

}
