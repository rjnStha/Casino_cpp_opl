#include "GameEngine.h"

//Constructor 
//ask user for load, new game or exit
GameEngine::GameEngine() { homeScreen(); }

/* *********************************************************************
Function Name: newDealCards
Purpose: gets 4 cards from deck to player hands and table
Parameters: dealTable, boolean to decide whether to deal cards in table
Return Value: void
Local Variables:
Algorithm:
Assistance Received: none
********************************************************************* */
void GameEngine::newDealCards(bool dealTable) 
{

	//Distribute card from the deck to table and hands
	if (dealTable) { for (int i = 0; i < 4; i++) { gameTable.storeTable(gameDeck.getNewCard()); } }
	for (int i = 0; i < 4; i++) { computer.storeHand(gameDeck.getNewCard()); }
	for (int i = 0; i < 4; i++) { human.storeHand(gameDeck.getNewCard()); }
}

/* *********************************************************************
Function Name: printGameScreen
Purpose: prints the home prompt and get the numeric action code from user
	to either load , exit or play a new game
Parameters: none
Return Value: 
Local Variables:
Algorithm:
Assistance Received: none
********************************************************************* */
void const GameEngine::homeScreen() 
{

	//create new instances of member variables for each Home Screen call
	gameTable = Table();
	human = Human() ;
	computer = Computer();
	round = 0;
	lastCapturer = "None";

	//to store the input form user
	int optionValue;
	std::cout << "********\t*********\n";
	std::cout << "\tCASINO\n";
	std::cout << "********\t*********\n";
	std::cout << " *Action*\t*NumCode*\n New Game\t  0\n Load Game\t  1\n Exit\t\t  -1\n";
	std::cout << "********\t*********\n";
	do {
		std::cout << "Enter valid NumCode(0,1,-1): ";
		std::cin >> optionValue;
		std::cout << "********\t*********\n\n";
	} while (optionValue > 1 && optionValue < -1);

	//Check for user input and set variables and print promts for respective action
	switch (optionValue) {

		case 0: 
		{
			//New Game
			std::cout << "New Game ...\n\n";

			//create new instances of member variables
			gameDeck = Deck(true);
					
			//prints all cards in Deck
			gameDeck.printDeckCards();
			//deals 3 set of 4 cards
			newDealCards(true);

			//coin toss do until h or 
			coinToss();
			
			break;
		}

		case 1:
		{
			
			//Load Game
			//create empty Deck
			gameDeck = Deck(false);

			//get the name of the file from the user
			std::string fileName = getLoadGameFile();
			
			if(fileName == "EXIT") { 
				std::cout<<" Exiting to Home Menu\n "<<std::endl;
				homeScreen(); 

			} 
			else { loadGame(fileName); }

			break;
		}

		default: {return;}
	}

	//infinite loop
	int feedBackMove;
	while (true) {

		//check for changes for dealing cards, new round and end of the tournament
		if(checkGameChanges()) {
			std::cout<<" Exiting to Home Menu\n "<<std::endl;
			homeScreen();
			break;
		}

		//check for human or computer move
		if (humanTurn) { 
			feedBackMove = humanMove();
			//successful move
			//change turn to computer
			if(feedBackMove == 0) { humanTurn = false; }

		} else { 
			computerMove(); 
			feedBackMove = 0;
		}
		
		//recurse if exit called
		if(feedBackMove == -1) {
			std::cout<<" Exiting to Home Menu\n "<<std::endl;
			homeScreen();
			break;
		}

		else if(feedBackMove == 1) {
			//inValid move
			std::cout << "\n Please Enter A Valid Move!!! \n\n";
		}
	}
}

/* *********************************************************************
Function Name: printGameBoard
Purpose: prints all the cards in table and player's hand in systematic layout
Parameters: none
Return Value: unsigned int, NumCOde for user selected action
Local Variables:
Algorithm:
Assistance Received: none
********************************************************************* */
void const GameEngine::printGameBoard()
{
	gameDeck.printDeckCards();

	std::cout << " |--------------------------------\n";

	//Display the round
	std::cout << " | Round: "<<round<<"\n";
	std::cout << " |\n";

	//Display the round
	std::cout << " | Last Capturer: "<<lastCapturer<<"\n";
	std::cout << " |\n";

	//score
	std::cout << " | \t: ";
	std::cout << "Score: " << computer.getScore() << std::endl;

	//Display the comp hand 
	std::cout << " | Comp\t: ";
	computer.printHandOrPile(true);

	//Display the comp pile 
	std::cout << " | \t: ";
	computer.printHandOrPile(false);

	std::cout << " |\n";

	//Display table cards
	std::cout << " | Table: ";
	gameTable.printTable();
	std::cout << " |\n";

	//score
	std::cout << " | \t: ";
	std::cout << "Score: " << human.getScore() << std::endl;
	
	//Display human hand
	std::cout << " | Human: ";
	human.printHandOrPile(true);

	//Display hand pile
	std::cout << " | \t: ";
	human.printHandOrPile(false);

	std::cout << " |--------------------------------\n\n";
}

