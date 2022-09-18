#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <Windows.h>
#include <fstream>
#include <conio.h>

using namespace std;

/*
SNAKE AND LADDER GAME - GROUP MEMBERS:
1. DAWOOD AHMAD - 20F-0111
2. ARBAZ AMEER KHAN - 20F-0270

INSTRUCTION FOR CODE READER:

1. Because array indexes in c++ start from 0, we have declared some arrays of size 5
but only used the last 4 elements and ignored the first 1 for simplicity.

2. Colors:
i) p1 - 47, green
ii) p2 - 79, red
iii) p3 - 111, yellow
iv) p4 - 31, blue
*/

//GLOBAL VARIABLES:
array<array<string, 3>, 100> gameBoard{}; //2D array for saving the entries 1- 100

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //Object for color manipulation


int die; //the result of a die
int choice; //to read player's choice from menu
int firstPlayer; //the number of player who makes the first move
int firstWinner = 0; //the number of the player who wins first
int winnerIndex; //to keep trach of winner's number while giving six points to another player

int dices[5]{}; //the results of dices
int players[5] = { 0, 0, 0, 0, 0 }; //the scores of the players

//2D arrays to save snakes and ladder indexes
array<array<int, 2>, 5> ladders{};
array<array<int, 2>, 5> snakes{};

//array for saving players who have won
array<int, 4> playersWhoHaveWon{};

//array for saving player names
array<string, 5> playerNames{ "", "", "", "", "" };

//array for counting the number of turns a player makes
array<int, 5> playerTurns{};



//Prototypes of all functions used
void printGameBoard();
bool isSnakeIndex(int index);
bool isSnakeHead(int index);
bool isLadderIndex(int index);
bool isLadderHead(int index);
void initializeGameBoard();
void initializeLadders();
bool ladderMatch(int index);
int newPositionAfterLadder(int index);
void initializeSnakes();
bool snakeMatch(int index);
int newPositionAfterSnake(int index);
void printMenu();
void startGame();
void printGameInfo();
int maxDice(int dices[]);
bool multiplicity(int dices[]);
void decideFirstPlayer();
void simulateGame();
int newPositionAfterLadder(int index);
void addDie(int& player, int die);
bool hasWon(int player);
void readPlayerNames();
void giveSixPoints();
bool isGameOver();
void saveWinner();
void printGameResults();
void printRecord();
void showCredits();
void showRulesAndInstructions();

//Main function
int main()
{
	bool run = true;

	while (run)
	{
		printMenu();
		cin >> choice;

		switch (choice)
		{
		case 1:
			startGame();
			break;
		case 2:
			showCredits();
			break;
		case 3:
			showRulesAndInstructions();
			break;
		case 4:
			printRecord();
			_getch();
			break;
		case 5:
			run = false;
			break;
		default:
			cout << "Please enter a valid choice." << endl;
			break;
		}
	}
	/*
	initializeArray();
	printMenu();

	cin >> choice;

	switch (choice)
	{
		default:
			break;
	}*/
	return 0;
}

