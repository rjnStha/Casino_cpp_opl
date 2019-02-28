#include "GameMoveCheck.h"

//Constructor:
GameMoveCheck::GameMoveCheck( std::vector<std::string> movesStorage, Table gameTable, Player player)
{
	/*
    *not good for computer move
    */

    //store the required objects as memeber variables
    this->gameTable = gameTable;
    this->player = player;

    //get the turn of the player
    //pop back since the info is the last element of the vector
    std::string turn = movesStorage.at(movesStorage.size()-1);
    movesStorage.pop_back();
    if(turn == "Human") humanTurn = true;
    else humanTurn = false;

    //extract the move information
    moveType = movesStorage.at(0);
    
    //get hand position
    // converting string to int using string stream
    std::stringstream ss(movesStorage.at(1));
    ss >> handPosition;
    
    //check if table section of the information is empty
    //store in the table position
    if (movesStorage.size() > 2) {
        tablePosition = movesStorage;
        //removing the first two elements (action and hand position)
        tablePosition.erase(tablePosition.begin());
        tablePosition.erase(tablePosition.begin());
    }
}

//Overload constructor for computer logic and human help
GameMoveCheck::GameMoveCheck(bool humanTurn,Table gameTable, Player player)
{
    //store the parameter as memeber variable
    this->humanTurn = humanTurn;
    this->gameTable = gameTable;
    this->player = player;
}

/* *********************************************************************
Function Name: moveCheck
Purpose: calls respective action move Check function
Parameters:
Return Value: boolean, true if move valid
Local Variables: handCard, String info of selected hand position
Algorithm:
Assistance Received: none
********************************************************************* */
bool GameMoveCheck::moveCheck()
{

    //hand Card and its numeric value
    std::string handCard = player.getHandCard(handPosition);
    int handCardValue = cardStringToValue(handCard);

	if (moveType == "t" ) {
        
        //moveCheck for trail
        //get local vector<vector> for cards in table
        std::vector<std::vector<std::string>> tableCards = gameTable.getAllCards();
        //loop through each table cards and find build
        //check if the build is own by the player, true -> restrict trailing
        for(int i = 0; i<tableCards.size();i++ ){
            std::vector<std::string> tempCard = tableCards.at(i);
            for(int j = 0; j<tempCard.size();j++ ){
                //find the build
                if(tempCard.size()>2){
                    if((humanTurn && tempCard.at(1)=="Human")||
                        (!humanTurn && tempCard.at(1)=="Computer")){ return false; }
                }
            }   
        }

        return true;
       
    } else  if (moveType == "c" ) {
        //move Check for capture
        return captureBuildSumChecker(handCardValue);

    } else {
        //moveCheck for build

        //get the vector of hand cards
        std::vector<std::string> handCardList = player.getAllHandCards();

        //flag to find if build valued card in hand
        bool buildValueFound = false;
        int buildValue = 0;
        //loop through all the hand cards to find the build valued hand card
        for(int i = 0; i<handCardList.size();i++ ){

            //check for selected hand card and ignore if found
            if(i == handPosition){ continue; }

            //build value equals the value of given Hand Card
            buildValue = cardStringToValue(handCardList.at(i));

            //check validity for given hand card
            if(buildValue >= handCardValue && captureBuildSumChecker(buildValue)) {
                //found a hand card whose value is equal to build Value
                buildValueFound = true;
                break;
            }
        }

        //if build move valid generate pairs to make move
        if(buildValueFound) { generateBuildPairs(buildValue); }

        return buildValueFound;
    }
}

/* *********************************************************************
Function Name: cardStringToValue
Purpose: converts card value in character to numeric value
Parameters: key, char the character value of card DX --> X
Return Value: int, integer value of the card
Local Variables:
Algorithm: Check for X, A, J, Q and assign necessary values
Assistance Received: none
********************************************************************* */
int const GameMoveCheck::cardStringToValue(std::string key) 
{

    switch(key.at(1))
    {
        case 'A': return 1;
        case 'K': return 13;
        case 'Q': return 12;
        case 'J': return 11;
        case 'X': return 10;
        default: return key.at(1)-'0';
    }
}

