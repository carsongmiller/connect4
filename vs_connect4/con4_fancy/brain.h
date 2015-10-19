#ifndef _BRAIN
#define _BRAIN

#include <iostream>
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <cmath>

using namespace std;

class Board;

class Brain
{
private:
	long int *score; //stores minimax scores
	long int *rankedScore; //stores minimax scores (ranked best to worst)
	int MAX_DEPTH;
	int w_, h_; //width and height of board


public:
	//CONSTRUCTORS
	Brain(int h, int w, int max);

	//checks if the disc at board[r][c] has won
	bool winDetect(Board &board, int r, int c, int who);

	//checks if "who" is one disc away from winning
	int nearWinDetect(Board &board, int who);

	//checks if "who" has two places they could win stacked on top of eachother (returns column)
	bool vTrapDetect(Board &board, int who);

	//checks if "who" has a horizontal trap set up (O X X X O) (returns right open column)
	bool hTrapDetect(Board &board, int who);

	//determines the best move for "who"
	void minimax(Board board, int &tempScore, int who, int currentCheck, int iter, int turn);

	//ranks the scores of columns
	void rankScores();

	//resets the score[] array
	void scoreReset();

	//resets the rankedScore[] array
	void rankedScoreReset();

	//initializes score()
	void scoreInit();

	//initializes rankedScore()
	void rankedScoreInit();

	//prints the score[] array (mostly for debugging)
	void printScore();



	//ACCESSOR FUNCTIONS

	//returns an entry of score[]
	int getScore(int c);

	//returns an entry of rankedScore[]
	int getRankedScore(int c);
};

#endif