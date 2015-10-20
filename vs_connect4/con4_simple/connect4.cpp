//connect4
//Carson Miller
//10-7-2015


/*

TO DO:
-shallower recursion beginning, allow deeper as game goes on
-when the board is symmetrical, the scores for each column should be symmetrical (fixed)
-consider adding a small bit of randomness into minimax
	-like if two options are both very good and within %20 of eachother or something
	-add a chance that the "worse" one is chosen

-weigh scores better (possibly fixed)

*/



#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <cmath>

using namespace std;


//pDisc =  disc; cDisc = computer disc; nDisc = no disc
const int pDisc = 1, cDisc = 2, nDisc = -1;
//width and height variables
const int w_ = 7, h_ = 6;
//Base number of maximum iterations (depending on the stage in the game, recursion may go more or less deep)
int MAX_DEPTH;


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
int playMove(int board[][w_], int col, int who, int &turn);

//deletes the top disc in the given column
bool unPlayMove(int board[][w_], int col, int &turn);

//checks if the disc at board[r][c] has won
bool winDetect(int board[][w_], int r, int c, int who);

//checks if "who" is one disc away from winning
int nearWinDetect(int board[][w_], int who);

//checks if "who" has two places they could win stacked on top of eachother (returns column)
bool vTrapDetect(int board[][w_], int who);

//checks if "who" has a horizontal trap set up (O X X X O) (returns right open column)
bool hTrapDetect(int board[][w_], int who);

//determines the best move for "who"
void minimax(int board[][w_], long int score[], int &tempScore, int who, int currentCheck, int iter, int turn);

//checks if playing in the given column would create a win for the other player
bool createLoss(int board[][w_], int col, int who);

//prints the board
void printBoard(int board[][w_]);

//takes care of printing most of the screen
void printScreen(int board[][w_], long int score[]);

//copies the board
void copyBoard(int board[][w_], int newBoard[][w_]);

//prints a given string of text in a different color then changes the text color back to white (overloaded for string, int, and char arguments)
void printColor(string str, int color);
void printColor(int i, int color);
void printColor(char c, int color);

//returns an appropriate symbol for the given index when given the board
char getChar(int board[][w_], int r, int c);

//ranks the scores of columns
void rankScores(long int score[], long int rankedScore[]);

//prints the score[] array (mostly for debugging)
void printScore(long int score[]);

//resets the score[] array
void scoreReset(long int score[]);

//resets the rankedScore[] array
void rankedScoreReset(long int rankedScore[]);

//initializes the board
void initBoard(int board[][w_]);

//prints out which player is which color
void printPlayerColors();

//checks if a cell is in bounds of board[][]
bool isValidCell(int board[][w_], int r, int c);

//sets a cell with no logical checks
int setCell(int board[][w_], int r, int c, int who);

//player choosing his move and making move
void playerMove(int board[][w_], long int score[], int &turn, int &rowPlayed, int &colPlayed);