//check for valid set of cards whose sum is equal to the the valueCompare
/* *********************************************************************
Function Name: captureBuildSumChecker
Purpose: Checks if sum of given move is divisible by the value to compare
    if divisible calls recursiveSumChecker for further validation of the move
Parameters: valueCompare, integer the value of the card to compare
Return Value: boolean, true if the move is valid
Local Variables: selectedPosValue, Vector to store valid moves for given value Compare
Algorithm:
    1. get the first element the highest value in the set
    2. for(size of list)
        3. Add the given value with next elements in the list and check the sum for =< valueCompare
        4. If equal then remove both the elements
        5. If less than get another value from the list and add to the sum, check for condition
        6. If greater than get next value
    7. Repeat 0,1,2,3,4 until no element in the list
Assistance Received: none
********************************************************************* */
bool GameMoveCheck::captureBuildSumChecker(int valueCompare) 
{

	//get local vector<vector> for cards in table
	std::vector<std::vector<std::string>> tableCards = gameTable.getAllCards();

	// 1. Get all cards of given position and their numeric value from the table 
	// 2. Check if the value is less or greater than compare value
	//		a. if greater return false, in valid move 
	//		b. if less than add to the list, need further checking for validity
	//		c. equal than ignore, no futher check needed


	//to store the values of cards in vector
    std::vector<int> selectedPosValue;
    
    //add hand value for build, since hand card would be part of the pair with table cards to get value = compareValue
    int handCardValue = cardStringToValue(player.getHandCard(handPosition));
    //check with valueCompare
    if(moveType == "b" && handCardValue < valueCompare) { selectedPosValue.push_back(handCardValue); }

    //loop to get each cards from the given positions
    for(int i = 0; i < tablePosition.size();i++) {

        //get the value of the card
        int tableValue;
        //get the index(position) of the table card
        int index = std::stoi(tablePosition.at(i));
        std::vector<std::string> currentTableCard = tableCards.at(index);
        

        //check for build or loose card
        if (currentTableCard.size() > 2) {

			tableValue = std::stoi(currentTableCard.at(0));
        	
        	if(moveType == "b"){
                //Check for Multi build for action build and return false since multi build cannot be further build into
                if(currentTableCard.at(2) == "Multi" && tableValue != valueCompare) { return false; }
                //check for build owned by the same player
                if((humanTurn && currentTableCard.at(1) == "Human") 
                    || (!humanTurn && currentTableCard.at(1) == "Computer")){
                    std::cout<<"..Invalid move to use own build to extend build.."<<std::endl;
                    return false;
                }
            }

        	//possible to capture table card with build only if build value = valueCompare
        	if(moveType == "c" && tableValue != valueCompare) { return false; }          

        } 
        else { tableValue = cardStringToValue(currentTableCard.at(1)); }

        //the given table value can never be greater than the valueCompare
        //only add values less than valueCompare
        if(tableValue > valueCompare) { return false; }
        else if(tableValue < valueCompare){ selectedPosValue.push_back(tableValue); }
        //else don't add to the list, equal
    }

    // 3. Sort tbe list of values of the selected table cards in descending order
    // 4. 	a. Get the first element(highset values) from the list
    //      for(size of list)
    //  	b. Add the given value with next elements in the list and check the sum for =< valueCompare
    // 	   	c. If equal then remove both the elements
    //   	d. If less than get another value from the list and add to the sum, check for condition
    // 		e. If greater than get next value
    // 5. repeat 4 until no element in the list --> valid move

    //sort the the table values list in descending order with respect to their values
    std::sort(selectedPosValue.rbegin(), selectedPosValue.rend());

    //std::cout<<"captureBuildSumChecker::Move Validation.."<<std::endl;
    //check for empty vector
    while (!selectedPosValue.empty()){
		
		//get the max value, the first value and erase the value from the vector
		int currentMax = selectedPosValue.at(0);
		selectedPosValue.erase(selectedPosValue.begin());

		//vector storing the remaining values to compare with current Max value
		std::vector<int> tempSelectedPosValue = selectedPosValue;
		//vector to store the vector returned after recursiveSumChecker call
		//if returned vector not empty --> pair found
		std::vector<int> returnedRemPosValue;

		//get tempSelectedPosValue 
        while(!tempSelectedPosValue.empty()){

        	//recursively finds the pair for current Max value
            returnedRemPosValue = recursiveSumChecker(currentMax,valueCompare,tempSelectedPosValue);
            
            //break since pair for current Max found
            if(returnedRemPosValue.size() != 0){ break; }

            //remove the first element of the tempSelectedPosValue
            //to get new element to compare with the current Max value
            tempSelectedPosValue.erase(tempSelectedPosValue.begin());
        }
        

        //check if the returned vector is empty after loop --> no pair found --> invalid move
        if(returnedRemPosValue.size() == 0) { return false;} 
        else {
            //valid values
            //find their poistion and remove from the list
            for(int i =0; i<returnedRemPosValue.size();i++){
           		//search the element
                std::vector<int>::iterator it = std::find(selectedPosValue.begin(), selectedPosValue.end(), returnedRemPosValue.at(i));
	            //removing the element
	            selectedPosValue.erase(it);

        	}

  		}
  	}

    return true;
}

