#ifndef _BRAIN
#define _BRAIN

#include <string>
#include <iostream>
#include <windows.h>
#include <cstdlib>

class Board;

class Brain
{
private:
	//Base number of maximum iterations (depending on the stage in the game, recursion may go more or less deep)
	int MAX_ITER;
	int* score;
	int* score_ranked;
	int h_, w_;
	bool placed;

	//pointer to be used in minimax
	int** newBoard;

public:
	//Constructors
	Brain(int max, int w, int h);

	//detects if "who" has won in board[][]
	bool winDetect(Board board, int who);

	//detects if "who" has won in newBoard[][]
	bool winDetect_NB(Board board, int who);

	//detects if "who" can win in one more move and returns where to play to block in newBoard[][]
	int nearWinDetect(Board board, int who);

	//determines the best move for "who"
	void minimax(Board board, int who, int currentCheck, int iter);

	//resets the score for each column to 0
	void scoreReset();

	//accessor functions
	int getScore(int col);

	//ranks columns based on score
	void rankScores();

	//resets all indexes of rankScore() to -1
	void rankScoreReset();

	//returns the column at the given rank
	int getRankScore(int rank);
};

#endif