//function to print the bame board
void printGameBoard()
{
	cout << endl;
	int j = 0; //for manipulation

	//print upper boundary
	for (int i = 0; i < 76; i++)
	{
		cout << "-";
	}
	cout << endl;

	//For loop for going through 1 to 100
	for (int i = gameBoard.size() - 1; i >= 0; i--)
	{
		//reset the color
		SetConsoleTextAttribute(hConsole, 15);

		//the boundary at left
		if ((i + 1) % 10 == 0)
		{
			cout << "|    ";
		}

		//reverse the rows by using j
		if ((i / 10) % 2 == 0)
		{
			if ((i + 1) % 10 == 0)
			{
				j = i - 10;
			}
			j++;

			//setting color according to the players
			if (j > 0)
			{
				if (j == players[1])
				{
					SetConsoleTextAttribute(hConsole, 47);
				}
				else if (j == players[2])
				{
					SetConsoleTextAttribute(hConsole, 79);
				}
				else if (j == players[3])
				{
					SetConsoleTextAttribute(hConsole, 111);
				}
				else if (j == players[4])
				{
					SetConsoleTextAttribute(hConsole, 31);
				}
			}

			//printing the number
			cout << right << setw(3) << gameBoard[j][0];

			//reset the color
			SetConsoleTextAttribute(hConsole, 15);

			//Red color for snake
			if (isSnakeIndex(j))
			{
				SetConsoleTextAttribute(hConsole, 12);
			}

			//snake entry
			cout << gameBoard[j][1];

			//reset color
			SetConsoleTextAttribute(hConsole, 15);

			//Green color for ladder
			if (isLadderIndex(j))
			{
				SetConsoleTextAttribute(hConsole, 10);
			}

			//entry 3
			cout << gameBoard[j][2];

			//reset the color
			SetConsoleTextAttribute(hConsole, 15);
		}
		//use i for other rows
		else
		{
			//setting color according to the positions of players
			if (i > 0)
			{
				if (i == players[1])
				{
					SetConsoleTextAttribute(hConsole, 47);
				}
				else if (i == players[2])
				{
					SetConsoleTextAttribute(hConsole, 79);
				}
				else if (i == players[3])
				{
					SetConsoleTextAttribute(hConsole, 111);
				}
				else if (i == players[4])
				{
					SetConsoleTextAttribute(hConsole, 31);
				}
			}
			//printing the number
			cout << right << setw(3) << gameBoard[i][0];

			//reset the color
			SetConsoleTextAttribute(hConsole, 15);

			//red color for snake
			if (isSnakeIndex(i))
			{
				SetConsoleTextAttribute(hConsole, 12);
			}

			//printing the snake entry
			cout << gameBoard[i][1];


			//Green color for ladder
			if (isLadderIndex(i))
			{
				SetConsoleTextAttribute(hConsole, 10);
			}

			cout << gameBoard[i][2];
			//Reset the color
			SetConsoleTextAttribute(hConsole, 15);
		}

		//printing line break after every 10 entries
		if (i % 10 == 0)
		{
			cout << "|";
			cout << endl;
			cout << endl;
		}
	}

	//print lower boundary
	for (int i = 0; i < 76; i++)
	{
		cout << "-";
	}
	cout << endl;
	cout << endl;
}

//boolean function to check if an entry has a snake
bool isSnakeIndex(int index)
{
	array<int, 24> snakeIndexes{ 5,6,7,14,15, 32,33,34,35,36, 20,21,22,39, 62,
	63,64,65,73,74, 97, 96, 95, 94 };

	for (int i = 0; i < snakeIndexes.size(); i++)
	{
		if (index == snakeIndexes[i])
		{
			return true;
		}
	}
	return false;
}

//boolean function to check if an entry has the head of a snake
bool isSnakeHead(int index)
{
	array<int, 5> snakes{ 15, 36, 39, 73, 97 };
	for (int i = 0; i < snakes.size(); i++)
	{
		if (index == snakes[i])
		{
			return true;
		}
	}
	return false;
}

//boolean function to check if an entry has a ladder
bool isLadderIndex(int index)
{
	array<int, 30> ladders{ 1, 2, 3, 15, 16, 14, 22, 36, 37, 35, 34, 33, 59, 58, 57, 62, 63, 75,76,
	50, 69, 68, 67, 66, 65, 89, 90, 91, 92, 93 };
	for (int i = 0; i < ladders.size(); i++)
	{
		if (index == ladders[i])
		{
			return true;
		}
	}
	return false;
}

//boolean function to check if an entry has the head of a ladder
bool isLadderHead(int index)
{
	array<int, 5> ladderHeads{ 14, 33, 75, 65, 93 };

	for (int i = 0; i < ladderHeads.size(); i++)
	{
		if (index == ladderHeads[i])
		{
			return true;
		}
	}
	return false;
}