/* *********************************************************************
    Function Name: recursiveSumChecker
    Purpose: utility function for captureBuildSumChecker that recursively checks for valid sets of card for given move
    Parameters:sum ,int to store the sum value --> 0 when capture and selected handValue when build
            valueCompare, int to store the value to compare
            valueSetTable, Vector<Integer> to store the values of selected table card
    Return Value: boolean, true if move valid
    Local Variables: tempValidValue, Vector to store the pair of valid card values
    Algorithm:
        1. For a given given card in table add its value to the sum
        2. Check if the sum value is equal, less or more than the valueCompare
        3. if equal add the value to the vector and return
        4. if less than remove the first element(the current value) and get new value and call recursiveSumChecker with
            new valueSetTable
        5. if more than return empty vector as a sign of invalid move

    Assistance Received: none
    ********************************************************************* */
std::vector<int> GameMoveCheck::recursiveSumChecker(int sum, int valueCompare, std::vector<int> selectedPosValue)
{

	//std::cout<<"recursiveSumChecker:: "<< sum <<" "<< valueCompare <<" ";
	//for (std::vector<int>::const_iterator i = selectedPosValue.begin(); i != selectedPosValue.end(); ++i) std::cout << *i << ' ';

	//empty vector
    std::vector<int> tempValidValue;

    //recursion when the sum is less the valueCompare
    //itr = valueSetTableNew.iterator();
    int currentLowerValue = selectedPosValue.at(0);
    int tempSum = sum + currentLowerValue;

    //only delete when found equal
    //and return true
    if(tempSum == valueCompare){
    	//std::cout<<":: = "<<std::endl;
        selectedPosValue.erase(selectedPosValue.begin());
        //returning the value that lead to the required sum
   		std::vector<int> temp;
        temp.push_back(currentLowerValue);
        return temp;
    }
    else if(tempSum < valueCompare){

    	//std::cout<<":: < "<<std::endl;

        //remove the first element from the temp selectedPosValue
        std::vector<int> tempValueSetTable = selectedPosValue;
        tempValueSetTable.erase(tempValueSetTable.begin());

        //loop until the list is empty
        while(!tempValueSetTable.empty()){
            tempValidValue = recursiveSumChecker(tempSum,valueCompare,tempValueSetTable);

            //Check for non empty vector
            //end of the recursion
            if(!tempValidValue.empty()){
                tempValidValue.push_back(currentLowerValue);
                return tempValidValue;
            }
            tempValueSetTable.erase(tempValueSetTable.begin());
        }
        //returns empty list
        return tempValueSetTable;
    }
    else{
    	//std::cout<<":: > "<<std::endl;

        //tempSum > valueCompare
        return tempValidValue;
    }
}