/* *********************************************************************
Function Name: coinToss
Purpose: random coin toss and check the result to the human response and sets the turn
Parameters:
Return Value: void
Local Variables:
x, boolean that stores the randomized value of 0 or 1
Algorithm:
1) user input head or tail
2)randomize between 0 and 1
3) compare to the user input
4) print the result
Assistance Received: none
********************************************************************* */
void GameEngine::coinToss()
{
	char coinTailHead;
	do
	{
		std::cout << "****************************************\n";
		std::cout << std::endl;
		std::cout << "Coin toss, head or tail? h for head and t for tail: ";
		std::cin >> coinTailHead;
		std::cout << "****************************************\n\n";

	} while (coinTailHead != 'h' && coinTailHead != 'H' &&coinTailHead != 't' && coinTailHead != 'T');


	//Random get the coin side
	srand(time(NULL));
	bool x = rand() % 2;

	//compare the coin toss with user guess
	if ((x == 0 && coinTailHead == 'h') || (x == 1 && coinTailHead == 't'))
	{
		humanTurn = true;
		std::cout << x << ": YOU GUESSED RIGHT!! It is human's turn" << std::endl;
	}
	else
	{
		humanTurn = false;
		std::cout << x << ": YOU GUESSED WRONG!! It is computer's turn" << std::endl;
	}
	std::cout << std::endl;
}

/* *********************************************************************
Function Name: humaMove
Purpose: carries out human move, also handles save game
Parameters: none
Return Value: error code with 0 as move completed, a interger value
Local Variables: none
Algorithm:
1) Ask the card from user hand and whether to capture, trial or build
2) Call the required function for capture, trial or build
Assistance Received: none
********************************************************************* */
int GameEngine::humanMove()
{
	//prompt human turn
	std::cout << "****************************************\n";
	std::cout << "************* Human's turn *************\n";
	std::cout << "****************************************\n";
	//print the player and table cards
	printGameBoard();

	//vector to store the user input move list
	//also stores exit and save instruction if called by user
	std::vector<std::string> moveInfo = getHumanMoveInfo();

	//Exit if exit called by the user
	if(moveInfo.at(0) == "EXIT") return -1;
	else if(moveInfo.at(0) == "SAVE") {
		//save game
		std::string fileName;
		std::cout<<" Save Game... \n";
		std::cout << "****************************\n";
		std::cout<<" Enter the filename: ";
		//ask user for file name
		std::cin>>fileName;
		std::cout << "****************************\n";
		//save the game
		if(saveGame(fileName)) { std::cout<<"\n Saved to file "<<fileName<<".txt\n\n"; }
		//exit
		return -1;

	} else if(moveInfo.at(0) == "HELP"){

		std::cout << "************** HELP MODE ***************\n";

		GameMoveCheck moveCheck = GameMoveCheck(humanTurn,gameTable,human);

		//get the best valid capture and build move
		std::vector<std::string> bestValidCaptureMove = moveCheck.getBestValidMove(true);
		std::vector<std::string> bestValidBuildMove = moveCheck.getBestValidMove(false);
		
		if(bestValidCaptureMove.size() == 2 && bestValidBuildMove.size() == 2){
			//no valid cpature or build move
			//trail the first card
			std::cout<<"Trail: No better move.. \n\t";

		}else{

			//get the num of cards to compare
			int numCardCapture = std::stoi(bestValidCaptureMove.at(bestValidCaptureMove.size()-1));
			int numCardBuild = std::stoi(bestValidBuildMove.at(bestValidBuildMove.size()-1));

			//remove the info from the vector
			bestValidCaptureMove.pop_back();
			bestValidBuildMove.pop_back();

			//get the score of cards to compare
			int scoreCapture = std::stoi(bestValidCaptureMove.at(bestValidCaptureMove.size()-1));
			int scoreBuild = std::stoi(bestValidBuildMove.at(bestValidBuildMove.size()-1));
			//remove the info from the vector
			bestValidCaptureMove.pop_back();
			bestValidBuildMove.pop_back();

			//get the build value and erase from the bestValidBuildMove
			//check size since invlaid build move lacks buildValue info
			int buildValue = -1;
			if(bestValidBuildMove.size()!=0){
				buildValue = std::stoi(bestValidBuildMove.at(bestValidBuildMove.size()-1));
				bestValidBuildMove.pop_back();
			}
			std::cout << "\n------------------------\n\n";
			//pompt for capture
			std::cout<<"The best capture move::\n\t[hanPos] [tablePos1].. : ";
			for(int i = 0; i<bestValidCaptureMove.size();i++ ){ std::cout<<bestValidCaptureMove.at(i)<<" ";}
			std::cout<<"Score: "<<scoreCapture<<" NumCards: "<<numCardCapture<<"\n\n";

			//pompt for build
			std::cout<<"The best build move::\n\t[hanPos] [tablePos1].. : ";
			for(int i = 0; i<bestValidBuildMove.size();i++ ){ std::cout<<bestValidBuildMove.at(i)<<" ";}
			std::cout<<"Score: "<<scoreBuild<<" NumCards: "<<numCardBuild<<"\n\n";

			//variable required to execute the move
			int handPosition;
			std::vector<std::string> tableCardPosition;
			std::vector<std::vector<std::string>> buildPairs;

			std::cout<<"The best move: ";
			if(scoreCapture > scoreBuild || (scoreCapture == scoreBuild && numCardCapture >= numCardBuild)){ std::cout<<"Capture\n"; }
			else { std::cout<<"Build\n"; }

		}

		std::cout << "\n****************************************\n";

		//sleep
		std::this_thread::sleep_for(std::chrono::seconds(2));
		
		//though 1 is for invalid move, using the numCode would lead back to asking user 
		return 1 ;

	} else{
		//add the player turn, required by GameMoveCheck
		moveInfo.push_back("Human");
		
		//check if the move is valid
		GameMoveCheck moveCheck = GameMoveCheck(moveInfo,gameTable,human);
		if(!moveCheck.moveCheck()) { return 1; }

		//Valid move
		std::cout << "\nThe move is VALID .. Executing the move!!! \n\n";

		//Check for move type and valid move
		//trail
		if (moveCheck.getMoveType() == "t") { 
			trail(moveCheck.getHandPosition()); 
		}
		else if (moveCheck.getMoveType() == "c") { 
			//capture
			capture(moveCheck.getHandPosition(),moveCheck.getTablePosition());
	        //Setting the last capturer to the given turn
			lastCapturer = "Human";
		
		}else{
			//build
			build(moveCheck.getHandPosition(),moveCheck.getTablePosition(),moveCheck.getCurrentBuildPairds());
		}

		//sleep
		std::this_thread::sleep_for(std::chrono::seconds(2));

		return 0;
	}
}

