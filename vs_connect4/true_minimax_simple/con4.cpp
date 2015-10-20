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
#include <limits>

using namespace std;


//pDisc =  disc; cDisc = computer disc; nDisc = no disc
const int pDisc = 1, cDisc = 2, nDisc = -1;
//width and height variables
const int w_ = 7, h_ = 6;
//Base number of maximum iterations (depending on the stage in the game, recursion may go more or less deep)
int MAX_DEPTH = 5;


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


/*
---------------------------
ALGORITHMS/HEURISTICS
---------------------------
*/

	//does a static evaluation of the board
	int staticEval(int board[][w_]);

	//ultimate returns column of the best move for the computer
	int minimax(int board[][w_], int maximizer, int minormax, int depth);

	//determines the best move for "who"
	//void OLDminimax(int board[][w_], int score[], int &tempScore, int who, int currentCheck, int iter);

	//checks if the disc at board[r][c] has won
	bool winDetect(int board[][w_], int r, int c, int who);

	//checks if "who" is one disc away from winning
	int nearWinDetect(int board[][w_], int who);

	//checks if "who" has two places they could win stacked on top of eachother (returns column)
	bool vTrapDetect(int board[][w_], int who);

	//checks if "who" has a horizontal trap set up (O X X X O) (returns right open column)
	bool hTrapDetect(int board[][w_], int who);

	//checks if playing in the given column would create a win for the other player
	bool createLoss(int board[][w_], int col, int who);

/*
---------------------------
BOARD MANIPULATION
---------------------------
*/

	//returns true if move is valid, returns false if not
	int playMove(int board[][w_], int col, int who);

	//deletes the top disc in the given column
	bool unPlayMove(int board[][w_], int col);

	//copies the board
	void copyBoard(int board[][w_], int newBoard[][w_]);

	//initializes the board
	void boardInit(int board[][w_]);

	//sets a cell with no logical checks
	int setCell(int board[][w_], int r, int c, int who);

	//player choosing his move and making move
	void humanTurn(int board[][w_], int &rowPlayed, int &colPlayed);

	void compTurn(int board[][w_], int &colPlayed, int &rowPlayed);

/*
---------------------------
PRINT FUNCTIONS
---------------------------
*/

	//prints the board
	void printBoard(int board[][w_]);

	//takes care of printing most of the screen
	//void OLDprintScreen(int board[][w_], int score[]);
	void printScreen(int board[][w_]);

	//prints a given string of text in a different color then changes the text color back to white (overloaded for string, int, and char arguments)
	void printColor(string str, int color);
	void printColor(int i, int color);
	void printColor(char c, int color);

	//prints the score[] array (mostly for debugging)
	void printScore(int score[]);

	//prints out which player is which color
	void printPlayerColors();

	//checks for a win for the given player and prints a statement if there is a win
	bool printWin(int board[][w_], int r, int c, int who);

/*
---------------------------
OTHER HELPER FUNCTIONS
---------------------------
*/

	//returns an appropriate symbol for the given index when given the board
	char getChar(int board[][w_], int r, int c);

	//ranks the scores of columns
	void rankScores(int score[], int rankedScore[]);

	//resets the score[] array
	void scoreReset(int score[]);

	//resets the rankedScore[] array
	void rankedScoreReset(int rankedScore[]);

	//checks if a cell is in bounds of board[][]
	bool isValidCell(int board[][w_], int r, int c);

	//Picks who goes first
	int preGame();



int main()
{
	int board[h_][w_]; //the main board
	bool cont = true, validMove, newGame = true; //determines whether the game should continue
	char first; //who will go first
	bool validFirst = false; //makes sure first is a valid entry
	int moveChoice; //human player's move choice
	int score[w_]; //stores the scores of each column (handed to minimax())
	int rankedScore[w_]; //stores the scores in rank order
	char yn; //takes input from player as to whether they want to play again
	int rowPlayed; //stores the row in which the last disc was played
	int colPlayed; //stores the column in which the last disc was played
	int tempScore = 0;
	int whosTurn; //keeps track of who's turn it is (1 = player1/human, 2 = player2/computer)
	int turn = 0; //keeps track of which turn it is

	while (newGame)
	{
		boardInit(board);
		whosTurn = preGame();
		printScreen(board);

		while (turn < w_*h_)
		{
			if (whosTurn == 1) //player's turn
			{
				humanTurn(board, rowPlayed, colPlayed);
				printScreen(board);
				if (printWin(board, rowPlayed, colPlayed, pDisc)) break;
			}

			else if (whosTurn == 2) //computer's turn
			{
				compTurn(board, colPlayed, rowPlayed);
				printScreen(board);
				cout << "The computer played in column ";
				printColor(colPlayed, GREEN);
				cout << "\n\n";
				if (printWin(board, rowPlayed, colPlayed, cDisc)) break;
			}

			if (whosTurn == 1) whosTurn = 2;
			else whosTurn = 1;
		}

		cout << "Would you like to play again? (y/n): ";
		cin >> yn;
		if (yn != 'y' && yn != 'Y')
			newGame = false;
	}

	return 0;
}