/* *********************************************************************
Function Name: generateStringBuildMove
Purpose: generates a vector of vector<string > to store the pairs forming the build
Parameters: buildValue, int build Value based on which the build info is generated
Return Value: void
Local Variables: buildCards, Vector of Vector where each element, vector stores the card and its value
    the variable is a list of all the buildable cards
Algorithm:
    1. Get each element and find the pair whose sum would equal to the build
    2. When found add the pair to Build info String
Assistance Received: none
********************************************************************* */
void GameMoveCheck::generateBuildPairs(int buildValue)
{
    //add value of build to the currentBuildPair
    std::vector<std::string> buildXInfo;
    //value of build
    buildXInfo.push_back(std::to_string(buildValue));
    //player info
    if(humanTurn) { buildXInfo.push_back("Human"); }
    else { buildXInfo.push_back("Computer"); }
    currentBuildPairs.push_back(buildXInfo);
    
    //get vector of table cards  
    std::vector<std::vector<std::string>> tableCards = gameTable.getAllCards();

    //Vector of vector for build
    //Vector of String for both card and its value, sorted 
    std::vector<std::vector<std::string>> buildCards;

    //store selected hand card and it's value in a vector 
    //store the vector in buildCards
    std::vector<std::string> tempCard;
    //get position card --> get int value --> convert to string --> store in vector
    tempCard.push_back( std::to_string(cardStringToValue(player.getHandCard(handPosition))) );
    tempCard.push_back(player.getHandCard(handPosition));
    buildCards.push_back(tempCard);

    //store all the selected table cards
    for(int i = 0; i<tablePosition.size();i++) { buildCards.push_back(tableCards.at(std::stoi(tablePosition.at(i)))); }

    //sorts the cards in ascending order based on the card value
    //starts building from highest value therefore highest values are removed first
    // in order to prevent removing lower indexed elements before higher ones
    sort(buildCards.begin(), buildCards.end(), compareInterval);

    //copy the value of cards from buildCards to selectedPosValue
    //required to find tha pair
    std::vector<int> selectedPosValueOG;
    std::vector<int> selectedPosValue;
    for(int i = 0; i<buildCards.size();i++) {
        //make a permanent copy of selectedPosValue since elements are erased when checking
        selectedPosValueOG.push_back(std::stoi(buildCards.at(i).at(0)));
        selectedPosValue.push_back(std::stoi(buildCards.at(i).at(0))); 
    }

    //std::cout<<"\ngenerateBuildPairs::Finding Build Pairs...\n";
    //check for empty vector
    while (!selectedPosValue.empty()){
        
        //get the max value, the first value and erase the value from the vector
        int currentMax = selectedPosValue.at(0);
        //removing any values equal to buildValue since they don't have pair
        while(currentMax == buildValue){

            //search the element and get the index
            int index = static_cast<int>(std::find(selectedPosValueOG.begin(), selectedPosValueOG.end(), currentMax)-selectedPosValueOG.begin());
            //mark that the position of the value is added
            //prevents double counting same element when using std::find
            selectedPosValueOG.at(index) = -1;

            //adding the single set of cards to currentBuildPairs
            //get the index -> access the element in buildCards --> store individual card into tempairs --> store tempPairs into currentBuildPairs
            //size of vector 
            int size = buildCards.at(index).size(); 
            
            //check for build and single cards
            if(size > 2){
                //builds
                //get the build type
                std::string buildType = buildCards.at(index).at(2);
                //loop through the vector containg build 
                for(int i = 3;i<size;i++){
                    
                    //vector to store the pair of cards
                    std::vector<std::string> tempPairs;
                    //get next element
                    //i++ increasing index to get next value
                    std::string card = buildCards.at(index).at(i);
                    //check for multi since they are already pairs,
                    //[ [CA C8] [C9] ] -- > two pairs

                    if(buildType == "Multi"){
                        
                        if(card == "["){
                            //get next card
                            i++;
                            card = buildCards.at(index).at(i);
                            //get all the cards within single build [..]
                            while(card != "]"){
                                //add the card to the vector
                                tempPairs.push_back(card);
                                //i++ --> for next iteration
                                i++;
                                //get the next card of the single build
                                card = buildCards.at(index).at(i);
                            }

                        } else {

                            //single cards whose value should equal to build value
                            //add them separately since they don't have pair
                            tempPairs.push_back(card);

                        }
                        //add the vector to thhe pair list
                        currentBuildPairs.push_back(tempPairs);

                    } else {
                        //Single Build
                        //break and store outside loop
                        //had problem using temPairs as local variable in this for loop
                        break;
                    }
                }

                //get the pairs for single build
                if(buildType == "Single"){ currentBuildPairs.push_back(addPairdsBuild(buildCards,index)); }

            } else { currentBuildPairs.push_back(addPairdsBuild(buildCards,index)); }

            //erase the first element since it equals buildValue
            selectedPosValue.erase(selectedPosValue.begin());
            //get new max value;
            currentMax = selectedPosValue.at(0);            
        }

        //removing the first element which is the current max value
        selectedPosValue.erase(selectedPosValue.begin());

        //vector storing the remaining values to compare with current Max value
        std::vector<int> tempSelectedPosValue = selectedPosValue;
        //vector to store the vector returned after recursiveSumChecker call
        //if returned vector not empty --> pair found
        std::vector<int> returnedRemPosValue;

        //get tempSelectedPosValue 
        while(!tempSelectedPosValue.empty()){

            //recursively finds the pair for current Max value
            returnedRemPosValue = recursiveSumChecker(currentMax,buildValue,tempSelectedPosValue);
            
            //break since pair for current Max found
            if(returnedRemPosValue.size() != 0){ break; }

            //remove the first element of the tempSelectedPosValue
            //to get new element to compare with the current Max value
            tempSelectedPosValue.erase(tempSelectedPosValue.begin());
            
        }

        //check if the returned vector is empty after loop --> no pair found --> error
        if(returnedRemPosValue.size() == 0) { std::cout<<"ERORR!!\n"; } 
        else {
            //valid values
            //pair up all the values that come up
            //search the element and get the index
            int index = static_cast<int>(std::find(selectedPosValueOG.begin(), selectedPosValueOG.end(), currentMax)-selectedPosValueOG.begin());
            //mark that the position of the value is added
            selectedPosValueOG.at(index) = -1;
            //get the card in vector
            std::vector<std::string> tempPairs1 = addPairdsBuild(buildCards,index);
                
            //find their poistion and remove from the list
            for(int i =0; i<returnedRemPosValue.size();i++){
                
                //search the element and get the index
                index = static_cast<int>(std::find(selectedPosValueOG.begin(), selectedPosValueOG.end(), returnedRemPosValue.at(i))-selectedPosValueOG.begin());
                //mark that the position of the value is added
                selectedPosValueOG.at(index) = -1;

                //get the vector with individual cards and merge to the local vector
                std::vector<std::string> tempPairs2 = addPairdsBuild(buildCards,index);
                tempPairs1.insert( tempPairs1.end(), tempPairs2.begin(), tempPairs2.end() );
                
                //get the iterator to erase from the selectedPosValue
                //since pair found erase the pairs since it helps in finding other pairs using recursion
                std::vector<int>::iterator it = std::find(selectedPosValue.begin(), selectedPosValue.end(), returnedRemPosValue.at(i));
                //removing the element
                selectedPosValue.erase(it);
            }

            //add the pair individual cards to the list of pair vectors
            currentBuildPairs.push_back(tempPairs1);
        }
    }
}

