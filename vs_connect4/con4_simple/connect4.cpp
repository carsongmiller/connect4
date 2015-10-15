//connect4
//Carson Miller
//10-7-2015


/*

Things to modify:
-shallower recursion beginning, allow deeper as game goes on
-weigh certain situations at certain situations in the game more heavily
-when the board is symmetrical, the scores for each column should be symmetrical (fixed)
-consider adding a small bit of randomness into minimax
	-like if two options are both very good and within %20 of eachother or something
	-add a chance that the "worse" one is chosen
-weigh scores better

-consider: run through minimax until the first instance of a win is found, then go some certain number of iterations deeper
-this would be overwritten by logic that makes minimax go shallower at the beginning

-add print functions to clean up main()

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
const int MAX_ITER = 6;


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

//detects if the disc at board[r][c] has won
bool winDetect(int board[][w_], int r, int c, int who);

//detects if "who" is one disc away from winning
int nearWinDetect(int board[][w_], int who);

//detects if "who" has two places they could win
bool trapDetect(int board[][w_], int who);

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

//initializes the board
void initBoard(int board[][w_]);

//prints out which player is which color
void printPlayerColors();

//checks if a cell is in bounds of board[][]
bool isValidCell(int board[][w_], int r, int c);



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
		initBoard(board);

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

				if (moveChoice > w_)
					cout << "There is no column " << moveChoice << ". Please choose a column between 1 and " << w_ << "\n\n";

				else if (!playMove(board, moveChoice - 1, pDisc))
					cout << "There is no space in that column, choose a different one\n\n";

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

			cout << "Now its' the computer's turn\n\n";
			cout << "thinking ...\n\n";

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

				if (winDetect(board, rowPlayed, colPlayed, cDisc))
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
				if (winDetect(board, rowPlayed, moveChoice - 1, pDisc))
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



void minimax(int board[][w_], int score[], int who, int currentCheck, int iter)
{
	int newBoard[h_][w_];
	copyBoard(board, newBoard);

	//printBoard(newBoard); //debug

	int nearWinP;
	int nearWinC;
	int rowPlayed;

	if (iter <= MAX_ITER)
	{
		for (int i = 0; i < w_; i++)
		{
			if (iter == 0 && i == 0)
			{
				nearWinC = nearWinDetect(newBoard, cDisc); //checks for immediate possibility to win
				if (nearWinC != -1)
				{
					score[nearWinC] += 1000000;
					break;
				}

				nearWinP = nearWinDetect(newBoard, pDisc); //checks for immediate need to block
				if (nearWinP != -1)
				{
					score[nearWinP] += 1000000;
					break;
				}
			}

			rowPlayed = playMove(newBoard, i, currentCheck);

			if (rowPlayed != -1)
			{
				if (winDetect(newBoard, rowPlayed, i, cDisc))
				{
					if (iter == 0)
					{
						//score[i] += 1000;
					}
					else
						//score[i] += (MAX_ITER - iter + 1) / (iter + 1);
						score[i] += pow(7, (1.0*MAX_ITER) / iter);
				}


				else if (winDetect(newBoard, rowPlayed, i, pDisc))
				{
					if (iter == 1)
					{
						//score[i] -= 1000;
					}
					else
						//score[i] -= (MAX_ITER - iter + 1) / (iter + 1);
						score[i] += pow(7, (1.0*MAX_ITER) / iter);
				}


				else
				{
					if (currentCheck == cDisc)
						currentCheck = pDisc;
					else if (currentCheck == pDisc)
						currentCheck = cDisc;

					minimax(newBoard, score, who, currentCheck, iter + 1);
				}
			}

			unPlayMove(newBoard, i);
		}
	}
}



bool winDetect(int board[][w_], int r, int c, int who)
{
	int consec;

	//checking for horizontal
		consec = 0;
		for (int i = c - 3; i <= c + 3; i++)
		{
			if (isValidCell(board, r, i) && board[r][i] == who)
			{
				if (++consec >= 4)
					return true;
			}
			else
				consec = 0;
		}

	//checking for vertical
		consec = 0;
		for (int i = r - 3; i <= r + 3; i++)
		{
			if (isValidCell(board, i, c) && board[i][c] == who)
			{
				if (++consec >= 4)
					return true;
			}
			else
				consec = 0;
		}


	//checking for diagonal down
		consec = 0;
		for (int i = -3; i <= 3; i++)
		{
			if (isValidCell(board, r + i, c + i) && board[r + i][c + i] == who)
			{
				if (++consec >= 4)
					return true;
			}
			else
				consec = 0;
		}

	//checking for diagonal up
		consec = 0;
		for (int i = -3; i <= 3; i++)
		{
			if (isValidCell(board, r - i, c + i) && board[r - i][c + i] == who)
			{
				if (++consec >= 4)
					return true;
			}
			else
				consec = 0;
		}

	return false;
}




int nearWinDetect(int board[][w_], int who)
{
	int rowPlayed;

	for (int i = 0; i < w_; i++)
	{
		rowPlayed = playMove(board, i, who);

		if (winDetect(board, rowPlayed, i, who))
		{
			unPlayMove(board, i);
			return i;
		}

		else
		{
			unPlayMove(board, i);
		}
	}

	return -1;
}



bool trapDetect(int board[][w_], int who)
{
	int rowPlayed1, rowPlayed2;

	for (int i = 0; i < w_; i++)
	{
		rowPlayed1 = playMove(board, i, who); //play first move

		if (rowPlayed1 != -1 && winDetect(board, rowPlayed1, i, who))
		{
			rowPlayed2 = playMove(board, i, who); //play second move
			if (rowPlayed2 != -1 && winDetect(board, rowPlayed2, i, who))
			{
				unPlayMove(board, i);
				unPlayMove(board, i);
				return true;
			}
			if(rowPlayed2 != -1)
				unPlayMove(board, i);
		}

		if(rowPlayed1 != -1)
			unPlayMove(board, i);
	}
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
	for (int i = h_ - 1; i > 0; i--)
	{
		if (board[i][col] != nDisc && board[i - 1][col] == nDisc)
		{
			board[i][col] = nDisc;
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



void initBoard(int board[][w_])
{
	for (int r = 0; r < h_; r++)
		{
			for (int c = 0; c < w_; c++)
			{
				board[r][c] = nDisc;
			}
		}
}



void printPlayerColors()
{
	cout << "Player: ";
	printColor('O', LIGHTRED);
	cout << endl;
	cout << "Computer: ";
	printColor('O', LIGHTBLUE);
	cout << endl;
}



bool isValidCell(int board[][w_], int r, int c)
{
	if (r >= 0 && r < h_ && c >= 0 && c < w_)
		return true;
	else
		return false;
}