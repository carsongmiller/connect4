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

-minimax heavily favors left side.  fix this

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
const int pDisc = 1, cDisc = 2, nDisc = -1;
//width and height variables
const int w_ = 7, h_ = 6;
//Base number of maximum iterations (depending on the stage in the game, recursion may go more or less deep)
const int MAX_ITER = 4;


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
int playMove(int board[][w_], int col, int who);

//deletes the top disc in the given column
bool unPlayMove(int board[][w_], int col);

//returns how many winning arrangements (combinations) the given cell is involved in
int numWinComb(int board[][w_], int r, int c);

//constructs 3D array of [row][column][comb#] = (string)comb
void makeCombTable(string *** combTable, int board[][w_]);

//detects if "who" has won
//bool winDetectOLD(int board[][w_], int who);

//detects if the disc at board[r][c] has won
bool winDetect(int board[][w_], int r, int c);

//detects if "who" is one disc away from winning
int nearWinDetectA(int board[][w_], int who);
int nearWinDetectB(int board[][w_], int who);

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

//ranks the scores of columns
void rankScores(int score[], int rankedScore[]);

//prints the score[] array (mostly for debugging)
void printScore(int score[]);

//resets the score[] array
void scoreReset(int score[]);

//resets the rankedScore[] array
void rankedScoreReset(int rankedScore[]);