/* *********************************************************************
Function Name: addPairdsBuild
Purpose: //returns the vector with the card info from the buildCards, required by the build action 
        to generate pairs for build 
Parameters: std::vector<std::string>, cardA and cardBb
Return Value: bool
Local Variables: 
Algorithm:
Assistance Received: none
********************************************************************* */
std::vector<std::string> GameMoveCheck::addPairdsBuild(std::vector<std::vector<std::string>> buildCards,int index)
{
    //adding the single set of cards to currentBuildPairs
    //get the index -> access the element in buildCards --> store individual card into tempairs --> store tempPairs into currentBuildPairs
   std::vector<std::string> tempPairs;
    //size of vector 
    int size = buildCards.at(index).size(); 
    //check for build and single cards
    if(size > 2){
        //builds
        for(int i = 3;i<size;i++){
            std::string card = buildCards.at(index).at(i);
            //check for multi build brackets 
            if(card.size() == 2 ) tempPairs.push_back(card);
        }

    } else { tempPairs.push_back(buildCards.at(index).at(1)); }

    return tempPairs;
}

/* *********************************************************************
Function Name: compareInterval
Purpose: sorts the cards in descinding order based on the card value
Parameters: std::vector<std::string>, cardA and cardBb
Return Value: bool
Local Variables: 
Algorithm:
Assistance Received: none
********************************************************************* */
// Compares two intervals according to staring times. 
bool GameMoveCheck::compareInterval(std::vector<std::string> cardA, std::vector<std::string> cardB)
{ 
    //setting the value of card A and B depending loose or build card
    int valueCardA = std::stoi(cardA.at(0));
    int valueCardB = std::stoi(cardB.at(0));
    //ascending
    return (valueCardA > valueCardB);
}

