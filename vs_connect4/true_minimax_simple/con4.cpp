//connect4
//Carson Miller
//10-7-2015


/*

TO DO:

-make sure alpha beta pruning is working correctly

*/



#include <iostream>
#include <fstream>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <cmath>
#include <limits>
#include <ctime>

//#define _DEBUG

//#define _STATIC_EVAL_DEBUG
//#define _AB_DEBUG
//#define _BOARD_DEBUG

#define _USE_AB

#define w_				7
#define h_				6

#define pDisc			1
#define cDisc			2
#define nDisc			0

#define LONG_THOUGHT	false
#define TYPICAL_DEPTH	8
#define EARLY_DEPTH		4

using namespace std;

//int MAX_DEPTH;

#ifdef _DEBUG
	ofstream debug;
#endif


//Values for SetConsoleTextAttribute()

#define BLACK			0
#define BLUE			1
#define GREEN			2
#define CYAN			3
#define RED				4
#define MAGENTA			5
#define BROWN			6
#define LIGHTGRAY		7
#define DARKGRAY		8
#define LIGHTBLUE		9
#define LIGHTGREEN		10
#define LIGHTCYAN		11
#define LIGHTRED		12
#define LIGHTMAGENTA	13
#define YELLOW			14
#define WHITE			15

#define PLAYER_COLOR	YELLOW
#define COMP_COLOR		LIGHTRED
#define GAME_COLOR		LIGHTCYAN


/*
---------------------------
ALGORITHMS/HEURISTICS
---------------------------
*/

	//does a static evaluation of the board
	int staticEval(int board[][w_], int maximizer, int turn);

	//ultimately returns column of the best move for the computer
	int minimax(int board[][w_], int maximizer, int minormax, int depth, int MAX_DEPTH, int turn);
	int ab_minimax(int board[][w_], int maximizer, int minormax, int depth, int MAX_DEPTH, int turn, int alpha, int beta, int &cutCount);

	//checks if the disc at board[r][c] has won
	bool winDetect(int board[][w_], int r, int c, int who);

	//checks if "who" is one disc away from winning (returns column to block)
	int nearWinDetect(int board[][w_], int who);

	//returns the number of places "who" could win in one move
	int nearWinCount(int board[][w_], int who);

	//returns the number of verticle traps that "who" has on the board
	int vTrapCount(int board[][w_], int who);

	//returns the column of first verticle trap found (or -1 if none found)
	int vTrapDetect(int board[][w_], int who);

	//returns whether "who" has a horizontal trap set up (O X X X O) (returns right open column)
	bool hTrapDetect(int board[][w_], int who);

	//returns the # of places on the board (even if they're not available yet) where one disc could complete a string of 4
	int oneToWinCount(int board[][w_], int who);

/*
---------------------------
BOARD MANIPULATION
---------------------------
*/

	//returns true if move is valid, returns false if not
	int playMove(int board[][w_], int col, int who);

	//deletes the top disc in the given column
	bool unPlayMove(int board[][w_], int col);

	//initializes the board
	void boardInit(int board[][w_]);

	//sets a cell with no logical checks
	int setCell(int board[][w_], int r, int c, int who);

	//player choosing his move and making move
	void playerTurn(int board[][w_], int &rowPlayed, int &colPlayed);

	//computer choosing and playing move
	void compTurn(int board[][w_], int &rowPlayed, int &colPlayed, int &turn, int MAX_DEPTH, int &cutCount);

/*
---------------------------
PRINT FUNCTIONS
---------------------------
*/

	//prints the board
	void printBoard(int board[][w_]);
	void printBoard(int board[][w_], ofstream &output); //overloaded for debuging

	//takes care of printing most of the screen
	void printScreen(int board[][w_]);

	//prints a given string of text in a different color then changes the text color back to white (overloaded for string, int, and char arguments)
	void printColor(string str, int color);
	void printColor(string str, int color, ofstream &output); //overloaded for debuging
	void printColor(int i, int color);
	void printColor(int i, int color, ofstream &output); //overloaded for debuging
	void printColor(char c, int color);
	void printColor(char c, int color, ofstream &output); //overloaded for debuging

	//prints out which player is which color
	void printPlayerColors();

/*
---------------------------
OTHER HELPER FUNCTIONS
---------------------------
*/

	//returns an appropriate symbol for the given index when given the board
	char getChar(int board[][w_], int r, int c);

	//checks if a cell is in bounds of board[][]
	bool isValidCell(int board[][w_], int r, int c);

	//picks who goes first
	int preGame();



