#ifndef MINIMAX_AI_H
#define MINIMAX_AI_H

#include "con4_util.h"


/*
---------------------------
ALGORITHMS/HEURISTICS
---------------------------
*/

//does a static evaluation of the board
int staticEval(int board[][w_], int maximizer, int turn);

//ultimate returns column of the best move for the computer
int minimax(int board[][w_], int maximizer, int minormax, int depth, int MAX_DEPTH, int turn);
int ab_minimax(int board[][w_], int maximizer, int minormax, int depth, int MAX_DEPTH, int turn, int alpha, int beta, int &cutCount);

//checks if "who" is one disc away from winning (returns column to block)
int nearWinDetect(int board[][w_], int who);

//counts how many places "who" could win in one move
int nearWinCount(int board[][w_], int who);

//checks if "who" has two places they could win stacked on top of eachother (returns column)
int vTrapDetect(int board[][w_], int who);

//checks if "who" has a horizontal trap set up (O X X X O) (returns right open column)
bool hTrapDetect(int board[][w_], int who);

//counts the # of places on the board (even if they're not available yet) where one disc could complete a string of 4
int oneToWinCount(int board[][w_], int who);

//umbrella function for taking a turn using the minimax algorithm
void minimaxTurn(int board[][w_], int &rowPlayed, int &colPlayed, int &turn, int MAX_DEPTH, int whosTurn);


using namespace std;

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
		maxVTrapCount = vTrapDetect(board, maximizer);
		//printScreen(board);
		minVTrapCount = vTrapDetect(board, minimizer);
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
				if (minormax == cDisc)
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



int ab_minimax(int board[][w_], int maximizer, int minormax, int depth, int MAX_DEPTH, int turn, int alpha, int beta)
{
	//"maximizer" keeps track of which player originally called minimax()
	//"minormax" keeps track of whether minimax() is currently evaulating a min or a max node (1 = max, 2 = min)

	int score[w_]; //creating clean score[] array
	for (int i = 0; i < w_; i++)
		score[i] = INT_MIN; //initializing all indices to invalid

	int R, C = w_/2;
	int checkNum = 1; //keeps track of how many columns have been checked

	int minimizer; //keeps track of who the minimizer is, set to opposite of maximizer
		if (maximizer == cDisc) minimizer = pDisc;
		else minimizer = cDisc;

	while (checkNum <= w_)
	{
		if (alpha >= beta)
			break;

		R = playMove(board, C, minormax);
		
		if (R != -1)
		{
			if (turn >= 8) //checks for a win for
			{
				if(minormax == 1 && winDetect(board, R, C, maximizer))
					score[C] = 1000;
				else if(minormax == 2 && winDetect(board, R, C, minimizer))
					score[C] = -1000;
			}

			else if (depth == MAX_DEPTH)
				score[C] = staticEval(board, maximizer, turn);

			else
			{
				if (minormax == 1)
					score[C] = ab_minimax(board, maximizer, 2, depth + 1, MAX_DEPTH, turn, alpha, beta);
				else
					score[C] = ab_minimax(board, maximizer, 1, depth + 1, MAX_DEPTH, turn, alpha, beta);
			}

			//Checking to reassign alpha and beta
			if (score[C] != INT_MIN)
			{
				if (minormax == 1 && score[C] > alpha)
					alpha = score[C]; //reassigning alpha

				else if (minormax == 2 && score[C] < beta)
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
				if (count == 2) break;
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

			else unPlayMove(board, i);
		}
	}

	return -1;
}



int vTrapDetect(int board[][w_], int who)
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



void minimaxTurn(int board[][w_], int &rowPlayed, int &colPlayed, int &turn, int MAX_DEPTH, int whosTurn)
{
	cout << "COMPUTER'S TURN\n\nthinking ... ";

	int notWhosTurn;
	if (whosTurn == pDisc) notWhosTurn = cDisc;
	else notWhosTurn = pDisc;

	int nearWin = nearWinDetect(board, whosTurn);
	if (nearWin != -1)
	{
		colPlayed = nearWin;
		rowPlayed = playMove(board, colPlayed, whosTurn);
		return;
	}

	else
	{
		nearWin = nearWinDetect(board, notWhosTurn);
		if (nearWin != -1)
		{
			colPlayed = nearWin;
			rowPlayed = playMove(board, colPlayed, notWhosTurn);
			return;
		}
	}

	//Sleep(turn * 200);

	colPlayed = ab_minimax(board, whosTurn, 1, 1, MAX_DEPTH, turn, INT_MIN, INT_MAX); //starting alpha/beta out arbitrarily small/large
	rowPlayed = playMove(board, colPlayed, whosTurn);
}

#endif