/* *********************************************************************
Function Name: getPowerSet
Purpose: returns power set of all possible combination of table cards for a given hand card
Parameters: 
Return Value: Vector of Vector where each element is possible move
Local Variables:
Algorithm:
    1. Get the size of power set powet_set_size = pow(2, set_size)
    2. Loop for counter from 0 to pow_set_size
        (a) Loop for i = 0 to set_size
            (i) If ith bit in counter is set ith element from set for this subset

Assistance Received: none
********************************************************************* */
std::vector<std::vector<std::string>> GameMoveCheck::tablePositionPowerSet()
{

    //get the size of the table
    int set_size = gameTable.getTableSize();
    //adding index to vector and look for it's power set
    std::vector<std::string> tablePositionList;   
    for(int i =0; i<set_size;i++){ tablePositionList.push_back(std::to_string(i)); }

    //vector of vector to store the power set
    std::vector<std::vector<std::string>> tablePosPowerSet;

    //set_size of power set of a set with set_size n is (2**n-1)
    unsigned int pow_set_size = pow(2, set_size);

    //Run fr counter 000..0 to 111..1
    for(int counter = 0; counter < pow_set_size; counter++){
        std::vector<std::string> currentSet;
        for(int j=0;j<set_size;j++){
            //Check  if jth bit in the counter is set, 
            //if set then add jth element from set
            if(counter & (1<<j)) { currentSet.push_back(tablePositionList.at(j)); }
        }
        //adding the set to the powerSet
        tablePosPowerSet.push_back(currentSet);
    }

    //removving the first element which is the empty set
    tablePosPowerSet.erase(tablePosPowerSet.begin());

    return tablePosPowerSet;
}

/* *********************************************************************
Function Name: validCaptureList
Purpose: returns a list of all the valid moves for action Capture
Parameters: 
Return Value: Vector of Vector
Local Variables:
Algorithm:
    1. get tablePosition power set
    2. for each hand check if the moves are valid
    3. if valid store the move 
Assistance Received: none
********************************************************************* */
std::vector<std::vector<std::string>> GameMoveCheck::getValidCaptureList()
{

    //indicates the move type, required since further checking functions uses the variable
    this->moveType = "c";

    //list to store all the valid capture moves
    std::vector<std::vector<std::string>> validCaptureList;
    
    //get the power set
    std::vector<std::vector<std::string>> powerSet = tablePositionPowerSet();

    //get the local variable of the hand of the player 
    std::vector<std::string> playerHand = player.getAllHandCards();
    //loop through each element of the power set
    for(int i =0; i<powerSet.size();i++){
        //set the table memeber variable to the set
        tablePosition = powerSet.at(i);
        //loop through each hand position
        for(int j=0; j<playerHand.size();j++){
            //set the memmber variable handPosition
            handPosition = j;
            //get the hand card for given position and get the value
            std::string handCard = player.getHandCard(handPosition);
            int handCardValue = cardStringToValue(handCard);
            //check if the given move is valid, if valid --> store in vector

            //to temoporarily hold the valid positions
            std::vector<std::string> tempValidMove;            
            //check for valid move
            if(captureBuildSumChecker(handCardValue)){
                //add hand and table position to the list
                tempValidMove.push_back(std::to_string(handPosition));
                for(int k = 0; k<tablePosition.size();k++){ tempValidMove.push_back(tablePosition.at(k)); }
                //addding the valid move vector to the list
                validCaptureList.push_back(tempValidMove);
            }
        }
    }

    return validCaptureList;
}

