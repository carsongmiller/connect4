//connect4
//Carson Miller
//10-7-2015


/*

Things to modify:of
-fix winDetect() (I think it's working now)
-shallower recursion beginning, allow deeper as game goes on
-weigh certain situations at certain situations in the game more heavily
-when the board is symmetrical, the scores for each column should be symmetrical
-consider adding a small bit of randomness into minimax
	-like if two options are both very good and within %20 of eachother or something
	-add a chance that the "worse" one is chosen
-weigh scores better
-consider: run through minimax until the first instance of a win is found, then go some certain number of iterations deeper
	-this would be overwritten by logic that makes minimax go shallower at the beginning

*/



#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <string>

using namespace std;


//pDisc =  disc; cDisc = computer disc; nDisc = no disc
const int pDisc = 1, cDisc = 2, nDisc = 0;
//width and height variables
const int w_ = 7, h_ = 6;
//Base number of maximum iterations (depending on the stage in the game, recursion may go more or less deep)
const int MAX_ITER = 7;


//Values for SetConsoleTextAttribute()
HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);

const int BLACK = 0;
const int BLUE = 1;
const int GREEN = 2;
const int CYAN = 3;
const int RED = 4;
const int MAGENTA = 5;
const int BROWN = 6;
const int LIGHTGRAY = 7;
const int DARKGRAY = 8;
const int LIGHTBLUE = 9;
const int LIGHTGREEN = 10;
const int LIGHTCYAN = 11;
const int LIGHTRED = 12;
const int LIGHTMAGENTA = 13;
const int YELLOW = 14;
const int WHITE = 15;


//returns true if move is valid, returns false if not
bool playMove(int board[][w_], int col, int who);

//detects if "who" has won
bool winDetect(int board[][w_], int who);

//determines the best move for "who"
void minimax(int board[][w_], int score[], int who, int currentCheck, int iter);

//prints the board
void printBoard(int board[][w_]);

//copies the board
void copyBoard(int board[][w_], int newBoard[][w_]);

//prints a given string of text in a different color then changes the text color back to white (overloaded for string, int, and char arguments)
void printColor(string str, int color);
void printColor(int i, int color);
void printColor(char c, int color);

//returns an appropriate symbol for the given index when given the board
char getChar(int board[][w_], int r, int c);



int main()
{
	int board[h_][w_]; //the main board
	bool cont = true, validMove, newGame = true; //determines whether the game should continue
	char whoFirst; //who will go first
	int moveChoice; //human player's move choice
	int score[w_]; //stores the scores of each column (handed to minimax())
	int bestScore; //stores the number of the column with the best score
	char yn; //takes input from player as to whether they want to play again

	while (newGame)
	{
		//initializing the board to all blank spaces
		for (int r = 0; r < h_; r++)
		{
			for (int c = 0; c < w_; c++)
			{
				board[r][c] = nDisc;
			}
		}


		//First output of the program:
		system("cls");

		cout << "CONNECT 4!\n\nWho should go first? (m for me, c for computer): ";
		cin >> whoFirst;

		if (whoFirst == 'm' || whoFirst == 'M')
		{
			printBoard(board);

			validMove = false;
			while (!validMove)
			{
				cout << "Where would you like to go? (enter column number): ";
				cin >> moveChoice;

				if (!playMove(board, moveChoice - 1, pDisc))
					cout << "There is no space in that column, choose a different one\n\n";

				if (moveChoice > w_)
					cout << "There is no column " << moveChoice << ". Please choose a column between 1 and " << w_ << "\n\n";

				else
					validMove = true;
			}
		}

		system("cls");
		cout << "CONNECT 4!\n\n\n";
		printBoard(board);

		//MAIN GAME LOOP

		while (cont)
		{
			//reset the score of each column to 0 at the beginning of each turn
			for (int i = 0; i < w_; i++)
				score[i] = 0;

			cout << "Now its' the computer's turn\n";
			cout << "\nthinking ...\n\n";

			minimax(board, score, cDisc, cDisc, 0);


			cout << endl << endl;

			bestScore = 0;

			for (int i = 1; i < w_; i++)
			{
				if (score[i] > score[bestScore])
					bestScore = i;
			}

			//now the computer will make its move

			playMove(board, bestScore, cDisc);

			system("cls");
			cout << "CONNECT 4!\n\n\n";

			//printing the score array for debugging
				for (int i = 0; i < w_; i++)
					cout << score[i] << "\t";

			cout << "\n";

			printBoard(board);

			if (winDetect(board, cDisc))
			{
				cout << "\nThe computer wins!\n";
				break;
			}

			//now the player's turn
			//Player chosing his move
			validMove = false;
			while (!validMove)
			{
				cout << "Where would you like to go? (enter column number): ";
				cin >> moveChoice;

				if (!playMove(board, moveChoice - 1, pDisc))
					cout << "There is no space in that column, choose a different one\n";
				else
					validMove = true;
			}

				system("cls");
				cout << "CONNECT 4!\n\n\n";
				printBoard(board);

			//detecting a player win	
			if (winDetect(board, pDisc))
			{
				cout << "\nYou win!\n";
				break;
			}

		}

		cout << "Would you like to play again? (y/n): ";
		cin >> yn;
		if (yn != 'y' && yn != 'Y')
			newGame = false;
	}

	return 0;
}