void initializeGameBoard()
{
	//for loop to initalize the array
	for (int i = 0; i < gameBoard.size(); i++)
	{
		//base case
		gameBoard[i][0] = ((i < 9) ? "0" : "") + to_string(i + 1);
		gameBoard[i][1] = "";
		gameBoard[i][2] = "    ";

		//if there is a snake at ith element
		if (isSnakeIndex(i))
		{
			gameBoard[i][1] = "*";
			gameBoard[i][2] = "   ";
			if (isSnakeHead(i))
			{
				gameBoard[i][1] = "^^";
				gameBoard[i][2] = "  ";
			}
		}

		//if there is a ladder at ith element
		if (isLadderIndex(i) && !isSnakeIndex(i)) //if there is no snake
		{
			if (!isLadderHead(i))
			{
				gameBoard[i][2] = "*   ";
			}
			else
			{
				gameBoard[i][2] = "^^  ";
			}
		}
		else if (isLadderIndex(i) && isSnakeIndex(i)) //if there is a snake
		{
			//if there is head of a snake
			if (isSnakeHead(i))
			{
				if (isLadderHead(i))
				{
					gameBoard[i][2] = "^^";
				}
				else
				{
					gameBoard[i][2] = "* ";
				}
			}
			//if there is other entry of snake (not head)
			else
			{
				if (isLadderHead(i)) //if it is the top of ladder
				{
					gameBoard[i][2] = "^^ ";
				}
				else
				{
					gameBoard[i][2] = "*  ";
				}
			}
		}
	}
}

//save ladder entries - start and end of each ladder
void initializeLadders()
{
	ladders[0][0] = 1;
	ladders[0][1] = 15;

	ladders[1][0] = 22;
	ladders[1][1] = 34;

	ladders[2][0] = 59;
	ladders[2][1] = 76;

	ladders[3][0] = 50;
	ladders[3][0] = 65;

	ladders[4][0] = 89;
	ladders[4][1] = 93;
}

//boolean function to tell if after rolling die the player gets a ladder
bool ladderMatch(int index)
{
	for (int i = 0; i < 5; i++)
	{
		if (ladders[i][0] == index)
		{
			return true;
		}
	}
	return false;
}

//function to return new position after ladder
int newPositionAfterLadder(int index)
{
	for (int i = 0; i < 5; i++)
	{
		if (ladders[i][0] == index)
		{
			return ladders[i][1];
		}
	}
}

//save snake entries - start and end of each snake
void initializeSnakes()
{
	snakes[0][0] = 14;
	snakes[0][1] = 7;

	snakes[1][0] = 39;
	snakes[1][1] = 22;

	snakes[2][0] = 36;
	snakes[2][1] = 32;

	snakes[3][0] = 73;
	snakes[3][1] = 62;

	snakes[4][0] = 97;
	snakes[4][1] = 94;
}

//boolean function to tell if after rolling die the player gets bitten by a snake
bool snakeMatch(int index)
{
	for (int i = 0; i < 5; i++)
	{
		if (snakes[i][0] == index)
		{
			return true;
		}
	}
	return false;
}

//function that returns new position after snake bite
int newPositionAfterSnake(int index)
{
	for (int i = 0; i < 5; i++)
	{
		if (snakes[i][0] == index)
		{
			return snakes[i][1];
		}
	}
}

//function to print game menu
void printMenu()
{
	system("cls");

	cout << "WELCOME TO SNAKE AND LADDER...\n\n"
		<< "YOU HAVE THE FOLLWING OPTIONS: " << endl;

	cout << "1. Play Game!" << endl;;
	cout << "2. Show Credits" << endl;;
	cout << "3. Rules and Instructions" << endl;;
	cout << "4. Records" << endl;;
	cout << "5. Exit" << endl;
}

//function to read names of participants
void readPlayerNames()
{

	cin.ignore();
	for (int i = 1; i < playerNames.size(); i++)
	{
		cout << "Player " << i << " Name: " << endl;
		getline(cin, playerNames[i]);
	}
}

//function to begin the game and call other functions
void startGame()
{
	system("cls"); //clear the screen

	//read names of players
	readPlayerNames();

	//intialize the entries of the game board
	initializeGameBoard();

	//set the ladder entries
	initializeLadders();

	//set the snake entries
	initializeSnakes();

	system("cls");
	cout << "SNAKE AND LADDER GAME\n";

	//decide which player has first turn
	decideFirstPlayer();

	//play the game
	simulateGame();
}