/*
=========================================================

ALGORITHMS/HEURISTICS

=========================================================
*/



int staticEval(int board[][w_], int maximizer)
{

	//Checking for win detection will be taken care of in minimax()
	//A board will only be sent here if there is no win found in the board

	int minimizer;
	if (maximizer == cDisc) minimizer = pDisc;
	else minimizer = cDisc;

	int score = 0;
	int maxNearWinCount, minNearWinCount, maxVTrapCount, minVTrapCount;

	maxNearWinCount = nearWinDetect(board, maximizer);
	minNearWinCount = nearWinDetect(board, minimizer);
	maxVTrapCount = vTrapDetect(board, maximizer);
	minVTrapCount = vTrapDetect(board, minimizer);

	score += 5 * maxNearWinCount;
	score -= 5 * minNearWinCount;

	score += 10 * maxVTrapCount;
	score -= 10 * minVTrapCount;

	if (hTrapDetect(board, maximizer)) score += 2;
	else if (hTrapDetect(board, minimizer)) score -= 2;

	return score;
}



int minimax(int board[][w_], int maximizer, int minormax, int depth)
{
	//"original_caller" keeps track of which player originally called minimax()
	//"minormax" keeps track of whether minimax() is currently evaulating a min or a max node (1 = min, 2 = max)

	int newBoard[h_][w_]; //creating a copy of the board
	copyBoard(board, newBoard);
	
	int score[w_]; //creating clean score[] array
	for (int i = 0; i < w_; i++)
		score[i] = 0;

	int R, C;

	for (C = 0; C < w_; C++)
	{
		R = playMove(newBoard, C, minormax);

		if (R != -1)
		{
			if (winDetect(newBoard, R, C, minormax)) //checks for a win for original_caller
			{
				if (minormax == maximizer)
					score[C] = 1000000;
				else
					score[C] = -1000000;
			}

			else if (depth == MAX_DEPTH)
				score[C] = staticEval(newBoard, maximizer);

			else
			{
				if(minormax == cDisc)
					score[C] = minimax(newBoard, maximizer, pDisc, depth + 1);
				else
					score[C] = minimax(newBoard, maximizer, cDisc, depth + 1);
			}

			unPlayMove(newBoard, C);
		}

		else
			score[C] = INT_MIN; //INT_MIN will be the value for an invalid cell
	}

	int low, high;

	for (int i = 0; i < w_; i++) //initializing low and high to a valid cell
	{
		if (score[i] != INT_MIN)
		{
			low = i;
			high = i;
			break;
		}
	}

	for (int i = 0; i < w_; i++)
	{
		if (score[i] != INT_MIN)
		{
			if (score[i] > score[high])
				high = i;
			else if (score[i] < score[low])
				low = i;
		}
	}

	if (depth == 0)
	{
		if (minormax == maximizer) return high;
		else return low;
	}
	
	else
	{
		if (minormax == maximizer) return score[high];
		else return score[low];
	}
}



/*void OLDminimax(int board[][w_], int score[], int &tempScore, int who, int currentCheck, int iter, int turn)
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
					if (currentCheck == cDisc)
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

							if (currentCheck == cDisc)
								tempScore += pow(5, MAX_DEPTH - iter);
							else
								tempScore -= pow(4, MAX_DEPTH - iter);
						}

						if (turn >= 20)
						{
							if (vTrapDetect(board, currentCheck)) //checks for a vertical trap in favor of the computer
							{
								if (currentCheck == cDisc)
									tempScore += pow(3, MAX_DEPTH - iter);
								else
									tempScore -= pow(3, MAX_DEPTH - iter);
							}
						}
					}

					if (currentCheck == cDisc)
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
}*/



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
	int count = 0;

	for (int i = 0; i < w_; i++)
	{
		rowPlayed = playMove(board, i, who);
		if (rowPlayed != -1)
		{
			if (winDetect(board, rowPlayed, i, who))
			{
				unPlayMove(board, i);
				count++;
			}

			else
			{
				unPlayMove(board, i);
			}
		}
	}

	return count;
}