bool playMove(int board[][w_], int col, int who)
{
	bool open = false;
	for (int i = h_ - 1; i >= 0; i--)
	{
		if (board[i][col] == 0)
		{
			open = true;
			board[i][col] = who;
			break;
		}
	}

	return open;
}



void minimax(int board[][w_], int score[], int who, int currentCheck, int iter)
{
	int newBoard[h_][w_];
	copyBoard(board, newBoard);
	if (iter <= MAX_ITER)
	{
		for (int i = 0; i < w_; i++)
		{
			if (playMove(newBoard, i, currentCheck))
			{
				if (winDetect(newBoard, cDisc))
				{
					if (iter == 0)
					{
						score[i] += 1000;
					}
					else
						score[i] += (MAX_ITER - iter);
				}


				else if (winDetect(newBoard, pDisc))
				{
					if (iter == 1)
					{
						score[i] += 500;
					}
					else
						score[i] -= (MAX_ITER - iter);
				}


				else if (!winDetect(newBoard, cDisc) && !winDetect(newBoard, pDisc))
				{
					if (currentCheck == cDisc)
						currentCheck = pDisc;
					else if (currentCheck == pDisc)
						currentCheck = cDisc;

					minimax(newBoard, score, who, currentCheck, iter + 1);
				}
			}
		}
	}
}



bool winDetect(int board[][w_], int who)
{
	bool win = false;

	//detecting horizontal wins
	for (int r = h_ - 1; r >= 0; r--)
	{
		for (int c = 0; c < w_ - 3; c++)
		{
			if (board[r][c] == who)
			{
				if (board[r][c + 1] == who)
				{
					if (board[r][c + 2] == who)
					{
						if (board[r][c + 3] == who)
							win = true;
					}
				}
			}
		}
	}


	//detecting vertical wins
	for (int c = 0; c < w_; c++) //cycling through all columns
	{
		for (int r = h_ - 1; r > 2; r--) //cycling through sets of 4 cells in the same column
		{
			if (board[r][c] == who)
			{
				if (board[r - 1][c] == who)
				{
					if (board[r - 2][c] == who)
					{
						if (board[r - 3][c] == who)
							win = true;
					}
				}
			}
		}
	}


	//detcting diagonal-up wins
	for (int r = h_ - 1; r > 2; r--)
	{
		for (int c = 0; c < w_ - 3; c++)
		{
			if (board[r][c] == who)
			{
				if (board[r - 1][c + 1] == who)
				{
					if (board[r - 2][c + 2] == who)
					{
						if (board[r - 3][c + 3] == who)
							win = true;
					}
				}
			}
		}
	}

	//detecting diagonal-down wins
	for (int r = 0; r < h_ - 3; r++)
	{
		for (int c = 0; c < w_ - 3; c++)
		{
			if (board[r][c] == who)
			{
				if (board[r + 1][c + 1] == who)
				{
					if (board[r + 2][c + 2] == who)
					{
						if (board[r + 3][c + 3] == who)
							win = true;
					}
				}
			}
		}
	}

	return win;
}



//Following code is pretty messy, but it works and makes a nice print out

void printBoard(int board[][w_])
{
	for (int i = 1; i <= w_; i++)
	{
		cout << "    ";
		printColor(i, GREEN);
		cout << "\t";
	}

	cout << "\n+";

	//First row of ----+----
	for (int i = 0; i < w_; ++i)
		cout << "-------+";

	cout << "\n";


	//rows of cells with discs in them	
	for (int r = 0; r < h_; r++)
	{
		//First Row of | with no discs
			cout << "|   ";

			for (int i = 0; i < w_; i++)
				cout << "    |   ";

		//Row with discs

			cout << "\n|   ";

			for (int c = 0; c < w_; c++)
			{
				//decide which disc to print
				if (board[r][c] == 0)
					cout << getChar(board, r, c);
				else if (board[r][c] == 1)
					printColor(getChar(board, r, c), LIGHTRED);
				else if (board[r][c] == 2)
					printColor(getChar(board, r, c), LIGHTBLUE);

				cout << "   |   ";
			}

		//Second Row of | with no discs
			cout << "\n|   ";

			for (int i = 0; i < w_; i++)
				cout << "    |   ";

		//Bottom of each cell ----+----
			cout << "\n+";

			for (int i = 0; i < w_; ++i)
				cout << "-------+";

			cout << "\n";
	}
}



char getChar(int board[][w_], int r, int c)
{
	if (board[r][c] == 0)
		return ' ';
	else if (board[r][c] == 1 || board[r][c] == 2)
		return 'O';
}



void copyBoard(int board[][w_], int newBoard[][w_])
{
	for (int r = 0; r < h_; r++)
	{
		for (int c = 0; c < w_; c++)
		{
			newBoard[r][c] = board[r][c];
		}
	}
}



void printColor(string str, int color)
{
	SetConsoleTextAttribute(H, color);
	cout << str;
	SetConsoleTextAttribute(H, WHITE);
}

void printColor(int i, int color)
{
	SetConsoleTextAttribute(H, color);
	cout << i;
	SetConsoleTextAttribute(H, WHITE);
}

void printColor(char c, int color)
{
	SetConsoleTextAttribute(H, color);
	cout << c;
	SetConsoleTextAttribute(H, WHITE);
}