//the function to roll die and decide who makes first turn
void decideFirstPlayer()
{
	srand(time(0)); //seed rand() for new random values

	//tell players to roll die
	cout << "Each player will roll the dice to decide who makes first move." << endl << endl;


	for (int i = 1; i < 5; i++)
	{
		//prompt the player to roll die
		cout << playerNames[i] << "'s turn. " << endl << endl
			<< "Press enter to roll the dice: " << endl;
		cin.ignore();
		cout << endl;

		dices[i] = rand() % 6 + 1;

		//print die of each player
		cout << playerNames[i] << " rolled: " << dices[i] << endl << endl;

		//if more than 1 player roll max die
		if (i == 4 && multiplicity(dices))
		{
			system("cls");

			//tell players to roll die again
			cout << "More than one players rolled " << dices[maxDice(dices)] << "\n"
				<< "Roll the dice again:" << endl;
			i == 0;
		}
	}

	//first player is one who rolls max dice
	firstPlayer = maxDice(dices);
}

//the function to print the score and color of each player
void printGameInfo()
{
	//player 1 color:
	SetConsoleTextAttribute(hConsole, 47);
	cout << playerNames[1] << ": ";
	cout << players[1] + 1;

	//player 2 color:
	SetConsoleTextAttribute(hConsole, 79);
	cout << "\t" << playerNames[2] << ": ";
	cout << players[2] + 1;


	//player 3 color:
	SetConsoleTextAttribute(hConsole, 111);
	cout << "\t" << playerNames[3] << ": ";
	cout << players[3] + 1;

	//player 4 color:
	SetConsoleTextAttribute(hConsole, 31);
	cout << "\t" << playerNames[4] << ": ";
	cout << players[4] + 1;

	//reset color
	SetConsoleTextAttribute(hConsole, 15);
}

//Colors:
/* Colors:
Colors:
p1 - 47, green
p2 - 79, red
p3 - 111, yellow
p4 - 31, blue
*/

//boolean function to check if more than one players have rolled max dice
bool multiplicity(int dices[])
{
	int index = maxDice(dices);
	for (int i = index + 1; i < 5; i++)
	{
		if (dices[i] == dices[index])
		{
			return true;
		}
	}
	return false;
}

//function that returns maximum value from dices rolled
int maxDice(int dices[])
{
	int maxDice = 1;
	for (int i = 1; i < 5; i++)
	{
		if (dices[i] > dices[maxDice])
		{
			maxDice = i;
		}
	}
	return maxDice;
}

//function to add the die to the score of a player
void addDie(int& player, int die)
{
	//check if die excedes position 100
	if (player + die <= 99)
	{
		player += die;
	}

	//if player gets a ladder
	if (ladderMatch(player))
	{
		//turn the screen red for 1.5 seconds
		system("color 27");
		Sleep(1500);
		system("color 07");

		player = newPositionAfterLadder(player);
	}

	//if player gets a snake
	if (snakeMatch(player))
	{
		//turn the screen red for 1.5 seconds
		system("color 47");
		Sleep(1500);
		system("color 07");
		player = newPositionAfterSnake(player);
	}


}

//boolean function to tell if a player has won or not
bool hasWon(int player)
{
	for (int i = 0; i < playersWhoHaveWon.size(); i++)
	{
		if (player == playersWhoHaveWon[i])
		{
			return true;
		}
	}
	return false;
}

