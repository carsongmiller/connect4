#include "brain.h"

Brain::Brain(int max)
{
	MAX_ITER = max;

	for (int i = 0; i < board.getW(); i++)
		score[i] = 0;

	w_ = board.getW();
	h_ = board.getH();
}



void Brain::minimax(Board board, int who, int currentCheck, int iter)
{
	Board newBoard = board;
	
	if (iter <= MAX_ITER)
	{
		for (int i = 0; i < w_; i++)
		{
			if (newBoard.playMove(i, currentCheck))
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



bool Brain::winDetect(int board[][w_], int who)
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



void Brain::scoreReset()
{
	for (int i = 0; i < board.getW(); i++)
		score[i] = 0;
}



int Brain::getScore(int col)
{
	return score[col];
}