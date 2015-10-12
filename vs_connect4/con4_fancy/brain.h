#define NOMINMAX

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
	int h_, w_;

public:
	//Constructors
	Brain(int max, int w, int h);

	//detects if "who" has won
	bool winDetect(Board board, int who);

	//determines the best move for "who"
	void minimax(Board board, int who, int currentCheck, int iter);

	//resets the score for each column to 0
	void scoreReset();

	//accessor functions
	int getScore(int col);
};

#endif