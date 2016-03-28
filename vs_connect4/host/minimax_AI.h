#ifndef MINIMAX_AI_H
#define MINIMAX_AI_H

#include "con4_util.h"

//does a static evaluation of the board
int staticEval(int board[][w_], int maximizer, int turn);

//ultimately returns column of the best move for the computer
int ab_minimax(int board[][w_], int maximizer, int minormax, int depth, int MAX_DEPTH, int turn, int alpha, int beta, int &cutCount);

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

	score += 50 * maxVTrapCount;
	score -= 50 * minVTrapCount;

	score += 10 * maxOneToWinCount;
	score -= 10 * minOneToWinCount;


	//Check horizontal wins
	if (hTrapDetect(board, maximizer))
	{
		//printScreen(board);
		score += 5000;
	}

	int hTrap = hTrapDetect(board, minimizer);

	if (hTrap > 0)
	{
		//printScreen(board);
		score -= 5000;
	}

	//printScreen(board);
	return score;
}



int ab_minimax(int board[][w_], int maximizer, int minormax, int depth, int MAX_DEPTH, int turn, int alpha, int beta)
{
	//"maximizer" keeps track of which player originally called minimax()
	//"minormax" keeps track of whether the parent of the current node is a max or min node (1 = min, 2 = max)

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

		if (minormax == 2)
		{
			if (maximizer == cDisc)	R = playMove(board, C, cDisc);
			else R = playMove(board, C, pDisc);
		}
		else
		{
			if (minimizer == cDisc) R = playMove(board, C, cDisc);
			else R = playMove(board, C, pDisc);
		}
		//printScreen(board);
		
		if (R != -1)
		{
			if (turn >= 7)
			{
				if (minormax == 2 && winDetect(board, R, C, maximizer))
					score[C] = 10000;
				else if (minormax == 1 && winDetect(board, R, C, minimizer))
					score[C] = -10000;
			}

			if (depth == MAX_DEPTH && score[C] == INT_MIN) //if at target depth and the score for the current column hasn't already been set
				score[C] = staticEval(board, maximizer, turn);

			else if(depth < MAX_DEPTH)
			{
				if (minormax == 1)
					score[C] = ab_minimax(board, maximizer, 2, depth + 1, MAX_DEPTH, turn, alpha, beta);
				else
					score[C] = ab_minimax(board, maximizer, 1, depth + 1, MAX_DEPTH, turn, alpha, beta);
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
			//printScreen(board);
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
		if (minormax == 2)
			return high;
		else
			return low;
	}

	else
	{
		if (minormax == 2)
			return score[high];
		else
			return score[low];
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



int vTrapDetect(int board[][w_], int who) //returns the column of first verticle trap found (or -1 if none found)
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
							return c;

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
			rowPlayed = playMove(board, colPlayed, whosTurn);
			return;
		}
	}

	colPlayed = ab_minimax(board, whosTurn, 1, 1, MAX_DEPTH, turn, INT_MIN, INT_MAX); //starting alpha/beta out arbitrarily small/large
	rowPlayed = playMove(board, colPlayed, whosTurn);
}

#endif