int main()
{
	srand(time(0));

	#ifdef _DEBUG
		debug.open("debug.txt");
		if (debug.fail())
			cout << "Failed to open debug stream";
	#endif

	int board[h_][w_];		//the main board
	bool newGame = true;	//whether to start a new game
	int endOfGame;			//6 = yes, 7 = no
	int rowPlayed = 0;		//stores the row in which the last disc was played
	int colPlayed = 0;		//stores the column in which the last disc was played
	int whosTurn;			//keeps track of who's turn it is (1 = player1/human, 2 = player2/computer)
	int turn;				//keeps track of how mant discs have been played
	int MAX_DEPTH;			//max depth of recursion for minimax
	int winner = 0;			//winner at end of game
	int cutCount;			//number of terminal nodes cut

	while (newGame)
	{
		boardInit(board);
		system("cls");
		whosTurn = preGame();
		printScreen(board);

		turn = 0;

		while (turn < w_*h_)
		{
			if (turn < 2) MAX_DEPTH = EARLY_DEPTH;
			else if (MAX_DEPTH > w_*h_ - turn) MAX_DEPTH = w_*h_ - turn;
			else MAX_DEPTH = TYPICAL_DEPTH;

			if (whosTurn == 1) //player's turn
			{
				playerTurn(board, rowPlayed, colPlayed);
				printScreen(board);
				if (winDetect(board, rowPlayed, colPlayed, pDisc))
				{
					winner = 1;
					break;
				}
			}

			else if (whosTurn == 2) //computer's turn
			{
				cutCount = 0;
				compTurn(board, rowPlayed, colPlayed, turn, MAX_DEPTH, cutCount);
				printScreen(board);
				cout << "COMPUTER PLAYED: ";
				printColor(colPlayed+1, GAME_COLOR);
				cout << "\n\n";

				//cout << "CutCount: " << cutCount << "\n\n";

				#ifdef _AB_DEBUG
					cout << "CutCount: " << cutCount << "\n\n";
				#endif

				if (winDetect(board, rowPlayed, colPlayed, cDisc))
				{
					winner = 2;
					break;
				}
				Sleep(1000);			
			}

			if (whosTurn == 1) whosTurn = 2;
			else whosTurn = 1;

			turn++;
			if (turn >= w_*h_) break;
		}

		//Displaying an end game message box
		switch (winner)
		{
		case 0:
			endOfGame = MessageBox(NULL, TEXT("Cat's Game!\n\nPlay again?"), TEXT("End of Game"), MB_YESNO);
			break;

		case 1:
			endOfGame = MessageBox(NULL, TEXT("You Win!\n\nPlay again?"), TEXT("End of Game"), MB_YESNO);
			break;

		case 2:
			endOfGame = MessageBox(NULL, TEXT("The Computer Wins!\n\nPlay again?"), TEXT("End of Game"), MB_YESNO);
			break;
		}

		if (endOfGame == 6) newGame = true;
		else if (endOfGame == 7) newGame = false;
	}
	return 0;
}



/*
=========================================================

ALGORITHMS/HEURISTICS

=========================================================
*/



//-returns a score for a static state of the board
int staticEval(int board[][w_], int maximizer, int turn)
{
	//Checking for win detection will be taken care of in minimax()
	//A board will only be sent here if there is no win found in the board

	int minimizer;
	if (maximizer == cDisc) minimizer = pDisc;
	else minimizer = cDisc;

	int score = 0;
	int maxVTrapCount = 0, minVTrapCount = 0;
	int maxOneToWinCount = 0, minOneToWinCount = 0;

	maxOneToWinCount = oneToWinCount(board, maximizer);
	//printScreen(board);
	minOneToWinCount = oneToWinCount(board, minimizer);
	//printScreen(board);

	if (turn >= 10)
	{
		maxVTrapCount = vTrapCount(board, maximizer);
		//printScreen(board);
		minVTrapCount = vTrapCount(board, minimizer);
		//printScreen(board);
	}

	#ifdef _STATIC_EVAL_DEBUG
		//printBoard(board, debug);
		debug << "maxVTrapCount: " << maxVTrapCount << "\n";
		debug << "minVTrapCount: " << minVTrapCount << "\n";
		debug << "maxOneToWinCount: " << maxOneToWinCount << "\n";
		debug << "maxOneToWinCount: " << minOneToWinCount << "\n";
	#endif

	score += 50 * maxVTrapCount;
	score -= 50 * minVTrapCount;

	score += 10 * maxOneToWinCount;
	score -= 10 * minOneToWinCount;


	//Check horizontal wins
		if (hTrapDetect(board, maximizer))
		{
			//printScreen(board);
			score += 5000000;
		}

		if (hTrapDetect(board, minimizer))
		{
			//printScreen(board);
			score -= 2;
		}


	#ifdef _STATIC_EVAL_DEBUG
		debug << "\t\tscore: " << score << "\n";
		debug << "\texiting staticEval()\n\n";
	#endif
		//printScreen(board);
	return score;
}