/* *********************************************************************
Function Name: validBuildList
Purpose: returns a list of all the valid moves for action Build
Parameters: 
Return Value: Vector of Vector, 
    [buildValue] [hand pos] [table pos1]..
Local Variables:
Algorithm:
    1. get tablePosition power set
    2. for each hand check if the capture moves are valid
    3. if valid store the move 
Assistance Received: none
********************************************************************* */
std::vector<std::vector<std::string>> GameMoveCheck::getValidBuildList()
{

    //indicates the move type, required since further checking functions uses the variable
    this->moveType = "b";

    //list to store all the valid capture moves
    std::vector<std::vector<std::string>> validBuildList;
    //get the power set
    std::vector<std::vector<std::string>> powerSet = tablePositionPowerSet();
    //get the vector of hand cards
    std::vector<std::string> handCardList = player.getAllHandCards();
    
    //get the local variable of the hand of the player 
    std::vector<std::string> playerHand = player.getAllHandCards();
    //loop through each element of the power set
    for(int i =0; i<powerSet.size();i++){
        //set the table memeber variable to the set
        this->tablePosition = powerSet.at(i);
        //loop through each hand position
        for(int j=0; j<playerHand.size();j++){
            //set the memmber variable handPosition
            this->handPosition = j;
            //get the hand card for given position and get the value
            std::string handCard = player.getHandCard(handPosition);
            int handCardValue = cardStringToValue(handCard);
            /************/

            //moveCheck for build

            //get the vector of hand cards
            std::vector<std::string> handCardList = player.getAllHandCards();

            //flag to find if build valued card in hand
            bool buildValueFound = false;
            int buildValue = 0;
            //loop through all the hand cards to find the build valued hand card
            for(int k = 0; k<handCardList.size();k++ ){

                //check for selected hand card and ignore if found
                if(k == handPosition){ continue; }

                //build value equals the value of given Hand Card
                buildValue = cardStringToValue(handCardList.at(k));

                //check validity for given hand card
                if(buildValue >= handCardValue && captureBuildSumChecker(buildValue)) {
                    //found a hand card whose value is equal to build Value
                    buildValueFound = true;

                    //to temoporarily hold the valid positions
                    std::vector<std::string> tempValidMove;
                    tempValidMove.push_back(std::to_string(buildValue));
                    tempValidMove.push_back(std::to_string(handPosition));
                    for(int k = 0; k<tablePosition.size();k++){ tempValidMove.push_back(tablePosition.at(k)); }
                    //addding the valid move vector to the list
                    validBuildList.push_back(tempValidMove);

                    break;
                }
            }
        }
    }

    return validBuildList;
}