/* *********************************************************************
Function Name: computerMove
Purpose: carries out computer move
Parameters: none
Return Value: error code with 0 as move completed, a interger value
Local Variables: none
Algorithm:
1) Call checkMoves to get set of possible moves
2) Calculate the optimum move, with max card values
3) Call the required function for capture, trial or build
Assistance Received: none
********************************************************************* */
int GameEngine::computerMove()
{
	std::cout << "****************************************\n";
	std::cout << "************* Computer's turn **********\n";
	std::cout << "****************************************\n";
	printGameBoard();

	//check if the move is valid
	GameMoveCheck moveCheck = GameMoveCheck(humanTurn,gameTable,computer);

	//get the best valid capture and build move
	std::vector<std::string> bestValidCaptureMove = moveCheck.getBestValidMove(true);
	std::vector<std::string> bestValidBuildMove = moveCheck.getBestValidMove(false);
	
	if(bestValidCaptureMove.size() == 2 && bestValidBuildMove.size() == 2){
		//no valid cpature or build move
		//trail the first card
		std::cout<<"Trailing first element, no better move.. \n\t";
		trail(0);

	}else{

		//get the num of cards to compare
		int numCardCapture = std::stoi(bestValidCaptureMove.at(bestValidCaptureMove.size()-1));
		int numCardBuild = std::stoi(bestValidBuildMove.at(bestValidBuildMove.size()-1));
		//remove the info from the vector
		bestValidCaptureMove.pop_back();
		bestValidBuildMove.pop_back();

		//get the score of cards to compare
		int scoreCapture = std::stoi(bestValidCaptureMove.at(bestValidCaptureMove.size()-1));
		int scoreBuild = std::stoi(bestValidBuildMove.at(bestValidBuildMove.size()-1));
		//remove the info from the vector
		bestValidCaptureMove.pop_back();
		bestValidBuildMove.pop_back();

		int buildValue = -1;
		if(bestValidBuildMove.size()!=0){
			buildValue = std::stoi(bestValidBuildMove.at(bestValidBuildMove.size()-1));
			bestValidBuildMove.pop_back();
		}
		
		std::cout << "\n------------------------\n\n";
		
		//pompt for capture
		std::cout<<"The best capture move::\n\t[hanPos] [tablePos1].. : ";
		for(int i = 0; i<bestValidCaptureMove.size();i++ ){ std::cout<<bestValidCaptureMove.at(i)<<" ";}
		std::cout<<"Score: "<<scoreCapture<<" NumCards: "<<numCardCapture<<"\n\n";

		//pompt for build
		std::cout<<"The best build move::\n\t[hanPos] [tablePos1].. : ";
		for(int i = 0; i<bestValidBuildMove.size();i++ ){ std::cout<<bestValidBuildMove.at(i)<<" ";}
		std::cout<<"Score: "<<scoreBuild<<" NumCards: "<<numCardBuild<<"\n\n";

		//variable required to execute the move
		int handPosition;
		std::vector<std::string> tableCardPosition;
		std::vector<std::vector<std::string>> buildPairs;

		std::cout<<"The best move: ";
		if(scoreCapture > scoreBuild || (scoreCapture == scoreBuild && numCardCapture >= numCardBuild)){
			std::cout<<"Capture\n";
			
			handPosition = std::stoi(bestValidCaptureMove.at(0));
			bestValidCaptureMove.erase(bestValidCaptureMove.begin());
			tableCardPosition = bestValidCaptureMove;
			
			//calling the capture function
			capture(handPosition,tableCardPosition);
			//Setting the last capturer to the given turn
			lastCapturer = "Computer";
	

		}else {
			std::cout<<"Build\n";
			//get the hand position and erase from the vector
			handPosition = std::stoi(bestValidBuildMove.at(0));
			bestValidBuildMove.erase(bestValidBuildMove.begin());
			//store the rem vector as table pos
			tableCardPosition = bestValidBuildMove;
			//set the hand and table position and generate build pairs
			moveCheck.setPosTableHand(handPosition,tableCardPosition);
			moveCheck.generateBuildPairs(buildValue);

			std::vector<std::vector<std::string>> a = moveCheck.getCurrentBuildPairds();
			//execute build
			build(handPosition,tableCardPosition,moveCheck.getCurrentBuildPairds());
		}

	}
	std::cout<<"\tExecuting the move!!!..\n\n";
	//sleep
	std::this_thread::sleep_for(std::chrono::seconds(2));
	humanTurn = true;
	return 0;
}

