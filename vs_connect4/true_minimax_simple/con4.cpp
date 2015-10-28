//connect4
//Carson Miller
//10-7-2015


/*

TO DO:

-Add alpha beta pruning
-Program crashed when there was only one available column
	-exact cause of the crash unknown

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

#define _DEBUG

//#define _STATIC_EVAL_DEBUG
//#define _AB_DEBUG
//#define _BOARD_DEBUG

#define w_ 7
#define h_ 6

#define pDisc 1
#define cDisc 2
#define nDisc 0

using namespace std;

//int MAX_DEPTH;

#ifdef _DEBUG
	ofstream debug;
#endif


//Values for SetConsoleTextAttribute()
//HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15


/*
---------------------------
ALGORITHMS/HEURISTICS
---------------------------
*/

	//does a static evaluation of the board
	int staticEval(int board[][w_], int maximizer);

	//ultimate returns column of the best move for the computer
	int minimax(int board[][w_], int maximizer, int minormax, int depth, int MAX_DEPTH);
	int ab_minimax(int board[][w_], int maximizer, int minormax, int depth, int MAX_DEPTH, int alpha, int beta, int &cutCount);

	//checks if the disc at board[r][c] has won
	bool winDetect(int board[][w_], int r, int c, int who);

	//checks if "who" is one disc away from winning (returns column to block)
	int nearWinDetect(int board[][w_], int who);

	//counts how many places "who" could win in one move
	int nearWinCount(int board[][w_], int who);

	//checks if "who" has two places they could win stacked on top of eachother (returns column)
	bool vTrapDetect(int board[][w_], int who);

	//checks if "who" has a horizontal trap set up (O X X X O) (returns right open column)
	bool hTrapDetect(int board[][w_], int who);

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
	void playerTurn(int board[][w_], int &rowPlayed, int &colPlayed);

	void compTurn(int board[][w_], int &rowPlayed, int &colPlayed, int &turn, int MAX_DEPTH);

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

	//ignores any key presses that may have occured during computer's turn
	void ignoreInput();



