#include "brain.h"
#include "board.h"

using namespace std;

Brain::Brain(int max, int w, int h)
{
	MAX_ITER = max;

	score = new int[w];

	for (int i = 0; i < w; i++)
		score[i] = 0;

	score_ranked = new int[w];

	for (int i = 0; i < w; i++)
		score_ranked[i] = 0;

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
			if (board.playMove_NB(i, currentCheck))
			{
				if (winDetect_NB(board, board.getDisc('c')))
				{
					if (iter == 0)
					{
						score[i] += 1000;
					}
					else
						score[i] += (MAX_ITER - iter);
				}


				else if (winDetect_NB(board, board.getDisc('p')))
				{
					if (iter == 1)
					{
						score[i] += 500;
					}
					else
						score[i] -= (MAX_ITER - iter);
				}


				else if (!winDetect_NB(board, board.getDisc('c')) && !winDetect_NB(board, board.getDisc('p')))
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

	board.NB_delete();
}



bool Brain::winDetect(Board board, int who)
{
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
							return true;
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
							return true;
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
							return true;
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
							return true;
					}
				}
			}
		}
	}

	return false;
}



bool Brain::winDetect_NB(Board board, int who)
{
	//detecting horizontal wins
	for (int r = h_ - 1; r >= 0; r--)
	{
		for (int c = 0; c < w_ - 3; c++)
		{
			if (board.getCell_NB(r, c) == who)
			{
				if (board.getCell_NB(r, c + 1) == who)
				{
					if (board.getCell_NB(r, c + 2) == who)
					{
						if (board.getCell_NB(r, c + 3) == who)
							return true;
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
			if (board.getCell_NB(r, c) == who)
			{
				if (board.getCell_NB(r - 1, c) == who)
				{
					if (board.getCell_NB(r - 2, c) == who)
					{
						if (board.getCell_NB(r - 3, c) == who)
							return true;
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
			if (board.getCell_NB(r, c) == who)
			{
				if (board.getCell_NB(r - 1, c + 1) == who)
				{
					if (board.getCell_NB(r - 2, c + 2) == who)
					{
						if (board.getCell_NB(r - 3, c + 3) == who)
							return true;
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
			if (board.getCell_NB(r, c) == who)
			{
				if (board.getCell_NB(r + 1, c + 1) == who)
				{
					if (board.getCell_NB(r + 2, c + 2) == who)
					{
						if (board.getCell_NB(r + 3, c + 3) == who)
							return true;
					}
				}
			}
		}
	}

	return false;
}



int Brain::nearWinDetect(Board board, int who) //uses newBoard[][] (no need to be used with board[][])
{
	//detecting horizontal wins
	for (int r = h_ - 1; r >= 0; r--)
	{
		for (int c = 0; c < w_ - 2; c++)
		{
			if (board.getCell_NB(r, c) == who)
			{
				if (board.getCell_NB(r, c+1) == who)
				{
					if (board.getCell_NB(r, c + 2) == who)
					{
						//making sure the winning spot is available & possible
						if (board.getCell_NB(r, c + 3) == board.getDisc('n') && board.getCell_NB(r + 1, c + 3) != board.getDisc('n'))
							return c + 3;
						else if (board.getCell_NB(r, c - 1) == board.getDisc('n') && board.getCell_NB(r + 1, c - 1) != board.getDisc('n'))
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
			if (board.getCell_NB(r, c) == who)
			{
				if (board.getCell_NB(r-1, c) == who)
				{
					if (board.getCell_NB(r - 2, c) == who)
					{
						//making sure the winning spot is available & possible
						if (board.getCell_NB(r - 3, c) == board.getDisc('n'))
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
			if (board.getCell_NB(r, c) == who)
			{
				if (board.getCell_NB(r-1, c+1) == who)
				{
					if (board.getCell_NB(r - 2, c + 2) == who)
					{
						//making sure the winning spot is available & possible
						if (board.getCell_NB(r-3, c + 3) == board.getDisc('n') && board.getCell_NB(r - 2, c + 3) != board.getDisc('n'))
							return c + 3;
						else if (board.getCell_NB(r + 1, c - 1) == board.getDisc('n') && board.getCell_NB(r + 2, c - 1) != board.getDisc('n'))
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
			if (board.getCell_NB(r, c) == who)
			{
				if (board.getCell_NB(r+1, c+1) == who)
				{
					if (board.getCell_NB(r + 2, c + 2) == who)
					{
						//making sure the winning spot is available & possible
						if (board.getCell_NB(r - 1, c - 1) == board.getDisc('n') && board.getCell_NB(r, c - 1) != board.getDisc('n'))
							return c - 1;
						else if (board.getCell_NB(r +3 , c + 3) == board.getDisc('n') && board.getCell_NB(r + 4, c + 3) != board.getDisc('n'))
							return c + 3;
					}
				}
			}
		}
	}

	return -1;
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



void Brain::rankScores()
{
	for (int i = 1; i < w_; i++) //keeps track of which index of the score[] array it's checking
	{
		placed = false; //makes sure a column is only ranked once
		for (int n = 0; n < i; n++) //keeps track of which index of nextBest score[i] is being checked against
		{
			if (score[i] > score[score_ranked[n]])
			{
				for (int x = w_ - 1; x > n; x--) //shuffles up by one all indexes >= the one that needs to be changed
				{
					score_ranked[x] = score_ranked[x - 1];
				}

				score_ranked[n] = i;
				placed = true;
				break; //again makes sure a column is only ranked once
			}
		}

		if (!placed)
			score_ranked[i] = i;
	}
}

void Brain::rankScoreReset()
{
	for (int i = 0; i < w_; i++)
		score_ranked[i] = -1;
}



int Brain::getRankScore(int rank)
{
	return score_ranked[rank];
}