/* *********************************************************************
Function Name: getHumanMoveInfo
Purpose: user input the action, hand position and table position/s
Parameters: none
Return Value: vector<string> that stores the move info in
format [action] [hand] [table1] [table2]..
Local Variables: none
Algorithm:
Assistance Received: none
********************************************************************* */
std::vector<std::string> GameEngine::getHumanMoveInfo() 
{
	//local variable to store the move input by the human
	std::vector<std::string> moveInfo;

	//user input for selection of action: Capture,Build or Trial
	std::string optionChoice;
	do
	{
		std::cout << "****************************************\n";
		std::cout << "* Enter -1 to Exit and -2 to Save the Game * \n";
		std::cout << "* Enter -3 for Help Mode * \n";	
		std::cout << " Action: Enter c for Capture, b for build and t for Trial: ";
		std::cin >> optionChoice;
		std::cout << "****************************************\n\n";

	} while (optionChoice != "c" && optionChoice != "b" && optionChoice != "t" 
		&& optionChoice != "-1" && optionChoice != "-2" && optionChoice != "-3");

	//option to exit or save
	if(optionChoice == "-1"){
		moveInfo.clear();
		moveInfo.push_back("EXIT");
		return moveInfo;

	} else if(optionChoice == "-2"){
		//save
		moveInfo.clear();
		moveInfo.push_back("SAVE");
		return moveInfo;
	} else if(optionChoice == "-3"){
		//save
		moveInfo.clear();
		moveInfo.push_back("HELP");
		return moveInfo;
	}


	//add the action info in moveInfo
	moveInfo.push_back(optionChoice);

	//Input the position of the human hand card
	int handPosition;
	do
	{
		std::cout << "****************************************\n";
		std::cout << "* Enter -1 to Exit and -2 to Save the Game * \n";	
		std::cout << "* First element at position 0 * \n\n Hand: Enter the position of the card in your hand: ";
		std::cin >> handPosition;
		std::cout << "****************************************\n\n";
	} while (handPosition >= human.getHandSize() || handPosition < -2);

	//option to exit or save
	if(handPosition == -1){
		moveInfo.clear();
		moveInfo.push_back("EXIT");
		return moveInfo;

	} else if(handPosition == -2){
		//save
		moveInfo.clear();
		moveInfo.push_back("SAVE");
		return moveInfo;
	}


	//add the hand position in moveInfo
	moveInfo.push_back(std::to_string(handPosition));

	//get the position of the table cards for capture and build action
	if (optionChoice != "t") {

		int tablePosition = 0;
		do {
			//validate and input positions for each table selection
			do
			{
				std::cout << "****************************************\n";
				std::cout << "* Enter -1 to Exit and -2 to Save the Game * \n";	
				std::cout << "* Enter -9 if done selecting:  * \n";
				std::cout << "* First element at position 0 * \n";
				std::cout << "* Enter atleast one table card position * \n\n Table: Enter the position of the card in the table: ";
				std::cin >> tablePosition;
				std::cout << "****************************************\n";
				if (tablePosition == -9) { break; }

			} while (tablePosition >= gameTable.getTableSize() || tablePosition < -2);

			//option to exit or save
			if(tablePosition == -1){
				moveInfo.clear();
				moveInfo.push_back("EXIT");
				return moveInfo;

			} else if(tablePosition == -2){
				//save
				moveInfo.clear();
				moveInfo.push_back("SAVE");
				return moveInfo;
			}

			//Check for repetive table position
			if (moveInfo.size() > 2) {
				bool multipleTablePosition = false;
				for (int i = 2; i < moveInfo.size(); i++) {
					if (moveInfo.at(i) == std::to_string(tablePosition)) {
						multipleTablePosition = true;
						break;
					}
				}

				if (multipleTablePosition) {
					std::cout << "\n\t !!The table postion already selected!!\n\n";
					continue;
				}
			}

			//check if atleast one table position is added
			if (tablePosition == -9 && moveInfo.size() == 2) {
				std::cout << "\n\t !!Select atleast one card from the table!!\n\n";
				continue;
			}

			//Exit if -1
			if (tablePosition == -9) {
				std::cout << "\n!!Move Selection Complete!! \n\n";
				break;
			}

			//add the table position in moveInfo
			moveInfo.push_back(std::to_string(tablePosition));

			//printing the move Info
			std::cout << "\n MoveInfo ( [action] [hand] [table1] [table2].. ) : ";
			for (int i = 0; i < moveInfo.size(); i++) { std::cout << moveInfo.at(i) << " "; }
			std::cout << "\n\n";

		} while (!(tablePosition == -9 && moveInfo.size() > 2));
	}

	//printing the move info
	std::cout << "MoveInfo ( [action] [hand] [table1] [table2].. ) : ";
	for (int i = 0; i < moveInfo.size(); i++) { std::cout << moveInfo.at(i) << " "; }
	std::cout << "\n\n";

	return moveInfo;
}

/* *********************************************************************
Function Name: trail
Purpose: carries out trail action for human or computer with given hand position
Parameters: handPosition, integer that stores the hand position to trail
Return Value: bool, true if successful move
Local Variables:
Algorithm:
Assistance Received: none
********************************************************************* */
bool GameEngine::trail(int handPosition) 
{

	//Check human or computer
	//get the card from hand and store in the table board
	if (humanTurn) { gameTable.storeTable(human.popHandCard(handPosition)); }
	else { gameTable.storeTable(computer.popHandCard(handPosition)); }

	return true;
}

/* *********************************************************************
Function Name: capture
Purpose: carries out capture action for human or computer with given hand position and table card position
Parameters:
    handPosition, integer that stores the hand card position to trail
    tableCardPosition, Vector storing the selected table card position
Return Value: String, info about the move
Local Variables: currentPlayer, Player to decide human or computer and use the variable to carry capture action
Algorithm:
Assistance Received: none
********************************************************************* */
bool GameEngine::capture(int handPosition, std::vector<std::string> tableCardPosition){
 	//since Capture involves deleting elements from table
    //arrange the tableCardPosition in descending order
    //prevents removing less indexed element before higher ones
    std::sort(tableCardPosition.rbegin(), tableCardPosition.rend());

    //get the current player
    Player *currentPlayer;
    if(humanTurn) {currentPlayer = &human;}
    else {currentPlayer = &computer;}
	
	//remove the card from hand and store into human pile
    currentPlayer->storePile(currentPlayer->popHandCard(handPosition));

    //storing selected table cards into human pile
    for(int i = 0; i<tableCardPosition.size(); i++){

        int currentTableCardPos = std::stoi(tableCardPosition.at(i));

        //check for build cards
        if(gameTable.getAllCards().at(currentTableCardPos).size() > 2){

            //local variable to store the vector with buildInfo
            std::vector<std::string> buildCard = gameTable.popTableCard(currentTableCardPos);
            
            //get each card from the build
            for(int j = 0; j<buildCard.size(); j++){ currentPlayer->storePile(buildCard.at(j)); }

        } else{

            //loose card
            currentPlayer->storePile(gameTable.popTableCard(currentTableCardPos).at(0));
        }

    }

	return true;
}