//-implementation of minimax algorithm without alpha beta pruning
int minimax(int board[][w_], int maximizer, int minormax, int depth, int MAX_DEPTH, int turn)
{

	//"original_caller" keeps track of which player originally called minimax()
	//"minormax" keeps track of whether minimax() is currently evaulating a min or a max node (1 = min, 2 = max)
	
	int score[w_]; //creating clean score[] array
	for (int i = 0; i < w_; i++)
		score[i] = 0;

	int R, C;

	for (C = 0; C < w_; C++)
	{

		R = playMove(board, C, minormax);
		//printScreen(board);

			#ifdef _BOARD_DEBUG
				printScreen(board);
			#endif

		if (R != -1)
		{
			if (turn >= 8 && winDetect(board, R, C, minormax)) //checks for a win for original_caller
			{

				if (minormax == maximizer)
					score[C] = 1000;

				else
					score[C] = -1000;
			}

			else if (depth == MAX_DEPTH)
				score[C] = staticEval(board, maximizer, turn);

			else
			{
				if(minormax == cDisc)
					score[C] = minimax(board, maximizer, pDisc, depth + 1, MAX_DEPTH, turn + 1);
				else
					score[C] = minimax(board, maximizer, cDisc, depth + 1, MAX_DEPTH, turn + 1);
			}

			unPlayMove(board, C);
			//printScreen(board);
				
				#ifdef _BOARD_DEBUG
					printScreen(board);
				#endif
		}

		else
			score[C] = INT_MIN; //INT_MIN will be the value for an invalid cell
	}

	int low, high;
	//printScreen(board);

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

	if (depth == 1)
	{
		if (minormax == maximizer)
		{
			return high;
		}
		else
		{
			return low;
		}
	}
	
	else
	{
		if (minormax == maximizer)
		{
			return score[high];
		}
		else
		{
			return score[low];
		}
	}
}



//-implementation of minimax algorithm with alpha beta pruning
int ab_minimax(int board[][w_], int maximizer, int minormax, int depth, int MAX_DEPTH, int turn, int alpha, int beta, int &cutCount)
{
	//"maximizer" keeps track of which player originally called minimax()
	//"minormax" keeps track of whether minimax() is currently evaulating a min or a max node (1 = min, 2 = max)

	int score[w_]; //creating clean score[] array
	for (int i = 0; i < w_; i++)
		score[i] = INT_MIN; //initializing all indices to invalid

	int R, C = w_ / 2, cut;
	int checkNum = 1; //keeps track of how many columns have been checked

	while (checkNum <= w_)
	{
		if (alpha >= beta)
		{
			cut = (w_ - checkNum + 1)*pow(7, MAX_DEPTH - depth);
			cutCount += cut;
			break;
		}

		if (minormax == 2)
		{
			if (maximizer == cDisc)	R = playMove(board, C, cDisc);
			else R = playMove(board, C, pDisc);
		}
		else
		{
			if (maximizer == cDisc) R = playMove(board, C, pDisc);
			else R = playMove(board, C, cDisc);
		}

		if (R != -1)
		{
			if (turn >= 7 && winDetect(board, R, C, minormax))
			{
				if (minormax == maximizer)
					score[C] = 10000;

				else
					score[C] = -10000;
			}

			else if (depth == MAX_DEPTH)
			{
				score[C] = staticEval(board, maximizer, turn);

				#ifdef _AB_DEBUG
								debug << "Called staticEval()\n";
				#endif
			}

			else
			{
				if (minormax == 1)
					score[C] = ab_minimax(board, maximizer, 2, depth + 1, MAX_DEPTH, turn, alpha, beta, cutCount);
				else
					score[C] = ab_minimax(board, maximizer, 1, depth + 1, MAX_DEPTH, turn, alpha, beta, cutCount);
			}

			//Checking to reassign alpha and beta
			if (score[C] != INT_MIN)
			{
				if (minormax == 2 && score[C] > alpha)
					alpha = score[C]; //reassigning alpha

				else if (minormax == 1 && score[C] < beta)
					beta = score[C]; //reassigning beta
			}
			unPlayMove(board, C);
		}

		if (checkNum % 2 == 1) C -= checkNum;
		else C += checkNum;
		checkNum++;
	}



	//initializing low and high to a valid cell
		int low, high;
		checkNum = 1;
		C = w_ / 2;
		while (checkNum <= w_)
		{
			if (score[C] != INT_MIN)
			{
				low = C;
				high = C;
				break;
			}

			else if (C == w_ - 1)
				return INT_MIN; //if all cells are invalid

			if (checkNum % 2 == 1) C -= checkNum;
			else C += checkNum;
			checkNum++;
		}



	//finding the highest and lowest cells
		checkNum = 1;
		C = w_ / 2;
		while (checkNum <= w_)
		{
			if (score[C] != INT_MIN)
			{
				if (score[C] > score[high])
					high = C;
				else if (score[C] < score[low])
					low = C;
			}

			if (checkNum % 2 == 1) C -= checkNum;
			else C += checkNum;
			checkNum++;
		}


	//returning values to parent node
		if (depth == 1)
		{
			if (minormax == maximizer)
			{
				return high;
			}
			else
			{
				return low;
			}
			}

		else
		{
			if (minormax == maximizer)
			{
				#ifdef _AB_DEBUG
							debug << "Returning score of " << score[high] << "\n";
				#endif
				return score[high];
			}
			else
			{
				#ifdef _AB_DEBUG
							debug << "Returning score of " << score[low] << "\n";
				#endif
				return score[low];
			}
		}
	}




