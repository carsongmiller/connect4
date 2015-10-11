#ifndef BRAIN_H
#define BRAIN_H

class Brain()
{
private:
	//Base number of maximum iterations (depending on the stage in the game, recursion may go more or less deep)
	int MAX_ITER = 7;
	int score[w_];
	int h_, w_;

public:
	//Constructors
	Brain(int max);
	
	//detects if "who" has won
	bool winDetect(int board[][w_], int who);

	//determines the best move for "who"
	void minimax(int board[][w_], int score[], int who, int currentCheck, int iter);

	//resets the score for each column to 0
	void scoreReset();

	//accessor functions
	int getScore(int col);

	friend void operator =(Board, Board)
}

#endif