/* *********************************************************************
Function Name: build
Purpose: carries out build action for human or computer with given hand position,table card position and build pairs
Parameters:
    handPosition, integer that stores the hand card position to trail
    tableCardPosition, Vector storing the selected table card position
    buildPairs, vector storing vector of strings, each vector string contains the pair cards for forming a build
Return Value: String, info about the move
Local Variables: currentPlayer, Player to decide human or computer and use the variable to carry capture action
Algorithm:
Assistance Received: none
********************************************************************* */
bool GameEngine::build(int handPosition, std::vector<std::string> tableCardPosition, std::vector<std::vector<std::string>> buildPairs){
	//since build involves deleting elements from table
    //arrange the tableCardPosition in descending order
    //prevents removing less indexed element before higher ones
    std::sort(tableCardPosition.rbegin(), tableCardPosition.rend());
    //removing selected table cards
    for(int i = 0; i<tableCardPosition.size(); i++){ gameTable.popTableCard(std::stoi(tableCardPosition.at(i))); }
    
    //removing the hand cards from the player 
    if(humanTurn) {human.popHandCard(handPosition);}
    else {computer.popHandCard(handPosition);}
    
	//storing the build info as string to store in table
	std::vector<std::string> buildCurrent;

	//value
	buildCurrent.push_back(buildPairs.at(0).at(0));
	//owner
	buildCurrent.push_back(buildPairs.at(0).at(1));
	//Multi or Single, multi as default, change to single when required in the loop below
	buildCurrent.push_back("Multi");
	//remove the first element which is the information of value and owner
	buildPairs.erase(buildPairs.begin());

	//replace the multi
	if(buildPairs.size() == 1){ buildCurrent.at(2) = "Single";}
	
	//loop to get each pair
	for(int i=0;i<buildPairs.size();i++){
        
        std::vector<std::string> pairCurrent = buildPairs.at(i);
        //if single card then 
        if(pairCurrent.size() == 1){
        	buildCurrent.push_back(pairCurrent.at(0));
        	continue;
        }

        //store "[" as start of single builds
        if(buildCurrent.at(2) == "Multi") buildCurrent.push_back("[");
        //for pairs access each of them and store in the table
        for(int j=0;j<pairCurrent.size();j++){ buildCurrent.push_back(pairCurrent.at(j)); }
        //store "]" as the end of single builds
        if(buildCurrent.at(2) == "Multi") buildCurrent.push_back("]");
	}

	//store the build vector in table
	gameTable.storeTable(buildCurrent);
	return true;
}

/* *********************************************************************
Function Name: getLoadGameFile
Purpose: list the files in the serialization folder and gets the user key for opening
	the file
Parameters: none
Return Value: string, file name with path
Local Variables:
Algorithm: 
	1)list the files in the serilization folder
	2)store each fileName in vector with respective key = index
	3) userInput gives the index in the vector
	4)return the fileName with including the path
Assistance Received: none
********************************************************************* */
std::string GameEngine::getLoadGameFile()
{

	//load game menu 
	std::cout << "********\t*********\n";
	std::cout << "\tLOAD GAME\n";
	std::cout << "********\t*********\n";
	std::cout << " *FileName*\t*NumCode*\n";
	
	//store the fileNames for later to match with user input 
	unsigned int index = 0;
	std::vector<std::string> listFile;
	std::string fileName;

	//listing the files
	struct dirent *entry;
	DIR *dir = opendir("/home/rshresth/00OPL/proj1Cpp/src/serializationTest");
   	if (dir == NULL) { return "fail"; }

	while ((entry = readdir(dir)) != NULL) {
		fileName = entry->d_name;
		if(fileName.length()>4){
			listFile.push_back(fileName);
			std::cout << " "<<fileName <<"\t\t"<<index<<std::endl;
   			index++;
   		}
   	}
   	closedir(dir);
   	std::cout << "********\t*********\n";
	
	int optionValue;
	while(true) {
		std::cout << "Enter valid NumCode,-1 to Exit: ";
		std::cin >> optionValue;
		std::cout << "********\t*********\n\n";
		if (optionValue == -1 || (optionValue <= index-1 && optionValue > -2) ) { break; }

	} 

   	if(optionValue != -1){
	   	//get the user input for the fileName
	   	fileName = "/home/rshresth/00OPL/proj1Cpp/src/serializationTest/";
	   	fileName += listFile.at(optionValue);
	   	return fileName;
	}

   	return "EXIT";
}