int main()
{
	srand(time(0));

	#ifdef _DEBUG
		debug.open("debug.txt");
		if (debug.fail())
			cout << "Failed to open debug stream";
	#endif

	int board[h_][w_]; //the main board
	bool newGame = true;
	int endOfGame; //6 = yes, 7 = no
	int rowPlayed = 0; //stores the row in which the last disc was played
	int colPlayed = 0; //stores the column in which the last disc was played
	int whosTurn; //keeps track of who's turn it is (1 = player1/human, 2 = player2/computer)
	int turn; //keeps track of how mant discs have been played
	int MAX_DEPTH = 6;
	int winner = 0;

	while (newGame)
	{
		boardInit(board);
		system("cls");
		whosTurn = preGame();
		printScreen(board);

		turn = 0;

		while (turn < w_*h_)
		{
			if (MAX_DEPTH > w_*h_ - turn) MAX_DEPTH = w_*h_ - turn;

			if (whosTurn == 1) //player's turn
			{

				playerTurn(board, rowPlayed, colPlayed);
				printScreen(board);

				//cout << "MAX_DEPTH: " << MAX_DEPTH << endl;

				if (winDetect(board, rowPlayed, colPlayed, pDisc))
				{
					winner = 1;
					break;
				}
			}

			else if (whosTurn == 2) //computer's turn
			{
				compTurn(board, rowPlayed, colPlayed, turn, MAX_DEPTH);
				printScreen(board);
				cout << "The computer played in column ";
				printColor(colPlayed+1, GREEN);
				cout << "\n\n";

				//cout << "MAX_DEPTH: " << MAX_DEPTH << endl;

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
		}

		//Displaying an end game message box
			if (winner == 0)
				endOfGame = MessageBox(NULL, TEXT("Cat's Game!\n\nPlay again?"),
					TEXT("End of Game"), MB_YESNO);
		
			else if(winner == 1)
				endOfGame = MessageBox(NULL, TEXT("You Win!\n\nPlay again?"),
					TEXT("End of Game"), MB_YESNO);
		
			else if(winner == 2)
				endOfGame = MessageBox(NULL, TEXT("The Computer Wins!\n\nPlay again?"),
					TEXT("End of Game"), MB_YESNO);

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



int staticEval(int board[][w_], int maximizer)
{
	//Checking for win detection will be taken care of in minimax()
	//A board will only be sent here if there is no win found in the board

	int minimizer;
	if (maximizer == cDisc) minimizer = pDisc;
	else minimizer = cDisc;

	int score = 0;
	int maxNearWinCount, minNearWinCount, maxVTrapCount, minVTrapCount;

	maxNearWinCount = nearWinCount(board, maximizer);
	//printScreen(board);
	minNearWinCount = nearWinCount(board, minimizer);
	//printScreen(board);
	maxVTrapCount = vTrapDetect(board, maximizer);
	//printScreen(board);
	minVTrapCount = vTrapDetect(board, minimizer);
	//printScreen(board);

#ifdef _STATIC_EVAL_DEBUG
	printBoard(board, debug);
	debug << "maxNearWinCount: " << maxNearWinCount << "\n";
	debug << "minNearWinCount: " << minNearWinCount << "\n";
	debug << "maxVTrapCount: " << maxVTrapCount << "\n";
	debug << "minVTrapCount: " << minVTrapCount << "\n";
#endif


	score += 5 * maxNearWinCount;
	score -= 5 * minNearWinCount;

	score += 10 * maxVTrapCount;
	score -= 10 * minVTrapCount;


//Check horizontal wins
#ifdef _STATIC_EVAL_DEBUG
	debug << "\t\tmaximizer hTrap: ";
#endif

	if (hTrapDetect(board, maximizer))
	{
		//printScreen(board);
		#ifdef _STATIC_EVAL_DEBUG
			debug << "TRUE\n";
		#endif
		score += 2;
	}
	else
	{
		//printScreen(board);
		#ifdef _STATIC_EVAL_DEBUG
				debug << "FALSE\n";
		#endif
	}

	#ifdef _STATIC_EVAL_DEBUG
		debug << "\t\tminimizer hTrap: ";
	#endif

	if (hTrapDetect(board, minimizer))
	{
		//printScreen(board);
		#ifdef _STATIC_EVAL_DEBUG
				debug << "\t\tTRUE\n";
		#endif
		score -= 2;
	}
	else
	{
		//printScreen(board);
		#ifdef _STATIC_EVAL_DEBUG
			debug << "FALSE\n";
		#endif
	}


	#ifdef _STATIC_EVAL_DEBUG
		debug << "\t\tscore: " << score << "\n";
		debug << "\texiting staticEval()\n\n";
	#endif
		//printScreen(board);
	return score;
}







int minimax(int board[][w_], int maximizer, int minormax, int depth, int MAX_DEPTH)
{

	//"original_caller" keeps track of which player originally called minimax()
	//"minormax" keeps track of whether minimax() is currently evaulating a min or a max node (1 = min, 2 = max)


	//int newBoard[h_][w_]; //creating a copy of the board
	//copyBoard(board, newBoard);
	
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
			if (winDetect(board, R, C, minormax)) //checks for a win for original_caller
			{

				if (minormax == maximizer)
					score[C] = 1000000;

				else
					score[C] = -1000000;
			}

			else if (depth == MAX_DEPTH)
				score[C] = staticEval(board, maximizer);

			else
			{
				if(minormax == cDisc)
					score[C] = minimax(board, maximizer, pDisc, depth + 1, MAX_DEPTH);
				else
					score[C] = minimax(board, maximizer, cDisc, depth + 1, MAX_DEPTH);
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
			else if (score[i] == score[low] && rand() % 2 == 0) //if two columns have the same score, randomize
				low = i;
			else if (score[i] == score[high] && rand() % 2 == 0) //if two columns have the same score, randomize
				high = 1;
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







int ab_minimax(int board[][w_], int maximizer, int minormax, int depth, int MAX_DEPTH, int alpha, int beta, int &cutCount)
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
			if (alpha > beta)
			{				
				#ifdef _AB_DEBUG
					int cut = (w_ - C)*pow(7, MAX_DEPTH - depth);

					debug << "Cut Branches: " << cut << "\n";
					cutCount += cut;
				#endif
				return INT_MIN;
			}

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
				if (minormax == cDisc)
				{
					score[C] = ab_minimax(newBoard, maximizer, pDisc, depth + 1, MAX_DEPTH, alpha, beta, cutCount);
				}
				else
				{
					score[C] = ab_minimax(newBoard, maximizer, cDisc, depth + 1, MAX_DEPTH, alpha, beta, cutCount);
				}
			}

			//Checking to reassign alpha and beta
				if (minormax == cDisc && score[C] > alpha)
				{
					alpha = score[C]; //reassigning alpha
				}

				else if (minormax == pDisc && score[C] < beta)
				{
					beta = score[C]; //reassigning beta
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
			}

			else
			{
				unPlayMove(board, i);
			}
		}
	}

	return count;
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
				return i;
			}

			else
			{
				unPlayMove(board, i);
			}
		}
	}

	return -1;
}