int main()
{
	int board[h_][w_]; //the main board
	bool cont = true, validMove, newGame = true; //determines whether the game should continue
	char whoFirst; //who will go first
	int moveChoice; //human player's move choice
	long int score[w_]; //stores the scores of each column (handed to minimax())
	long int rankedScore[w_]; //stores the scores in rank order
	char yn; //takes input from player as to whether they want to play again
	int rowPlayed; //stores the row in which the last disc was played
	int colPlayed; //stores the column in which the last disc was played
	int turn; //keeps track of which turn # it is
	int tempScore = 0;

	while (newGame)
	{
		turn = 0;
		initBoard(board);
		scoreReset(score);

		//First output of the program:
		system("cls");

		cout << "CONNECT 4!\n\nWho should go first? (m for me, c for computer): ";
		cin >> whoFirst;

		if (whoFirst == 'm' || whoFirst == 'M')
		{
			printScreen(board, score);
			playerMove(board, score, turn, rowPlayed, colPlayed);
		}

		//MAIN GAME LOOP

		while (cont)
		{
			MAX_DEPTH = (turn / 15) + 6;

			printScreen(board, score);

			scoreReset(score);
			rankedScoreReset(rankedScore);

			cout << "Now its' the computer's turn\n\n";
			cout << "thinking ...\n\n";

			minimax(board, score, tempScore, cDisc, cDisc, 0, turn);

			rankScores(score, rankedScore);


				cout << endl << endl;
								

			//now the computer will make its move
				for (int i = 0; i < w_; i++)
				{
					rowPlayed = playMove(board, rankedScore[i], cDisc, turn);
					if (rowPlayed != -1)
					{
						colPlayed = rankedScore[i];
						break;
					}
				}

				printScreen(board, score);

				cout << "The computer played in column ";
				printColor(colPlayed+1, GREEN);
				cout << "\n";


				if (winDetect(board, rowPlayed, colPlayed, cDisc))
				{
					cout << "\nThe computer wins!\n";
					break;
				}


			//now the player's turn
				playerMove(board, score, turn, rowPlayed, colPlayed);

				printScreen(board, score);

			//detecting a player win	
				if (winDetect(board, rowPlayed, colPlayed - 1, pDisc))
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



void minimax(int board[][w_], long int score[], int &tempScore, int who, int currentCheck, int iter, int turn)
{
	int newBoard[h_][w_];
	copyBoard(board, newBoard);

	//printBoard(newBoard); //debug

	int nearWinP, nearWinC, rowPlayed, vTrap, hTrap;

	if (iter <= MAX_DEPTH)
	{
		for (int i = 0; i < w_; i++)
		{
			//play in the center on the first turn
			if (turn <= 1)
			{
				score[w_ / 2] += 1000000;
				break;
			}

			//immediate block or win
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


			rowPlayed = playMove(newBoard, i, currentCheck, turn);

			if (rowPlayed != -1)
			{
				//printScreen(newBoard, score);
				if (winDetect(newBoard, rowPlayed, i, currentCheck)) //checks for a win
				{
					if(currentCheck == cDisc)
						tempScore += pow(4, 1.0*MAX_DEPTH - iter);
					else
						tempScore -= pow(3, 1.0*MAX_DEPTH - iter);
				}


			else //if no wins were found
				{
					if (currentCheck == cDisc && createLoss(board, i, who))
						tempScore -= 10000;

					if (turn >= 2 && turn < 20)
					{
						if (hTrapDetect(newBoard, currentCheck))
						{
							//cout << hTrap << "\t" << i << "\t" << iter << endl;
						
								if(currentCheck == cDisc)
									tempScore += pow(5, MAX_DEPTH - iter);
								else
									tempScore -= pow(4, MAX_DEPTH - iter);
						}

						/*if (turn >= 20)
						{
							if (vTrapDetect(board, currentCheck)) //checks for a vertical trap in favor of the computer
							{
								if (currentCheck == cDisc)
									tempScore += pow(3, MAX_DEPTH - iter);
								else
									tempScore -= pow(3, MAX_DEPTH - iter);
							}
						}*/
					}

					if(currentCheck == cDisc)
						minimax(newBoard, score, tempScore, who, pDisc, iter + 1, turn);
					else
						minimax(newBoard, score, tempScore, who, cDisc, iter + 1, turn);
				}
			}

			unPlayMove(newBoard, i, turn);

			if (iter == 0)
			{
				score[i] = tempScore;
				tempScore = 0;
			}

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
	int turn = 0;

	for (int i = 0; i < w_; i++)
	{
		rowPlayed = playMove(board, i, who, turn);
		if (rowPlayed != -1)
		{
			if (winDetect(board, rowPlayed, i, who))
			{
				unPlayMove(board, i, turn);
				return i;
			}

			else
			{
				unPlayMove(board, i, turn);
			}
		}
	}

	return -1;
}



bool vTrapDetect(int board[][w_], int who)
{
	int rowPlayed1, rowPlayed2;
	int prev;
	int turn = 0;

	for (int i = 0; i < w_; i++)
	{
		rowPlayed1 = playMove(board, i, who, turn); //play first move
		prev = setCell(board, rowPlayed1 - 1, i, nDisc); //avoids vertical win detection ("cleared cell 1")

		if (rowPlayed1 != -1 && winDetect(board, rowPlayed1, i, who))
		{
			setCell(board, rowPlayed1 - 1, i, prev); //replacing "cleared cell 1"
			rowPlayed2 = playMove(board, i, who, turn); //play second move
			prev = setCell(board, rowPlayed2 - 1, i, nDisc); //avoids vertical win detection ("cleared cell 2")

			if (rowPlayed2 != -1 && winDetect(board, rowPlayed2, i, who))
			{
				setCell(board, rowPlayed2 - 1, i, prev); //replacing "cleared cell 2"
				unPlayMove(board, i, turn);
				unPlayMove(board, i, turn);
				return true;
			}

			setCell(board, rowPlayed2 - 1, i, prev); //replacing "cleared cell 2"
			if(rowPlayed2 != -1)
				unPlayMove(board, i, turn);
		}

		setCell(board, rowPlayed1 - 1, i, prev); //replacing "cleared cell 1"
		if(rowPlayed1 != -1)
			unPlayMove(board, i, turn);
	}

	return false;
}



bool hTrapDetect(int board[][w_], int who)
{
	int count;
	int notWho;

	if (who == pDisc) notWho = cDisc;
	else notWho = pDisc;

	for (int r = h_ - 1; r >= 0; r--)
	{
		count = 0;
		for (int c = 0; c < w_; c++)
		{
			if (count == 0 || count == 4)
			{
				//open spaces on the sides must also have an open space beneath them
				if (board[r][c] == nDisc && (board[r+1][c] == cDisc || board[r+1][c] == pDisc || !isValidCell(board, r+1, c))) count++;
				else count = 0;
			}
			else if (count > 0 && count < 4)
			{
				if (board[r][c] == who) count++;
				else if (board[r][c] == notWho) count = 0;
			}
			else if (count == 5)
				return true;
		}
	}
	if (count < 5) return false;
	else return true;
}



bool createLoss(int board[][w_], int col, int who)
{
	int rowPlayed1, rowPlayed2, notWho;
	int falseTurn = 0;

	if (who == pDisc) notWho = cDisc;
	else notWho = pDisc;

	for (int c1 = 0; c1 < w_; c1++)
	{
		rowPlayed1 = playMove(board, c1, who, falseTurn);
		if (rowPlayed1 != -1)
		{
			for (int c2 = 0; c2 < w_; c2++)
			{
				rowPlayed2 = playMove(board, c2, notWho, falseTurn);
				if (rowPlayed2 != -1)
				{
					if (winDetect(board, rowPlayed2, c2, notWho) == true)
					{
						unPlayMove(board, c2, falseTurn);
						unPlayMove(board, c1, falseTurn);
						return true;
					}
					unPlayMove(board, c2, falseTurn);
				}
			}
			unPlayMove(board, c1, falseTurn);
		}
	}

	return false;
}



int playMove(int board[][w_], int col, int who, int &turn)
{
	for (int i = h_ - 1; i >= 0; i--)
	{
		if (board[i][col] == nDisc)
		{
			board[i][col] = who;
			turn++;
			return i;
		}
	}

	return -1;
}



bool unPlayMove(int board[][w_], int col, int &turn)
{
	for (int i = h_ - 1; i > 0; i--)
	{
		if (board[i][col] != nDisc && board[i - 1][col] == nDisc)
		{
			board[i][col] = nDisc;
			turn--;
			return true;
		}
		else if (i == 1 && board[0][col] != nDisc)
		{
			board[0][col] = nDisc;
			turn--;
			return true;
		}
	}

	return false;
}



void rankScores(long int score[], long int rankedScore[])
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



void playerMove(int board[][w_], long int score[], int &turn, int &rowPlayed, int &colPlayed)
{
	//takes care of most of a human player's turn

	bool isValid;
	
	isValid = false;
	cout << "Where would you like to go? (enter column number): ";
	cin >> colPlayed;

	while (!isValid)
	{
		if (colPlayed > w_ || colPlayed < 1)
		{
			cout << "There is no column " << colPlayed << ". Please choose a column between 1 and " << w_ << "\n\n";
			
			cout << "Where would you like to go? (enter column number): ";
			cin >> colPlayed;
		}
		else
			break;
	}

	while (!isValid)
	{
		rowPlayed = playMove(board, colPlayed - 1, pDisc, turn);

		if (rowPlayed == -1)
		{
			cout << "There is no space in that column, choose a different one\n\n";
			
			cout << "Where would you like to go? (enter column number): ";
			cin >> colPlayed;
		}

		else
			break;
	}
}



/*
	
	PRINT FUNCTIONS

*/



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



void printScore(long int score[])
{
	for (int i = 0; i < w_; i++)
		cout << score[i] << "\t";
	cout << "\n";
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



void printScreen(int board[][w_], long int score[])
{
	system("cls");
	cout << "CONNECT 4!\n";
	printPlayerColors();
	printBoard(board);
	printScore(score);
	//cout << "MAX_DEPTH: " << MAX_DEPTH;
	cout << "\n";
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



void scoreReset(long int score[])
{
	for (int i = 0; i < w_; i++)
		score[i] = 0;
}



void rankedScoreReset(long int rankedScore[])
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



bool isValidCell(int board[][w_], int r, int c)
{
	if (r >= 0 && r < h_ && c >= 0 && c < w_)
		return true;
	else
		return false;
}



int setCell(int board[][w_], int r, int c, int who)
{
	int prev = board[r][c];
	if (isValidCell(board, r, c))
	{
		board[r][c] = who;
		return prev;
	}
	else
		return 0;
}