/* *********************************************************************
Function Name: loadGame
Purpose: loads game of given filename
Parameters: fileName, String name of the file to load
Return Value: boolean, check whether load was success
Local Variables: many..and not so important
Algorithm:
1. Get a line form the file and parse the line
2. Check for different labels and carry out required actions to store the info
Assistance Received: none
********************************************************************* */
bool GameEngine::loadGame(std::string fileName) 
{

	//Empty string
	std::string label;
	std::string data;
	bool loadHuman = false;

	try {
		std::ifstream infile(fileName);
		std::string line;

		while (std::getline(infile, line))
		{
			std::istringstream iss(line);
			std::string delimiter = ": ";
			label = line.substr(0, line.find(delimiter));
			data = line.substr(line.find(delimiter)+1);

			//Check the label
			if (label == "Round") {
				//Round
				// converting string to int using string stream
				std::stringstream ss(data);
				ss >> round;

			} else if (label == "Computer" || label == "Computer:") { 
				//Computer data
				loadHuman = false; 

			} else if (label == "Human" || label == "Human:") { 
				//Human Data
				loadHuman = true;

			} else if (label == "   Score") {
				//Score
                int score;
            	std::stringstream ss(data);
				ss >> score;
                
                //check human/computer and set the score of the players
                if (loadHuman) { human.setScore(score); } 
                else { computer.setScore(score); }
           	
           	} else if (label == "   Hand") {

                //Hand
                std::stringstream ss(data);
                std::string card;
                while(ss >> card) {
                	//add each card to the hand of respeective player
                	if (loadHuman) { human.storeHand(card);}
                	else { computer.storeHand(card); }
                }
            } else if (label == "   Pile") {

                //Pile
                std::stringstream ss(data);
                std::string card;
                while(ss >> card) {
                	//add each card to the hand of respeective player
                	if (loadHuman) { human.storePile(card);}
                	else { computer.storePile(card); }
                }

            } else if (label == "Table") {

            	//Table
            	std::stringstream ss(data);
                std::string card;
                
                //loop to get single word in a string
                while(ss >> card) {
                	
                	//ignore build cards and only add single cards
                	if (card == "[") { do { ss >> card; }while (card != "]"); } 
                	else if (card.at(0) == '[') { do { ss >> card; } while (card.at(2) != ']'); } 
                	else { gameTable.storeTable(card); }

                }
			
			} else if (label == "Build Owner") {
				
				//build ownership
                // Parsing build since ignored build cards at label "Table"
                //call store build function
                if(!storeBuildTable(data)) return false;
			
			
			} else if (label == "Last Capturer") {

        		std::stringstream ss(data);
        		ss>>lastCapturer;
            
            } else if (label == "Deck") {

            	std::stringstream ss(data);
                std::string card;
                //storing each card in deck
                while(ss >> card) { gameDeck.storeDeck(card); }
               
        	} else if (label == "Next Player") {

				std::string turn;
        		std::stringstream ss(data);
        		ss>>turn;

        		//Player turn
                //Check if the next turn is computer or human
                if (turn == "Computer") { humanTurn = false; } 
                else { humanTurn = true; }

        	}




		}

	}
	catch (int e) {

		//Unable to open file
		//Error reading file
		//Displaying the turn of the player
		return false;

	}
	std::cout<<"\t* "<<fileName<<" Load Complete.. *\n";
	gameDeck.printDeckCards();

	return true;
}