//-detects wins for "who" given last cell played into
//-returns true if win found, false if not
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



//-counts the number of places that "who" could win immediately
int nearWinCount(int board[][w_], int who)
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
				if(count == 2) break;
			}

			else
			{
				unPlayMove(board, i);
			}
		}
	}

	return count;
}



//-returns the column in which "who" could play to win immediately (or -1 if no such column)
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
				return i;
			}

			else unPlayMove(board, i);
		}
	}

	return -1;
}



//-returns the number of verticle traps on the board for who
int vTrapCount(int board[][w_], int who)
{
	int prev, trapCount = 0, notWho;

	if (who == cDisc) notWho = pDisc;
	else notWho = cDisc;

	for (int c = 0; c < w_; c++) //nested for loops cycle through all possible vTrap locations
	{
		for (int r = 1; r < h_; r++)
		{
			if (board[r][c] == nDisc) //only checks for traps at all if the initial cell to be checked is empty (implies cell above is empty too)
			{
				board[r][c] = who; //set fist cell at which we will check for wins to "who"
				prev = setCell(board, r + 1, c, nDisc); //avoids vertical win detection

				if (winDetect(board, r, c, who)) //if "who" has a win at [r][c]
				{
					board[r][c] = notWho; //about to check if not who also has a win at [r][c] (would negate any verticle trap)
					if (!winDetect(board, r, c, notWho))
					{
						board[r - 1][c] = who; //set second cell at which we will check for wins to "who"
						if (winDetect(board, r - 1, c, who)) // if the second cell we checked had a win for "who"
							trapCount++; // there is a verticle trap

						board[r - 1][c] = nDisc; //clearing the second cell we checked
					}
				}

				board[r + 1][c] = prev; //replacing the cell we cleared to avoid verticle win detection
				board[r][c] = nDisc; //clearing the first cell we checked
			}
			else
				break;
		}
	}
	return trapCount;
}



//-returns the column of first verticle trap found (or -1 if none found)
int vTrapDetect(int board[][w_], int who)
{
	int prev, notWho;

	if (who == cDisc) notWho = pDisc;
	else notWho = cDisc;

	for (int c = 0; c < w_; c++) //nested for loops cycle through all possible vTrap locations
	{
		for (int r = 1; r < h_; r++)
		{
			if (board[r][c] == nDisc) //only checks for traps at all if the initial cell to be checked is empty (implies cell above is empty too)
			{
				board[r][c] = who; //set fist cell at which we will check for wins to "who"
				prev = setCell(board, r + 1, c, nDisc); //avoids vertical win detection

				if (winDetect(board, r, c, who)) //if "who" has a win at [r][c]
				{
					board[r][c] = notWho; //about to check if not who also has a win at [r][c] (would negate any verticle trap)
					if (!winDetect(board, r, c, notWho))
					{
						board[r - 1][c] = who; //set second cell at which we will check for wins to "who"
						if (winDetect(board, r - 1, c, who)) // if the second cell we checked had a win for "who"
						{
							board[r][c] = nDisc; //resetting [r][c] to clear before returning true
							board[r - 1][c] = nDisc; //resetting [r-1][c] to clear before returning true
							return c;
						}

						board[r - 1][c] = nDisc; //clearing the second cell we checked
					}
				}

				board[r + 1][c] = prev; //replacing the cell we cleared to avoid verticle win detection
				board[r][c] = nDisc; //clearing the first cell we checked
			}
			else
				break;
		}
	}
	return -1;
}



