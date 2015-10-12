#include "brain.h"
#include "board.h"

using namespace std;

Brain::Brain(int max, int w, int h)
{
	MAX_ITER = max;

	score = new int[w];

	for (int i = 0; i < w; i++)
		score[i] = 0;

	w_ = w;
	h_ = h;
}



void Brain::minimax(Board board, int who, int currentCheck, int iter)
{	
	board.createNewBoard();

	if (iter <= MAX_ITER)
	{
		for (int i = 0; i < w_; i++)
		{
			if (board.playMove(i, currentCheck))
			{
				if (winDetect(board, board.getDisc('c')))
				{
					if (iter == 0)
					{
						score[i] += 1000;
					}
					else
						score[i] += (MAX_ITER - iter);
				}


				else if (winDetect(board, board.getDisc('p')))
				{
					if (iter == 1)
					{
						score[i] += 500;
					}
					else
						score[i] -= (MAX_ITER - iter);
				}


				else if (!winDetect(board, board.getDisc('c')) && !winDetect(board, board.getDisc('p')))
				{
					if (currentCheck == board.getDisc('p'))
						currentCheck = board.getDisc('c');
					else if (currentCheck == board.getDisc('c'))
						currentCheck = board.getDisc('p');

					minimax(board, who, currentCheck, iter + 1);
				}
			}
		}
	}
}



bool Brain::winDetect(Board board, int who)
{
	bool win = false;

	//detecting horizontal wins
	for (int r = h_ - 1; r >= 0; r--)
	{
		for (int c = 0; c < w_ - 3; c++)
		{
			if (board.getCell(r, c) == who)
			{
				if (board.getCell(r, c+1) == who)
				{
					if (board.getCell(r, c+2) == who)
					{
						if (board.getCell(r, c+3) == who)
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
			if (board.getCell(r, c) == who)
			{
				if(board.getCell(r-1, c) == who)
				{
					if (board.getCell(r-2, c) == who)
					{
						if (board.getCell(r-3, c) == who)
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
			if (board.getCell(r, c) == who)
			{
				if (board.getCell(r-1, c+1) == who)
				{
					if (board.getCell(r-2, c+2) == who)
					{
						if (board.getCell(r-3, c+3) == who)
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
			if (board.getCell(r, c) == who)
			{
				if (board.getCell(r+1, c+1) == who)
				{
					if (board.getCell(r+2, c+2) == who)
					{
						if (board.getCell(r+3, c+3) == who)
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
	for (int i = 0; i < w_; i++)
		score[i] = 0;
}



int Brain::getScore(int col)
{
	return score[col];
}