bool vTrapDetect(int board[][w_], int who)
{
	int rowPlayed1, rowPlayed2, prev1, prev2, trapCount = 0, notWho;

	if (who == cDisc) notWho = pDisc;
	else notWho = cDisc;

	for (int i = 0; i < w_; i++)
	{
		rowPlayed1 = playMove(board, i, who); //play first move
		prev1 = setCell(board, rowPlayed1 + 1, i, nDisc); //avoids vertical win detection ("cleared cell 1")

		if (rowPlayed1 != -1 && winDetect(board, rowPlayed1, i, who))
		{
			setCell(board, rowPlayed1 + 1, i, prev1); //replacing "cleared cell 1"
			rowPlayed2 = playMove(board, i, who); //play second move
			prev2 = setCell(board, rowPlayed2 + 1, i, nDisc); //avoids vertical win detection ("cleared cell 2")

			if (rowPlayed2 != -1 && winDetect(board, rowPlayed2, i, who))
			{
				setCell(board, rowPlayed2 + 1, i, prev2); //replacing "cleared cell 2"
				trapCount++;
			}

			setCell(board, rowPlayed2 + 1, i, prev2); //replacing "cleared cell 2"
			if (rowPlayed2 != -1)
			{
				unPlayMove(board, i);
			}
		}

		setCell(board, rowPlayed1 + 1, i, prev1); //replacing "cleared cell 1"

		if (rowPlayed1 != -1)
		{
			unPlayMove(board, i);
		}

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



void compTurn(int board[][w_], int &rowPlayed, int &colPlayed, int &turn, int MAX_DEPTH)
{
	cout << "COMPUTER'S TURN\n\nthinking ... ";
	if (turn <= 1)
	{
		colPlayed = w_ / 2;
		rowPlayed = playMove(board, colPlayed, cDisc);
		return;
	}

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

	int cutCount = 0;

	colPlayed = minimax(board, cDisc, cDisc, 1, MAX_DEPTH);
	//colPlayed = ab_minimax(board, cDisc, cDisc, 1, MAX_DEPTH, -1000001, 1000001, cutCount); //starting alpha and beta out arbitrarily large

	#ifdef _AB_DEBUG
		debug << "Total Cut Count: " << cutCount << "\n";
	#endif
	

	rowPlayed = playMove(board, colPlayed, cDisc);
}





void playerTurn(int board[][w_], int &rowPlayed, int &colPlayed)
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
			cout << "\nThat is not a valid column\n";
			
			cout << "Please choose a column between 1 and " << w_ << "\n\n";

			Sleep(1000);

			cout << "Where would you like to go? (enter column number): ";
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

			cout << "Where would you like to go? (enter column number): ";
			cin.clear();
			cin.ignore();
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



void printPlayerColors()
{
	cout << "Player: ";
	printColor('O', LIGHTRED);
	cout << endl;
	cout << "Computer: ";
	printColor('O', LIGHTBLUE);
	cout << endl;
}



void printColor(string str, int color, ofstream &output) //overloaded for debuging
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, color);
	output << str;
	SetConsoleTextAttribute(H, WHITE);
}

void printColor(string str, int color)
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, color);
	cout << str;
	SetConsoleTextAttribute(H, WHITE);
}

void printColor(int i, int color, ofstream &output) //overloaded for debuging
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, color);
	output << i;
	SetConsoleTextAttribute(H, WHITE);
}

void printColor(int i, int color)
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, color);
	cout << i;
	SetConsoleTextAttribute(H, WHITE);
}

void printColor(char c, int color, ofstream &output) //overloaded for debuging
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, color);
	output << c;
	SetConsoleTextAttribute(H, WHITE);
}

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



char getChar(int board[][w_], int r, int c)
{
	if (board[r][c] == nDisc)
		return ' ';

	else if (board[r][c] == pDisc || board[r][c] == cDisc)
		return 'O';

	else
		return 'X'; //something went wrong
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

	cout << "CONNECT 4!\n\nWho would you like to go first? (m for me, c for computer): ";
	cin >> first;

	while (true)
	{
		if (first == 'm' || first == 'M') return 1;

		else if (first == 'c' || first == 'C') return 2;

		else
		{
			cout << "Please enter either m or c: ";
			cin >> first;
		}
	}
}


void ignoreInput()
{
	
}