//-detects if there is a horizontal trap for "who" on the board
//-returns true if trap found, false if not
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



//-returns # of cells that "who" could play into to win
//-includes cells not immediately accessible
int oneToWinCount(int board[][w_], int who)
{
	int count = 0;

	for (int c = 0; c < w_; c++)
	{
		for (int r = 0; r < h_; r++)
		{
			if (board[r][c] == nDisc)
			{
				setCell(board, r, c, who);
				if (winDetect(board, r, c, who))
					count++;
				setCell(board, r, c, nDisc);
			}
			else break;
		}
	}
	return count;
}





/*
=========================================================

BOARD MANIPULATION

=========================================================
*/


//-plays a disc for "who" in the given column if possible (column not full)
//-if given column is full, returns -1
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


//-removes the last disc played in the given column
//-returns true if operation successful, false otherwise (column already empty)
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



//-main controller function for computer's turn
void compTurn(int board[][w_], int &rowPlayed, int &colPlayed, int &turn, int MAX_DEPTH, int &cutCount)
{
	bool use_AB = false;
	#ifdef _USE_AB
		use_AB = true;
	#endif

	cout << "COMPUTER'S TURN\n\nthinking ... ";

	int nearWin = nearWinDetect(board, cDisc);
	if (nearWin != -1)
	{
		colPlayed = nearWin;
		rowPlayed = playMove(board, colPlayed, cDisc);
		return;
	}

	else
	{
		nearWin = nearWinDetect(board, pDisc);
		if (nearWin != -1)
		{
			colPlayed = nearWin;
			rowPlayed = playMove(board, colPlayed, cDisc);
			return;
		}
	}

	nearWin = vTrapDetect(board, cDisc);
	if (nearWin != -1)
	{
		colPlayed = nearWin;
		rowPlayed = playMove(board, colPlayed, cDisc);
	}


	if(LONG_THOUGHT)
		Sleep(turn * 200);

	if(use_AB)
		colPlayed = ab_minimax(board, cDisc, 2, 1, MAX_DEPTH, turn, INT_MIN, INT_MAX, cutCount); //starting alpha/beta out arbitrarily small/large
	else
		colPlayed = minimax(board, cDisc, 2, 1, MAX_DEPTH, turn);
	

	#ifdef _AB_DEBUG
		debug << "Total Cut Count: " << cutCount << "\n";
	#endif
	

	rowPlayed = playMove(board, colPlayed, cDisc);
}



//-main controller function for playerTurn
void playerTurn(int board[][w_], int &rowPlayed, int &colPlayed)
{
	//takes care of most of a human player's turn

	bool isValid;

	isValid = false;
	cout << "Your move! (enter column number): ";
	cin >> colPlayed;

	while (!isValid)
	{
		if (colPlayed > w_ || colPlayed < 1)
		{
			cout << "\nThat is not a valid column\n";
			
			cout << "Please choose a column between 1 and " << w_ << "\n\n";

			Sleep(1000);

			cout << "Your move! (enter column number): ";
			cin.clear();
			cin.ignore();
			cin >> colPlayed;
		}
		else
			break;
	}

	while (!isValid)
	{
		colPlayed--;

		rowPlayed = playMove(board, colPlayed, pDisc);

		if (rowPlayed == -1)
		{
			cout << "\nThere is no space in that column, choose a different one\n\n";

			Sleep(1000);

			cout << "Your move! (enter column number): ";
			cin.clear();
			cin.ignore();
			cin >> colPlayed;
		}

		else
			break;
	}
}



//-initializes all cells in the board to nDisc
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



//-sets the cell board[r][c] to who with no logical checks
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



//-prints the screen with pretty formatting
void printScreen(int board[][w_])
{
	system("cls");
	//cout << "CONNECT 4!\n";
	printPlayerColors();
	printBoard(board);
	//cout << "MAX_DEPTH: " << MAX_DEPTH;
	cout << "\n";
}