/* *********************************************************************
Function Name: storeBuildTable
Purpose: stores one build multi or single into the table for human and computer
Parameters: data, String the info of build in format --> [ [DX] [H9 SA] ] Human
Return Value: boolean, true for successful store of build
Local Variables:
Algorithm: Parse the string data using delimiter "[" and "]"
Assistance Received: none
********************************************************************* */
bool GameEngine::storeBuildTable(std::string data)
{

	//to store all the data in the build
	std::vector<std::string> buildCards;
	
	//to store the value of the build
    int buildValue = 0;
    //required to differentiate multi and single build and while calculating the build value
    int start;
    int end;

	std::stringstream ss(data);
    std::string card;
    
    //get a word from the stream
    ss >> card;
    	
	if (card == "[") {
		
		//multi build
		
		buildCards.push_back ("Multi");

	    //get next word from the stream
	    ss >> card;

        //6 types of card info
        // [ , [S8 , S8, S8], [S8] , ]
        //but ony [S8] and [S8 starts the single build

        //loop until the end of the build
        while (card != "]") {

        	//type [S8
            if (card.length() == 3 && card.at(0) == '[') {

                //adding [ and S8 from [S8 to vector as separate elements
                buildCards.push_back("[");
                //removing the "[" form the card
                card.erase(0,1);
                buildCards.push_back(card);

                //get new card
				ss >> card;

                //loop until the end of the single build of type [S8 .. S8]
                //end means data with value S8]
                while (card.length() != 3) {

                    //if not the end card S8]
                    //then the value must be individual card S8
                    buildCards.push_back(card);
                    //get new card to compare
                    ss >> card;
                    
                }

                //adding ] and S8 from S8] to vector as separate elements
                //removing the "]" form the card [S8
                card.erase(2,1);
                buildCards.push_back(card);
                buildCards.push_back("]");

            } else{

            	 //type [S8]

                //removing the "[" and "]" form the card and adding only S8 part 
                card.erase(0,1);
                card.erase(2,1);
                buildCards.push_back(card);
            }

            //get new single build or "]" as end of the multi build
            ss>>card;

        }

        //calculate the build value start and end index
        // Get index of element from iterator for start+1 and end
  		std::vector<std::string>::iterator it = std::find(buildCards.begin(), buildCards.end(), "[");
		start = std::distance(buildCards.begin(), it) + 1;
		it = std::find(buildCards.begin(), buildCards.end(), "]");
		end = std::distance(buildCards.begin(), it);
        
    } else if (card.length() == 3 && card.at(0) == '[') {

    	//single build
        buildCards.push_back("Single");

        //type [S8
        //erasing "[" and adding S8 from [S8 to vector
        card.erase(0,1);
        buildCards.push_back(card);

        //get new card
        ss>>card;

        //loop until the end of the single build of type [S8 .. S8]
        //end means data with value S8]
        while (card.length() != 3) {

            //if not the end card S8]
            //then the value must be individual card S8
            buildCards.push_back(card);
            //get new  card to compare as single card
            ss>>card;
        }

        //erasing "]" and adding S8 from S8] to vector and ending the single build
        card.erase(2,1);
        buildCards.push_back(card);
        
        //calculate the build value start and end index
        start = 1;
        end = buildCards.size();
    
    }else {
    	//error in the file
    	return false;
    }

    //calculating the build value
    //fails to calculate the buildValue for case
    // [0,Human, Multi, H6,C6, S6] since no [ and ]
    for(int i = start;i < end;i++){ buildValue += cardStringToValue(buildCards.at(i)); }

    //for case: [0,Human, Multi, H6,C6, S6]
    if(buildValue == 0) { buildValue = cardStringToValue(buildCards.at(1)); }


    //get new  card to compare as single card
	ss>>card;

    //temporary local string vector to store all info of a build
    std::vector<std::string> buildTotal;

    //value of the build after converting from int to string
    buildTotal.push_back(std::to_string(buildValue));
    
    //owner: human or computer
    buildTotal.push_back(card);

    //store the each card info in build into temp vector
    for (int i = 0; i < buildCards.size(); i++) { buildTotal.push_back(buildCards.at(i)); }

    //Store in the game table
    gameTable.storeTable(buildTotal);

	return true;
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
int GameEngine::cardStringToValue(std::string key) 
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

/* *********************************************************************
Function Name: saveGame
Purpose: saves game of given filename
Parameters: fileName, String name of the file to load
Return Value: boolean, check whether save was success
Local Variables: many..and not so important
Algorithm:
Assistance Received: none
********************************************************************* */
bool GameEngine::saveGame(std::string fileName)
{

	//add .txt and path to the user input file name
	std::string pathFileName = "/home/rshresth/00OPL/proj1Cpp/src/serializationTest/";
	pathFileName += fileName + ".txt";

	//open the file, create new 
	std::ofstream newFile(pathFileName);
	
	try {

		//temp local vector to store cards of different container
		std::vector<std::string> vectorCards;

        //Storing Round
        newFile << "Round: "<< round;

        //Storing Computer info
        newFile << "\n\nComputer: \n   Score: " << computer.getScore()<<"\n   Hand: ";
        //get cards from hand vector
        vectorCards = computer.getAllHandCards();
        for(int i = 0; i<vectorCards.size();i++){ newFile << vectorCards.at(i)<<" "; }
        //get cards from pile vector
        newFile <<"\n   Pile: ";
        vectorCards = computer.getAllPileCards();
        for(int i = 0; i<vectorCards.size();i++){ newFile << vectorCards.at(i)+" "; }
        
        //Storing Human info
       	newFile << "\n\nHuman: \n   Score: " << human.getScore()<<"\n   Hand: ";
        //get cards from hand vector
        vectorCards = human.getAllHandCards();
        for(int i = 0; i<vectorCards.size();i++){ newFile << vectorCards.at(i)<<" "; }
        //get cards from pile vector
        newFile <<"\n   Pile: ";
        vectorCards = human.getAllPileCards();
        for(int i = 0; i<vectorCards.size();i++){ newFile << vectorCards.at(i)+" "; }

      	//Store the buildOwner while accessing table
        newFile <<"\n\nTable: ";
        std::vector<std::vector<std::string>> tableCards = gameTable.getAllCards();
        //vector of strings to store all the builds
    	//required to input the information in "Build Owner" section of the file
    	vectorCards.clear();
        for(int i = 0; i < tableCards.size(); i++){
        	 //accessing elements of the table board

            //check the table element size for build
            //>2 means its a build
            if(tableCards.at(i).size() == 2){ newFile << tableCards.at(i).at(1)+" ";}
            else {
            	//build

            	//to store each build seperately
                std::string buildInfo;
 				
 				//check if multi or single build, stored as 3rd element of the table
                if(tableCards.at(i).at(2) == "Single") {
                    buildInfo +="[";

                    //Single build so simply store the cards
                    for(int j = 3; j < tableCards.at(i).size();j++){

                        //adding the cards without any space in between
                        buildInfo += tableCards.at(i).at(j);
                        
                        //Check if it's the last element of the build
                        //since no need to add " " to the last element
                        if(j != tableCards.at(i).size()-1) buildInfo += " ";

                    }
	            
	            } else{
                    //multi build
                    buildInfo +="[ ";

                    //loop to get the cards form the given element of the table
                    //card's info starts from index 3
                    for(int j = 3; j < tableCards.at(i).size();j++) {

                        //find if it's single or multi card
                        if(tableCards.at(i).at(j) == "[" ){
                           
                            //multi card build
                            //adding start
                            buildInfo += "[";

                            //increasing index to get new card
                            j++;

                            while (tableCards.at(i).at(j) != "]"){

                                buildInfo += tableCards.at(i).at(j);
                                j++;
                                if(tableCards.at(i).at(j) != "]") buildInfo += " ";
                            }

                            //adding end
                            buildInfo += "] ";
                        }
                        else {

                            //single card
                            //no bs just write the card inside [ ]
                            buildInfo += "["+tableCards.at(i).at(j)+"] ";
                        }
                    }
                }

	            //adding the build info in "Table: " section of  file
	            newFile <<buildInfo<<"] ";
                
	            //end of the build
	            buildInfo += "] "+tableCards.at(i).at(1);
                vectorCards.push_back(buildInfo);
            }

        }

        //adding the build owner info from vector to the content
        for(int i = 0; i<vectorCards.size();i++){ newFile <<"\n\nBuild Owner: "<<vectorCards.at(i); }

        //Store last capturer
        newFile <<"\n\nLast Capturer: "<<lastCapturer;
		
		//Store deck
        newFile <<"\n\nDeck: ";
        vectorCards = gameDeck.getDeck();
        for(int i = 0; i<vectorCards.size();i++){ newFile << vectorCards.at(i)<<" ";  }

    	//adding the next Player
        newFile <<"\n\nNext Player: ";
        if(humanTurn){ newFile << "Human"; }
        else{ newFile <<"Computer"; }

        //closing the file
        newFile.close();


    }
    catch (int e) {

		//Unable to open file
		//Error reading file
		//Displaying the turn of the player
		return false;

	}

	return true;
}

/* *********************************************************************
Function Name: checkGameChanges
Purpose: check for changes for dealing cards, new round and end of the tournament
Parameters:
Return Value: bool --> true for the end of the game
Local Variables
Algorithm:
Assistance Received: none
********************************************************************* */
bool GameEngine::checkGameChanges()
{
	
	bool dealTable = false;
		
	//check if player's hand is empty
	if(human.getHandSize() ==0 && computer.getHandSize() == 0) {
		
		if(gameDeck.getDeckSize() == 0){
			std::cout << "\n*********** Round "<<round<<" Complete ***********\n";

			//clear the table and store the cards to the pile of last capturer
			std::vector<std::vector<std::string>> tableCards = gameTable.getAllCards();
			for(int i=0;i<tableCards.size();i++){
				std::vector<std::string> card = tableCards.at(i);
				if(lastCapturer == "Human"){human.storePile(card.at(1));}
				else{computer.storePile(card.at(1)); }
			}

			//create new table
			gameTable = Table();
			
			printGameBoard();

			//calculate and display score
			std::vector<int> humanScoreInfo = calculatePlayerScore(human);
			std::vector<int> computerScoreInfo = calculatePlayerScore(computer);
			
			int humanScore = humanScoreInfo.at(0); 
			int computerScore = computerScoreInfo.at(0);

			//check for player with higher num of cards
			if(humanScoreInfo.at(1) > computerScoreInfo.at(1)){ humanScore += 3; }
			else if(humanScoreInfo.at(1) < computerScoreInfo.at(1)){ computerScore += 3; }


			//check for player with higher num of spade cards
			if(humanScoreInfo.at(2) > computerScoreInfo.at(2)){ humanScore ++; }
			else if(humanScoreInfo.at(2) < computerScoreInfo.at(2)){ computerScore ++; }


			std::cout<<"\nTotal Num of Cards: Human: "<<humanScoreInfo.at(1)<<" Computer: "<<computerScoreInfo.at(1)<<"\n";
			std::cout<<"Num of Spades: Human: "<<humanScoreInfo.at(2)<<" Computer: "<<computerScoreInfo.at(2)<<"\n";
			std::cout<<"Score for this round: Human: "<<humanScore<<" Computer: "<<computerScore<<"\n";

			//update score and round
			human.setScore(human.getScore() + humanScore);
			computer.setScore(computer.getScore() + computerScore);
			
			//get the final score to compare
			int finalScoreHuman = human.getScore();
			int finalScoreComputer = computer.getScore();
			
			std::cout<<"Tournament score: Human: "<<finalScoreHuman<<" Computer: "<<finalScoreComputer<<"\n";
			
			//check if the score of any player is above 21
			if(finalScoreHuman > 21 || finalScoreComputer > 21) { 
				//find the winner
				std::cout << "\n****** Game Over ******\n";
				std::cout<<"\tWinner of the game: ";
				if(finalScoreHuman > finalScoreComputer){ std::cout<<"Human";}
				else if(finalScoreHuman < finalScoreComputer){ std::cout<<"Computer";}
				else { std::cout<<"Draw";}
				//exit
				std::cout << "\n**********************\n\n";
				//sleep
				std::this_thread::sleep_for(std::chrono::seconds(3));
				return true;
			}

			//increase the round num
			round++;
			//create new deck
			gameDeck = Deck(true);
			//clear pile for both player
			human.clearPile();
			computer.clearPile();

			//deal table
			dealTable = true;

		}

		
		//set the turn for next round based on last Capturer
		if(lastCapturer == "Human"){ humanTurn = true;}
		else{ humanTurn = false;}
		
		//sleep
		std::this_thread::sleep_for(std::chrono::seconds(2));

		std::cout<<"\n Dealing New Cards\n "<<std::endl;
		newDealCards(dealTable);
	}

	return false;
}

/* *********************************************************************
Function Name: calculatePileScore
Purpose: calculates and returns the score for given player pile, also finds the num of total cards and num of spade cards
Parameters: Player, player to calculate the score
Return Value: vector<int>, vector holding the score, total num of cards and total num of spades
Local Variables
Algorithm:
Assistance Received: none
********************************************************************* */
std::vector<int> GameEngine::calculatePlayerScore(Player player)
{
	//varibles to store the info
	std::vector<int> scoreData;
	int score = 0;
	int countSpade = 0;

	//get the list of pile cards from the player
	std::vector<std::string> pile = player.getAllPileCards();
	
	for(int i =0; i<pile.size();i++){
		std::string card = pile.at(i);
		if(card.at(0) == 'S') countSpade++;
		score += GameMoveCheck::scoreCalculator(card);
	}

	//store the info in the vector
	scoreData.push_back(score);
	scoreData.push_back(pile.size());
	scoreData.push_back(countSpade);

	return scoreData;
}