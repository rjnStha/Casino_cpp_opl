#include <iostream>
#include "Table.h"


/* *********************************************************************
Function Name: printTable
Purpose: print cards in table
Parameters: none 
Return Value: void
Local Variables: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Table::printTable()
{
	for(int i = 0; i<gameBoard.size();i++)
	{
		if(gameBoard.at(i).size() == 2 ){
			//single card
			std::cout << gameBoard[i][1] <<" ";

		} else{

			//build card
			std::cout <<"[ ";
			for(int j = 3; j<gameBoard[i].size();j++) { 
				std::cout << gameBoard[i][j] <<" ";
			}
            std::cout <<gameBoard[i][1]<<" ] ";
		}
	}

	std::cout<<std::endl;
}

//stores the given card in a vector 
//stores the vector with the card in the table board
//stores the value of card,string as first element of the card
/* *********************************************************************
Function Name: storeCardsTable
Purpose: stores the given card in a vector 
Parameters: string, card
Return Value: void
Local Variables: none
Algorithm: get the cardvalue and add to the vector
Assistance Received: none
********************************************************************* */
void Table::storeTable(std::string card) 
{
	//erasing the suit info
	std::string cardValue;

	//store the numeric value as string 
	switch(card.at(1))
    {
        case 'A': 
        	cardValue = "1";
        	break;
        case 'K': 
        	cardValue = "13";
        	break;
        case 'Q': 
        	cardValue = "12";
        	break;
        case 'J': 
        	cardValue = "11";
        	break;
        case 'X': 
        	cardValue = "10";
        	break;
        default: cardValue = card.substr(1,1);
    }

	std::vector<std::string> temp;
	temp.push_back(cardValue);
	temp.push_back(card);
	storeTable(temp);
}


/* *********************************************************************
Function Name: popTableCard
Purpose: returns the table card of given index
Parameters: 
		index, integer. It holds the index as the position of card in the table
Return Value: the value of indexed card, a string value
Local Variables: 
		tenp, string to store board card at index
Algorithm:
Assistance Received: none
********************************************************************* */
std::vector<std::string> Table::popTableCard(int index)
{
	std::vector<std::string> temp = gameBoard.at(index);

    //check for loose or build card
    if(temp.size()>2){
        //build card
        //remove the brackets if multi build
        if(temp.at(2) == "Multi") {
        	temp.erase(std::remove(temp.begin(), temp.end(), "["), temp.end());
            temp.erase(std::remove(temp.begin(), temp.end(), "]"), temp.end());
        }

        //remove the first three elements since card info starts form fourth
        for(int i =0; i<3;i++) { temp.erase(temp.begin()); }

    }
    else{

        //loose card has 2 elements value and card
        //removing the value
		temp.erase(temp.begin());
    }

    //erased the card and return
	//(index+1)th element
	gameBoard.erase(gameBoard.begin()+index);
    return temp;
}