//-prints only the game board
void printBoard(int board[][w_])
{
	for (int i = 1; i <= w_; i++)
	{
		cout << "    ";
		printColor(i, GAME_COLOR);
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
				printColor(getChar(board, r, c), PLAYER_COLOR);
			else if (board[r][c] == cDisc)
				printColor(getChar(board, r, c), COMP_COLOR);

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



//prints the gameboard to the given ofstream
void printBoard(int board[][w_], ofstream &output) //overloaded for debuging
{
	for (int i = 1; i <= w_; i++)
	{
		output << "    ";
		printColor(i, GREEN, output);
		output << "\t";
	}

	output << "\n+";

	//First row of ----+----
	for (int i = 0; i < w_; ++i)
		output << "-------+";

	output << "\n";


	//rows of cells with discs in them	
	for (int r = 0; r < h_; r++)
	{
		//First Row of | with no discs
		output << "|   ";

		for (int i = 0; i < w_; i++)
			output << "    |   ";

		//Row with discs

		output << "\n|   ";

		for (int c = 0; c < w_; c++)
		{
			//decide which disc to print
			if (board[r][c] == nDisc)
				output << getChar(board, r, c);
			else if (board[r][c] == pDisc)
				printColor(getChar(board, r, c), LIGHTRED, output);
			else if (board[r][c] == cDisc)
				printColor(getChar(board, r, c), LIGHTBLUE, output);

			output << "   |   ";
		}

		//Second Row of | with no discs
		output << "\n|   ";

		for (int i = 0; i < w_; i++)
			output << "    |   ";

		//Bottom of each cell ----+----
		output << "\n+";

		for (int i = 0; i < w_; ++i)
			output << "-------+";

		output << "\n";
	}
}



//-prints the words "PLAYER" and "COMPUTER" in their respective colors
void printPlayerColors()
{
	cout << "\n\t ";
	printColor("PLAYER", PLAYER_COLOR);
	for (int i = 0; i < w_ - 3; i++)
		cout << "\t";
	printColor("COMPUTER", COMP_COLOR);
	cout << "\n\n";
}



//-prints text in color (multiple overloads)
void printColor(string str, int color, ofstream &output) //overloaded for debuging
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, color);
	output << str;
	SetConsoleTextAttribute(H, WHITE);
}



//-prints text in color (multiple overloads)
void printColor(string str, int color)
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, color);
	cout << str;
	SetConsoleTextAttribute(H, WHITE);
}



//-prints text in color (multiple overloads)
void printColor(int i, int color, ofstream &output) //overloaded for debuging
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, color);
	output << i;
	SetConsoleTextAttribute(H, WHITE);
}



//-prints text in color (multiple overloads)
void printColor(int i, int color)
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, color);
	cout << i;
	SetConsoleTextAttribute(H, WHITE);
}



//-prints text in color (multiple overloads)
void printColor(char c, int color, ofstream &output) //overloaded for debuging
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, color);
	output << c;
	SetConsoleTextAttribute(H, WHITE);
}



//-prints text in color (multiple overloads)
void printColor(char c, int color)
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, color);
	cout << c;
	SetConsoleTextAttribute(H, WHITE);
}



/*
=========================================================

OTHER HELPER FUNCTIONS

=========================================================
*/



//-returns the character corresponding to the disc at board[r][c]
//-either 'X' 'O' or ' '
char getChar(int board[][w_], int r, int c)
{
	if (board[r][c] == nDisc)
		return ' ';

	else if (board[r][c] == pDisc || board[r][c] == cDisc)
		return 'O';

	else
		return 'X'; //something went wrong
}



//-returns true if cell is in bounds of the board, false otherwise
bool isValidCell(int board[][w_], int r, int c)
{
	if (r >= 0 && r < h_ && c >= 0 && c < w_)
		return true;
	else
		return false;
}



//-handles pregame stuff like deciding who goes first
int preGame()
{
	string first;

	printColor("CONNECT 4!", GAME_COLOR);

	cout << "\n\nWho goes first? (\"me\" or \"computer\"): ";
	cin >> first;

	while (true)
	{
		for (int i = 0; i < first.length(); i++)
			first.at(i) = tolower(first.at(i));

		if (first == "me") return 1;

		else if (first == "computer") return 2;

		else
		{
			cout << "Please enter either \"me\" or \"computer\": ";
			cin >> first;
		}
	}
}