//boolean function to tell if all players have won or a player is remaining
bool isGameOver() {
	int winnersCount{ 0 };
	for (int i = 0; i < 4; i++)
	{
		if (playersWhoHaveWon[i] != 0)
		{
			winnersCount++;
		}
	}
	if (winnersCount == 4)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//function that enables first winner to give 6 points to another player
void giveSixPoints()
{
	for (int i = 1, j = 1; i < 5; i++)
	{
		if (!hasWon(i))
		{
			cout << j << ". " << playerNames[i] << endl;
			j++;
		}
		else
		{
			winnerIndex = i;
		}
	}
	cout << endl;

	cin >> choice;

	//validating the choice of the player
	while (choice < 1 || choice > 3)
	{
		cout << "Please enter a valid choice: " << endl;
		cin >> choice;
	}

	//adjusting the choice value
	if (choice >= winnerIndex)
	{
		choice++;
	}

	int initialPosition = players[choice];
	//give six points:
	addDie(players[choice], 6);

	system("cls");

	//print who got 6 bonus points from winner player
	cout << playerNames[winnerIndex] << " gave 6 points to " << playerNames[choice] << endl;

	//print the movement of the player after getting 6 points
	cout << playerNames[choice] << " Moved from position " << initialPosition + 1 << " to position "
		<< players[choice] + 1 << endl;
}

//function to print game results after game is over
void printGameResults()
{
	system("cls");
	cout << "GAME RESULTS: " << endl << endl << endl;
	cout << setw(20) << left << "Position"
		<< setw(24) << "Player Name"
		<< "Numer of Turns" << endl;

	cout << setw(20) << "Winner: "
		<< setw(24) << playerNames[playersWhoHaveWon[1]]
		<< playerTurns[playersWhoHaveWon[1]] << endl;

	cout << setw(20) << "Second: "
		<< setw(24) << playerNames[playersWhoHaveWon[2]]
		<< playerTurns[playersWhoHaveWon[2]] << endl;

	cout << setw(20) << "Third Position: "
		<< setw(24) << playerNames[playersWhoHaveWon[3]]
		<< playerTurns[playersWhoHaveWon[3]] << endl;
}

//function to play the game
void simulateGame()
{
	system("cls");

	//print which player makes the first move
	cout << playerNames[firstPlayer] << " rolled the highest. So "
		<< playerNames[firstPlayer] << " makes the first move. " << endl;

	bool continueGame = true;
	int currentPlayer = firstPlayer;
	int initialPosition = 0;
	bool skip = false; //to manipulate the printing of game info and game board

	//array to store which players can move
	array<bool, 5> playerAllowedToMove{ 0, false, false, false, false };


	while (continueGame)
	{
		if (!skip)
		{
			printGameInfo();
			printGameBoard();
		}

		srand(time(0)); //seed rand() for new random values

		skip = false;
		if (!hasWon(currentPlayer))
		{
			//player rolls the die
			cout << playerNames[currentPlayer] << "'s turn: " << endl;
			cout << "Press enter to roll the die: " << endl;

			//increment player turns
			playerTurns[currentPlayer]++;

			cin.ignore();
			die = rand() % 6 + 1;

			//clear the screen
			system("cls");

			//print player's score
			cout << playerNames[currentPlayer] << " rolled " << die << endl;

			//save initial position
			initialPosition = players[currentPlayer];

			//if the player is allowed to move, call the addDie() function
			if (playerAllowedToMove[currentPlayer])
			{
				addDie(players[currentPlayer], die);
			}

			//print the movement of the player
			if (initialPosition != 0)
			{
				cout << playerNames[currentPlayer] << " moved from position " << initialPosition + 1
					<< " to position " << players[currentPlayer] + 1 << endl;

			}
			bool firstPlayerToWin = false; // a temporary variable
			//if the player completes 100, the player wins

			//check if a player has won
			if (players[currentPlayer] == 99)
			{
				//check if he is the first player to win
				for (int i = 0; i < 4; i++)
				{
					if (playersWhoHaveWon[i] == 0)
					{
						playersWhoHaveWon[i] = currentPlayer;
						if (i == 0)
						{
							firstPlayerToWin = true;
						}
						i = 4;
					}
				}
				//show a win message
				cout << "Congratulations! " << playerNames[currentPlayer] << " Won!";

				//if the winner is first player to win
				if (firstPlayerToWin)
				{
					printGameBoard(); //print board again

					//prompt the first winner t give 6 points
					cout << playerNames[currentPlayer] << " is the first player to win. "
						<< "Chose a player to give him 6 points: " << endl;

					//give six points to another player
					giveSixPoints();
				}
			}

			//if the current player rolls 6
			if (die == 6)
			{
				//allow him to move if he is at position 1 and give him another turn
				if (!playerAllowedToMove[currentPlayer])
				{
					playerAllowedToMove[currentPlayer] = true;
					cout << playerNames[currentPlayer]
						<< " can start playing. " << endl
						<< "Now " << playerNames[currentPlayer] << " will roll again: " << endl;
				}
				//give him another turn if he is already allowed to move
				else
				{
					cout << playerNames[currentPlayer]
						<< " rolled 6. So He will roll again"
						<< endl;
				}
			}
			else if (currentPlayer == 4)
			{
				cout << endl;
				currentPlayer = 1;
			}
			else
			{
				currentPlayer++;
			}

		}
		else
		{
			skip = true;
			if (currentPlayer == 4)
			{
				currentPlayer = 1;
			}
			else
			{
				currentPlayer++;
			}
		}

		//if all players have completed 100
		if (isGameOver())
		{
			continueGame = false;
		}
	}

	//Program comes here once game is finished

	//Print game results
	printGameResults();

	//save the winner with his number of turns
	saveWinner();
	cout << "\n\nPress Enter to get back to menu. " << endl;
	cin.ignore();
}

//function to save the winner in a file use fstream
void saveWinner()
{
	ofstream fileWriter{ "Records.txt", ios::app };
	fileWriter << left << endl
		<< setw(25) << playerNames[playersWhoHaveWon[1]]
		<< playerTurns[playersWhoHaveWon[1]] << endl;
}

//function to print the records of winners
void printRecord()
{
	system("cls");
	cout << "WINNER RECORDS " << endl;
	cout << left << setw(10) << "MATCH"
		<< setw(25) << "WINNER" << "NUMBER OF TURNS" << endl;

	ifstream readFile{ "Records.txt" };
	string line = "";
	int matchNumber = 1;
	while (getline(readFile, line))
	{
		cout << setw(10) << matchNumber << line << endl;
		matchNumber++;
	}

	if (matchNumber == 1)
	{
		cout << endl << endl;
		cout << "No records found."
			<< endl;
	}

	cout << endl << "Press enter to get back " << endl;
	_getch();
}

//function to show credits - who made this game
void showCredits()
{
	system("cls");
	cout << "PROGRAMMING FUNDAMENTALS LAB - FINAL PROJECT: " << endl << endl;;
	cout << "-> A Snake and Ladder game: " << endl << endl;
	cout << "Made By D Section PF Group: " << endl;
	cout << "Group Members: " << endl;
	cout << "1. Dawood Ahmad - 20F-0111" << endl
		<< "2. Arbaz Ameer Khan - 20F-0270" << endl << endl;

	cout << "Press enter to go back.." << endl;
	_getch();
}

//function to show rules and instructions
void showRulesAndInstructions()
{
	system("cls");
	cout << "GAME INSTRUCTIONS: " << endl << endl;
	cout << "1. This game is for for players." << endl;
	cout << "2. Press 1 in menu to start the game." << endl;
	cout << "3. Enter the names of the players" << endl;
	cout << "4. Each player will get his turn to roll the dice. "
		<< "The player who rolls highest number will make the first turn." << endl;
	cout << "5. Each player rolls the die on his turn." << endl;
	cout << "6. The green * signs show the ladders. the green ^^ sign shows the top of a ladder." << endl;
	cout << "7. The red * sign show the snakes. The red ^^ sign shows the head of a snake" << endl;
	cout << "8. When a player climbs a ladder, screen turns green for 2 seconds." << endl;
	cout << "9. When a player is bitten by a snake, screen turns red for 2 seconds." << endl;

	cout << endl << endl << endl;

	cout << "GAME RULES: " << endl << endl;
	cout << "1. The goal of the game is to be the first person to "
		<< "get on the last square of the game." << endl;

	cout << "2. The players go onwards clockwise according to the number rolled by die." << endl;
	cout << "3. If you roll a 6, you get a second turn." << endl;
	cout << "4. You can obly move onto the board if you get 6." << endl;
	cout << "5. If you get to the bottom of the ladder, you automatically "
		<< "move up to the top of the ladder." << endl;
	cout << "6. If you get on the top of a snake, you automatically move "
		<< "downwards to the bottom of the snake." << endl;
	cout << "7. The player who finishes first gives 6 points "
		<< "to one player of his choice." << endl;

	cout << "8. When you reach near the 100, you can only win "
		<< "if you get an exact die for 100. You can't move to 101 or 102." << endl << endl << endl;

	cout << "Press enter to go back." << endl;
	_getch();

}