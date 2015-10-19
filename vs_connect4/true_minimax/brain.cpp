#include "brain.h"
#include "board.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <cmath>

using namespace std;

Brain::Brain(int h, int w, int max)
{
	h_ = h;
	w_ = w;
	MAX_DEPTH = max;

	scoreInit();
	rankedScoreInit();
}



void Brain::minimax(Board board, int &tempScore, int who, int currentCheck, int iter, int turn)
{
	Board* newBoard = new Board;
	*newBoard = board;

	/*
	int newBoard[h_][w_];
	copyBoard(board, newBoard);
	*/

	//printBoard(newBoard); //debug

	int nearWinP, nearWinC, rowPlayed;

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
				nearWinC = nearWinDetect(*newBoard, board.getDisc('c')); //checks for immediate possibility to win
				if (nearWinC != -1)
				{
					score[nearWinC] += 1000000;
					break;
				}

				nearWinP = nearWinDetect(*newBoard, board.getDisc('p')); //checks for immediate need to block
				if (nearWinP != -1)
				{
					score[nearWinP] += 1000000;
					break;
				}
			}


			rowPlayed = newBoard->playMove(i, currentCheck, turn);

			if (rowPlayed != -1)
			{
				//printScreen(newBoard, score);
				if (winDetect(*newBoard, rowPlayed, i, currentCheck)) //checks for a win
				{
					if (currentCheck == board.getDisc('c'))
						tempScore += pow(4, 1.0*MAX_DEPTH - iter);
					else
						tempScore -= pow(3, 1.0*MAX_DEPTH - iter);
				}


				else //if no wins were found
				{
					if (turn >= 2 && turn < 20)
					{
						if (hTrapDetect(*newBoard, currentCheck))
						{
							//cout << hTrap << "\t" << i << "\t" << iter << endl;

							if (currentCheck == board.getDisc('c'))
								tempScore += pow(5, MAX_DEPTH - iter);
							else
								tempScore -= pow(4, MAX_DEPTH - iter);
						}

						if (turn >= 20)
						{
							if (vTrapDetect(board, currentCheck)) //checks for a vertical trap in favor of the computer
							{
								if (currentCheck == board.getDisc('c'))
								tempScore += pow(3, MAX_DEPTH - iter);
								else
								tempScore -= pow(3, MAX_DEPTH - iter);
							}
						}
					}

					if (currentCheck == board.getDisc('c'))
						minimax(*newBoard, tempScore, who, board.getDisc('p'), iter + 1, turn);
					else
						minimax(*newBoard, tempScore, who, board.getDisc('c'), iter + 1, turn);
				}
			}

			newBoard->unPlayMove(i, turn);

			if (iter == 0)
			{
				score[i] = tempScore;
				tempScore = 0;
			}

		}
	}

	delete newBoard;
}



bool Brain::winDetect(Board &board, int r, int c, int who)
{
	int consec;

	//checking for horizontal
	consec = 0;
	for (int i = c - 3; i <= c + 3; i++)
	{
		if (board.isValidCell(r, i) && board.getCell(r, i) == who)
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
		if (board.isValidCell(i, c) && board.getCell(i, c) == who)
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
		if (board.isValidCell(r + i, c + i) && board.getCell(r+i, c+i) == who)
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
		if (board.isValidCell(r - i, c + i) && board.getCell(r-i, c+i) == who)
		{
			if (++consec >= 4)
				return true;
		}
		else
			consec = 0;
	}

	return false;
}




int Brain::nearWinDetect(Board &board, int who)
{
	int rowPlayed;
	int turn = 0;

	for (int i = 0; i < w_; i++)
	{
		rowPlayed = board.playMove(i, who, turn);
		if (rowPlayed != -1)
		{
			if (winDetect(board, rowPlayed, i, who))
			{
				board.unPlayMove(i, turn);
				return i;
			}

			else
				board.unPlayMove(i, turn);
		}
	}

	return -1;
}



bool Brain::vTrapDetect(Board &board, int who)
{
	int rowPlayed1, rowPlayed2;
	int prev;
	int turn = 0;

	for (int i = 0; i < w_; i++)
	{
		rowPlayed1 = board.playMove(i, who, turn); //play first move
		prev = board.setCell(rowPlayed1 - 1, i, board.getDisc('n')); //avoids vertical win detection (cleared cell 1)

		if (rowPlayed1 != -1 && winDetect(board, rowPlayed1, i, who))
		{
			board.setCell(rowPlayed1 - 1, i, prev); //replacing cleared cell 1
			rowPlayed2 = board.playMove(i, who, turn); //play second move
			prev = board.setCell(rowPlayed2 - 1, i, board.getDisc('n')); //avoids vertical win detection (cleared cell 2)

			if (rowPlayed2 != -1 && winDetect(board, rowPlayed2, i, who))
			{
				board.setCell(rowPlayed2 - 1, i, prev); //replacing cleared cell 2
				board.unPlayMove(i, turn);
				board.unPlayMove(i, turn);
				return true;
			}

			board.setCell(rowPlayed2 - 1, i, prev); //replacing cleared cell 2
			if (rowPlayed2 != -1)
				board.unPlayMove(i, turn);
		}

		board.setCell(rowPlayed1 - 1, i, prev); //replacing cleared cell 1
		if (rowPlayed1 != -1)
			board.unPlayMove(i, turn);
	}

	return false;
}



bool Brain::hTrapDetect(Board &board, int who)
{
	int count;
	int notWho;

	if (who == board.getDisc('p')) notWho = board.getDisc('c');
	else notWho = board.getDisc('p');

	for (int r = h_ - 1; r >= 0; r--)
	{
		count = 0;
		for (int c = 0; c < w_; c++)
		{
			if (count == 0 || count == 4)
			{
				if (board.getCell(r, c) == board.getDisc('n') && (board.getCell(r+1, c) == board.getDisc('c') || board.getCell(r+1, c) == board.getDisc('p') || !board.isValidCell(r + 1, c))) count++;
				else count = 0;
			}
			else if (count > 0 && count < 4)
			{
				if (board.getCell(r, c) == who) count++;
				else if (board.getCell(r, c) == notWho) count = 0;
			}
			else if (count == 5)
				return true;
		}
	}
	if (count < 5) return false;
	else return true;
}



void Brain::rankScores()
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



void Brain::printScore()
{
	for (int i = 0; i < w_; i++)
		cout << score[i] << "\t";
	cout << "\n";
}



void Brain::scoreReset()
{
	for (int i = 0; i < w_; i++)
		this->score[i] = 0;
}



void Brain::rankedScoreReset()
{
	for (int i = 0; i < w_; i++)
		this->rankedScore[i] = -1;
}



void Brain::scoreInit()
{
	score = new long int[w_];
	for (int i = 0; i < w_; i++)
		score[i] = 0;
}



void Brain::rankedScoreInit()
{
	rankedScore = new long int[w_];
	for (int i = 0; i < w_; i++)
		score[i] = -1;
}



int Brain::getScore(int c)
{
	return score[c];
}

int Brain::getRankedScore(int c)
{
	return rankedScore[c];
}