bool vTrapDetect(int board[][w_], int who)
{
	int rowPlayed1, rowPlayed2;
	int prev;
	int trapCount = 0;

	for (int i = 0; i < w_; i++)
	{
		rowPlayed1 = playMove(board, i, who); //play first move
		prev = setCell(board, rowPlayed1 - 1, i, nDisc); //avoids vertical win detection ("cleared cell 1")

		if (rowPlayed1 != -1 && winDetect(board, rowPlayed1, i, who))
		{
			setCell(board, rowPlayed1 - 1, i, prev); //replacing "cleared cell 1"
			rowPlayed2 = playMove(board, i, who); //play second move
			prev = setCell(board, rowPlayed2 - 1, i, nDisc); //avoids vertical win detection ("cleared cell 2")

			if (rowPlayed2 != -1 && winDetect(board, rowPlayed2, i, who))
			{
				setCell(board, rowPlayed2 - 1, i, prev); //replacing "cleared cell 2"
				unPlayMove(board, i);
				unPlayMove(board, i);
				trapCount++;
			}

			setCell(board, rowPlayed2 - 1, i, prev); //replacing "cleared cell 2"
			if (rowPlayed2 != -1)
				unPlayMove(board, i);
		}

		setCell(board, rowPlayed1 - 1, i, prev); //replacing "cleared cell 1"
		if (rowPlayed1 != -1)
			unPlayMove(board, i);
	}

	return trapCount;
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
				if (board[r][c] == nDisc && (board[r + 1][c] == cDisc || board[r + 1][c] == pDisc || !isValidCell(board, r + 1, c))) count++;
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

	if (who == pDisc) notWho = cDisc;
	else notWho = pDisc;

	for (int c1 = 0; c1 < w_; c1++)
	{
		rowPlayed1 = playMove(board, c1, who);
		if (rowPlayed1 != -1)
		{
			for (int c2 = 0; c2 < w_; c2++)
			{
				rowPlayed2 = playMove(board, c2, notWho);
				if (rowPlayed2 != -1)
				{
					if (winDetect(board, rowPlayed2, c2, notWho) == true)
					{
						unPlayMove(board, c2);
						unPlayMove(board, c1);
						return true;
					}
					unPlayMove(board, c2);
				}
			}
			unPlayMove(board, c1);
		}
	}

	return false;
}



/*
=========================================================

BOARD MANIPULATION

=========================================================
*/



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



void compTurn(int board[][w_], int &colPlayed, int &rowPlayed)
{
	cout << "COMPUTER'S TURN\n\nthinking ...";
	rowPlayed = minimax(board, cDisc, cDisc, 0);
	colPlayed = playMove(board, rowPlayed, cDisc);
}



void humanTurn(int board[][w_], int &rowPlayed, int &colPlayed)
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
		rowPlayed = playMove(board, colPlayed - 1, pDisc);

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



void boardInit(int board[][w_])
{
	for (int r = 0; r < h_; r++)
	{
		for (int c = 0; c < w_; c++)
		{
			board[r][c] = nDisc;
		}
	}
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



/*
=========================================================

PRINT FUNCTIONS

=========================================================
*/



void printScreen(int board[][w_])
{
	system("cls");
	cout << "CONNECT 4!\n";
	printPlayerColors();
	printBoard(board);
	//cout << "MAX_DEPTH: " << MAX_DEPTH;
	cout << "\n";
}



/*
void printScreen(int board[][w_], int score[])
{
	system("cls");
	cout << "CONNECT 4!\n";
	printPlayerColors();
	printBoard(board);
	printScore(score);
	//cout << "MAX_DEPTH: " << MAX_DEPTH;
	cout << "\n";
}
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



void printScore(int score[])
{
	for (int i = 0; i < w_; i++)
		cout << score[i] << "\t";
	cout << "\n";
}



bool printWin(int board[][w_], int r, int c, int who)
{
	if (winDetect(board, r, c, who))
	{
		if (who == cDisc)
			cout << "The Computer wins!\n\n";
		else
			cout << "You win!\n\n";
		return true;
	}
	else
		return false;
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



/*
=========================================================

OTHER HELPER FUNCTIONS

=========================================================
*/



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



char getChar(int board[][w_], int r, int c)
{
	if (board[r][c] == nDisc)
		return ' ';

	else if (board[r][c] == pDisc || board[r][c] == cDisc)
		return 'O';

	else
		return 'X'; //something went wrong
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



bool isValidCell(int board[][w_], int r, int c)
{
	if (r >= 0 && r < h_ && c >= 0 && c < w_)
		return true;
	else
		return false;
}



int preGame()
{
	char first;

	cout << "CONNECT 4!\n\n Who would you like to go first? (m for me, c for computer): ";
	cin >> first;

	while (true)
	{
		if (first == 'm' || first == 'M') return 1;

		else if (first == 'c' || first == 'C') return 2;

		else
		{
			cout << "\nPlease enter either m or c: ";
			cin >> first;
		}
	}
}