int main()
{
	int board[h_][w_]; //the main board
	bool cont = true, validMove, newGame = true; //determines whether the game should continue
	char whoFirst; //who will go first
	int moveChoice; //human player's move choice
	int score[w_]; //stores the scores of each column (handed to minimax())
	int rankedScore[w_]; //stores the scores in rank order
	int bestScore; //stores the number of the column with the best score
	char yn; //takes input from player as to whether they want to play again
	int rowPlayed; //stores the row in which the last disc was played
	int colPlayed; //stores the column in which the last disc was played
	//string *** combTable;

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

		cout << "SIMPLE CONNECT 4!\n\nWho should go first? (m for me, c for computer): ";
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
			scoreReset(score);
			rankedScoreReset(rankedScore);

			cout << "Now its' the computer's turn\n";
			cout << "\nthinking ...\n\n";

			minimax(board, score, cDisc, cDisc, 0);

			rankScores(score, rankedScore);


				cout << endl << endl;
								

			//now the computer will make its move
				for (int i = 0; i < w_; i++)
				{
					rowPlayed = playMove(board, rankedScore[i], cDisc);
					if (rowPlayed != -1)
					{
						colPlayed = rankedScore[i];
						break;
					}
				}

			system("cls");
			cout << "\nCONNECT 4!\n\n";

			printScore(score); //print the score[] array

			printBoard(board);

				if (winDetect(board, rowPlayed, colPlayed))
				{
					cout << "\nThe computer wins!\n";
					break;
				}


			//debug lines for nearWinDetect()
				//cout << "\n\ncDisc: " << nearWinDetect(board, cDisc) << "\n";
				//cout << "pDisc: " << nearWinDetect(board, pDisc) << "\n";

			//now the player's turn
			//Player chosing his move
			validMove = false;
				while (!validMove)
				{
					cout << "Where would you like to go? (enter column number): ";
					cin >> moveChoice;

					rowPlayed = playMove(board, moveChoice - 1, pDisc);

					if (rowPlayed == -1)
						cout << "There is no space in that column, choose a different one\n";
					else
						validMove = true;
				}

			system("cls");
			cout << "CONNECT 4!\n\n\n";
			printBoard(board);

			//detecting a player win	
				if (winDetect(board, rowPlayed, moveChoice - 1))
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



int playMove(int board[][w_], int col, int who)
{
	for (int i = h_ - 1; i >= 0; i--)
	{
		if (board[i][col] == nDisc)
		{
			board[i][col] = who;
			return i;
		}
	}

	return -1;
}



bool unPlayMove(int board[][w_], int col)
{
	for (int i = h_ - 1; i > 0; i++)
	{
		if (board[i][col] != nDisc && board[i - 1][col] == nDisc)
		{
			board[i][col] = 0;
			return true;
		}
		else if (i == 1 && board[0][col] != nDisc)
		{
			board[0][col] = nDisc;
			return true;
		}
	}

	return false;
}



void minimax(int board[][w_], int score[], int who, int currentCheck, int iter)
{
	int newBoard[h_][w_];
	copyBoard(board, newBoard);

	//printBoard(newBoard); //debug

	int nearWinP;
	int rowPlayed;

	if (iter <= MAX_ITER)
	{
		for (int i = 0; i < w_; i++)
		{
			rowPlayed = playMove(newBoard, i, currentCheck);

			if (rowPlayed != -1)
			{
				/*if (iter == 0)
				{
					nearWinP = nearWinDetectA(newBoard, pDisc);
					if (nearWinP != -1)
						score[nearWinP] += 1000000;
				}*/
				
				if (winDetect(newBoard, rowPlayed, i))
				{
					if (iter == 0)
					{
						score[i] += 1000;
					}
					else
						score[i] += (MAX_ITER - iter);
				}


				else if (winDetect(newBoard, rowPlayed, i))
				{
					//printBoard(newBoard); //debug
					if (iter == 1)
					{
						score[i] -= 1000;
					}
					else
						score[i] -= (MAX_ITER - iter);
				}


				else if (!winDetect(newBoard, rowPlayed, i) && !winDetect(newBoard, rowPlayed, i))
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



/*int numWinComb(int board[][w_], int r, int c)
{
	int neighbor = 0;

		for (int i = 1; i <= 3; i++)
		{
			if (board[r + i][c] == nDisc) //straight right
				neighbor++;
			if (board[r - i][c] == nDisc) //straight left
				neighbor++;
			if (board[r][c + i] == nDisc) //straight up
				neighbor++;
			if (board[r][c - i] == nDisc) //straight down
				neighbor++;
			if (board[r + i][c + i] == nDisc) //diagonal up right
				neighbor++;
			if (board[r - i][c + i] == nDisc) //diagonal down right
				neighbor++;
			if (board[r - i][c - i] == nDisc) // diagonal down left
				neighbor++;
			if (board[r + i][c - i] == nDisc) // diagonal up left
				neighbor++;
		}

		return (neighbor - 8);
}*/



/*void makeCombTable(string *** combTable, int board[][w_])
{
	//assign tripple pointer to array
		combTable = new string**[h_];
		for (int i = 0; i < h_; i++)
		{
			combTable[i] = new string*[w_];

			for (int j = 0; j < w_; j++)
				combTable[i][j] = new string[numWinComb(board, i, j)];
		}

	//initialize array to all 3's ()
		for (int i = 0; i < h_; i++)
		{
			for (int j = 0; j < w_; j++)
			{
				for (int k = 0; k < numWinComb(board, i, j); k++)
					combTable[i][j][k] = 
			}
		}
}*/


// not having bound detection makes this a lot tougher ...

bool winDetect(int board[][w_], int r, int c)
{
	int who = board[r][c];
	bool win = false;
	int consec;

	//straight left
		consec = 0;
		for (int i = 0; i < 3; i++) 
		{
			if (c - i >= 0 && board[r][c-i] == who)
				consec++;
			else
				break;
		}

		if (consec == 3)
			return true;

	//straight right
		consec = 0;
		for (int i = 0; i < 3; i++) 
		{
			if (c + i < w_ && board[r][c + i] == who)
				consec++;
			else
				break;
		}
		if (consec == 3)
			return true;

	//diagonal up left
		consec = 0;
		for (int i = 0; i < 3; i++)
		{
			if (c - i >= 0 && r - i >= 0 && board[r - i][c - i] == who)
				consec++;
			else
				break;
		}
		if (consec == 3)
			return true;

	//diagonal up right
		consec = 0;
		for (int i = 0; i < 3; i++)
		{
			if (c + i < w_ && r - i >= 0 && board[r - i][c + i] == who)
				consec++;
			else
				break;
		}
		if (consec == 3)
			return true;

	//diagonal down left
		consec = 0;
		for (int i = 0; i < 3; i++)
		{
			if (c - i >= 0 && r + i < h_ && board[r + i][c - i] == who)
				consec++;
			else
				break;
		}
		if (consec == 3)
			return true;

	//diagonal down right
		consec = 0;
		for (int i = 0; i < 3; i++)
		{
			if (c + i < w_ && r + i < h_ && board[r + i][c + i] == who)
				consec++;
			else
				break;
		}
		if (consec == 3)
			return true;

	//straight up
		consec = 0;
		for (int i = 0; i < 3; i++)
		{
			if (r - i >= 0 && board[r - i][c] == who)
				consec++;
			else
				break;
		}
		if (consec == 3)
			return true;

	//straight down
		consec = 0;
		for (int i = 0; i < 3; i++)
		{
			if (r + i < h_ && board[r + i][c] == who)
				consec++;
			else
				break;
		}
		if (consec == 3)
			return true;
}



/*bool winDetectOLD(int board[][w_], int who)
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
}*/




int nearWinDetectA(int board[][w_], int who)
{
	//detecting horizontal wins
	for (int r = h_ - 1; r >= 0; r--)
	{
		for (int c = 0; c < w_ - 2; c++)
		{
			if (board[r][c] == who)
			{
				if (board[r][c + 1] == who)
				{
					if (board[r][c + 2] == who)
					{
						//making sure the winning spot is available & possible
						if (board[r][c + 3] == nDisc && board[r + 1][c + 3] != nDisc)
							return c + 3;
						else if (board[r][c - 1] == nDisc && board[r + 1][c - 1] != nDisc)
							return c - 1;
					}
				}
			}
		}
	}


	//detecting vertical wins
	for (int c = 0; c < w_; c++) //cycling through all columns
	{
		for (int r = h_ - 1; r > 2; r--) //cycling through sets of 3 cells in the same column
		{
			if (board[r][c] == who)
			{
				if (board[r - 1][c] == who)
				{
					if (board[r - 2][c] == who)
					{
						//making sure the winning spot is available & possible
						if (board[r - 3][c] == nDisc)
							return c;
					}
				}
			}
		}
	}


	//detcting diagonal-up wins
	for (int r = h_ - 1; r > 1; r--)
	{
		for (int c = 0; c < w_ - 2; c++)
		{
			if (board[r][c] == who)
			{
				if (board[r - 1][c + 1] == who)
				{
					if (board[r - 2][c + 2] == who)
					{
						//making sure the winning spot is available & possible
						if (board[r - 3][c + 3] == nDisc && board[r - 2][c + 3] != nDisc)
							return c + 3;
						else if (board[r + 1][c - 1] == nDisc && board[r + 2][c - 1] != nDisc)
							return c - 1;
					}
				}
			}
		}
	}

	//detecting diagonal-down wins
	for (int r = 0; r < h_ - 2; r++)
	{
		for (int c = 0; c < w_ - 2; c++)
		{
			if (board[r][c] == who)
			{
				if (board[r + 1][c + 1] == who)
				{
					if (board[r + 2][c + 2] == who)
					{
						//making sure the winning spot is available & possible
						if (board[r - 1][c - 1] == nDisc && board[r][c - 1] != nDisc)
							return c - 1;
						else if (board[r + 3][c + 3] == nDisc && board[r + 4][c + 3] != nDisc)
							return c + 3;
					}
				}
			}
		}
	}

	return -1;
}



int nearWinDetectB(int board[][w_], int who)
{
	int rowPlayed;

	for (int i = 0; i < w_; i++)
	{
		rowPlayed = playMove(board, i, who);
		//printBoard(board);

		if (winDetect(board, rowPlayed, i))
		{
			unPlayMove(board, i);
			//printBoard(board);
			return i;
		}

		else
		{
			unPlayMove(board, i);
			//printBoard(board);
		}
	}

	return -1;
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
			if (board[r][c] == nDisc)
				cout << getChar(board, r, c);
			else if (board[r][c] == pDisc)
				printColor(getChar(board, r, c), LIGHTRED);
			else if (board[r][c] == cDisc)
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
	if (board[r][c] == nDisc)
		return ' ';

	else if (board[r][c] == pDisc || board[r][c] == cDisc)
		return 'O';

	else
		return 'X'; //something went wrong
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




void rankScores(int score[], int rankedScore[])
{
	bool placed;

	rankedScore[0] = 0; //creates a base reference for the ranking

	for (int i = 1; i < w_; i++) //keeps track of which index of the score[] array it's checking
	{
		placed = false; //makes sure a column is only ranked once
		for (int n = 0; n < i; n++) //keeps track of which index of nextBest score[i] is being checked against
		{
			if (score[i] > score[rankedScore[n]])
			{
				for (int x = w_ - 1; x > n; x--) //shuffles up by one all indexes >= the one that needs to be changed
				{
					rankedScore[x] = rankedScore[x - 1];
				}

				rankedScore[n] = i;
				placed = true;
				break; //again makes sure a column is only ranked once
			}
		}

		if (!placed)
			rankedScore[i] = i;
	}
}



void printScore(int score[])
{
	for (int i = 0; i < w_; i++)
		cout << score[i] << "\t";
	cout << "\n";
}



void scoreReset(int score[])
{
	for (int i = 0; i < w_; i++)
		score[i] = 0;
}



void rankedScoreReset(int rankedScore[])
{
	for (int i = 0; i < w_; i++)
		rankedScore[i] = -1;
}