/* *********************************************************************
Function Name: getBestValidMove
Purpose: returns the best capture or build move
Parameters: isCapture, to decide capture or build
Return Value: Vector of Vector, in format:
    [hand pos] [table pos1].. { [buildValue] for build } [score] [number of cards]  
Local Variables:
Algorithm:
    1. get a list of valid capture or build moves depending upon boolean parameter
    2. calculate the best move based on score and number of cards
Assistance Received: none
********************************************************************* */
std::vector<std::string> GameMoveCheck::getBestValidMove(bool isCapture)
{
    
    //to store the list of all the valid moves
    std::vector<std::vector<std::string>> validMoveList;
    //stores the best valid move
    std::vector<std::string> bestValidMove;

    //check for capture or build
    if(isCapture){ validMoveList = getValidCaptureList(); } 
    else { validMoveList = getValidBuildList(); } 
    
    //get all the table cards
    std::vector<std::vector<std::string>> tableCards = gameTable.getAllCards();

    //highscore and num cards to check when looping
    int highestScore = -1;
    int prevNumCards = -1;

    //loop through the list of all the valid capture moves
    for(int i =0; i<validMoveList.size();i++){
        
        
        //to calculate the score for given move
        int score = 0;
        int currentNumCards = 0;
        //temp vector to store current valid move from the list
        std::vector<std::string> tempValidMove = validMoveList.at(i);


        //if build ignore the first element since it is the build value
        std::string buildValue = "-1";
        if(!isCapture) {
            buildValue = tempValidMove.at(0);
            tempValidMove.erase(tempValidMove.begin());
        }
    

        //get the hand card and add to the score
        std::string card = player.getHandCard(std::stoi(tempValidMove.at(0)));
        std::cout<<tempValidMove.at(0)<<card;
        currentNumCards++;
        score += scoreCalculator(card);
        std::cout<<score<<"_";
        

        //get each table position
        for(int j =1; j<tempValidMove.size();j++){
            
            //std::cout<<tempValidMove.at(j);
            
            //get table card vector
            std::vector<std::string> cardVector = tableCards.at(std::stoi(tempValidMove.at(j)));
            
            //check for build or single card
            if(cardVector.size()>2){
                
                for(int k =3; k<cardVector.size();k++){
                    card = cardVector.at(k);
                    if( card!="[" && card!="]") {
                        std::cout<<card;
                        score += scoreCalculator(card);
                        std::cout<<score<<" ";
                        currentNumCards++;
                    }
                }

            } else {
                card = cardVector.at(1);
                std::cout<<card;
                score += scoreCalculator(card);
                std::cout<<score<<" ";
                currentNumCards++;
            }

            
        }
        //std::cout<<"\n";
        if(score > highestScore || (score == highestScore && currentNumCards > prevNumCards) ){
            
            //set new highScore
            highestScore = score;
            //set new best move
            bestValidMove = tempValidMove;
            
            //add back the build value for build
            if(!isCapture) { bestValidMove.push_back(buildValue);}
            
            //store the num cards, prevNumCards required to check for moves having same score
            //Changing the prevNumCards when new score is updated allows the numCard comparision
            //  only when the score are equal
            prevNumCards = currentNumCards;
        }
        
        std::cout<<"finalScore = "<<score<<" currentNumCards:"<<currentNumCards<<" prevNumCards:"<<prevNumCards<<" HighScore = "<<highestScore<<"\n\n";
    }
    
    //add the score at the end of the vector
    bestValidMove.push_back(std::to_string(highestScore));
    //add the number of cards 
    bestValidMove.push_back(std::to_string(prevNumCards));
    
    return bestValidMove;
}

/* *********************************************************************
Function Name: getBestValidCaptureMove
Purpose:  Check the card for aces and DX and S2 and calculates the score
Parameters: 
Return Value: Vector of Vector 
Local Variables:
Algorithm:
Assistance Received: none
********************************************************************* */
int const GameMoveCheck::scoreCalculator(std::string card)
{
    int score = 0;
    if( card == "CA" || card == "DA" || card == "SA" || card == "HA" || card == "S2") { score = 1;}
    else if(card == "DX"){ score = 2;}
    return score;
}

/* *********************************************************************
Function Name: tablePosition
Purpose: sets the table and hand position
Parameters: int, handPosition
    std::vector<std::string>, tablePosition
Return Value: void
Local Variables:
Algorithm:
Assistance Received: none
********************************************************************* */
void GameMoveCheck::setPosTableHand(int handPosition, std::vector<std::string> tablePosition){
    this->handPosition = handPosition;
    this->tablePosition = tablePosition;
}