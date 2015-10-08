//connect4
//Carson Miller
//10-7-2015

#include <iostream>
#include <windows.h>

using namespace std;

//pDisc =  disc; cDisc = computer disc; nDisc = no disc
const int pDisc = 1, cDisc = 2, nDisc = 0;
const int w_ = 7, h_ = 6;
const int MAX_ITER = 8;

//returns true if move is valid, returns false if not
bool playMove(int board[][w_], int col, int who);

//detects if "who" has won
bool winDetect(int board[][w_], int who);

//determines the best move for "who"
void minimax(int board[][w_], int score[], int who, int currentCheck, int iter);

//prints the board
void printBoard(int board[][w_]);

//copies the board
void copyBoard(int board[][w_], int newBoard[][w_]);
int main()
{
	int board[h_][w_]; //the main board
	bool cont = true, validMove, newGame = true; //determines whether the game should continue
	char whoFirst; //who will go first
	int moveChoice; //human player's move choice
	int score[w_]; //stores the scores of each column (handed to minimax())
	int bestScore; //stores the number of the column with the best score
	char yn; //takes input from player as to whether they want to play again

	while (newGame)
	{
		//initializing the board to all blank spaces
		for (int r = 0; r < h_; r++)
		{
			for (int c = 0; c < w_; c++)
			{
				board[r][c] = nDisc;
			}
		}


		//First output of the program:

		cout << "CONNECT 4!\n\nWho should go first? (m for me, c for computer): ";
		cin >> whoFirst;

		if (whoFirst == 'm' || whoFirst == 'M')
		{
			printBoard(board);

			validMove = false;
			while (!validMove)
			{
				cout << "Where would you like to go? (enter column number): ";
				cin >> moveChoice;

				if (!playMove(board, moveChoice-1, pDisc))
					cout << "There is no space in that column, choose a different one\n";
				else
					validMove = true;
			}
		}

		//MAIN GAME LOOP

		while (cont)
		{
			//reset the score of each column to 0 at the beginning of each turn
			for (int i = 0; i < w_; i++)
				score[i] = 0;

			cout << "Now its' the computer's turn\n";
			cout << "\nthinking ...\n\n";

			minimax(board, score, cDisc, cDisc, 0);

			//printing the score array for debugging
				for(int i = 0; i < w_; i++)
					cout << score[i] << "\t";

			cout << endl << endl;

			bestScore = 0;

			for (int i = 1; i < w_; i++)
			{
				if (score[i] > score[bestScore])
					bestScore = i;
			}

			//now the computer will make its move

			playMove(board, bestScore, cDisc);
			printBoard(board);

			if (winDetect(board, cDisc))
			{
				cout << "\nThe computer wins!\n";
				break;
			}

			//now the player's turn

			validMove = false;
			while (!validMove)
			{
				cout << "Where would you like to go? (enter column number): ";
				cin >> moveChoice;

				if (!playMove(board, moveChoice-1, pDisc))
					cout << "There is no space in that column, choose a different one\n";
				else
					validMove = true;
			}

		}

		cout << "Would you like to play again? (y/n): ";
		cin >> yn;
		if (yn != 'y' && yn != 'Y')
			newGame = false;
	}

	return 0;
}



bool playMove(int board[][w_], int col, int who)
{
	bool open = false;
	for (int i = h_ - 1; i >= 0; i--)
	{
		if (board[i][col] == 0)
		{
			open = true;
			board[i][col] = who;
			break;
		}
	}

	return open;
}



void minimax(int board[][w_], int score[], int who, int currentCheck, int iter)
{
	int newBoard[h_][w_];
	copyBoard(board, newBoard);
	if(iter <= MAX_ITER)
	{
		for (int i = 0; i < w_; i++)
		{
			if (playMove(newBoard, i, currentCheck))
			{
				if (winDetect(newBoard, cDisc))
				{
					if(iter < 2)
					{
						score[i] += 1000;
					}
					else
						score[i] += (MAX_ITER - iter);
				}
				

				else if (winDetect(newBoard, pDisc))
				{	
					if(iter < 2)
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

					minimax(newBoard, score, who, currentCheck, iter+1);
				}
			}
		}
	}
}



bool winDetect(int board[][w_], int who)
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



void printBoard(int board[][w_])
{
	cout << "1\t2\t3\t4\t5\t6\t7\n\n";

	for (int r = 0; r < h_; r++)
	{
		for (int c = 0; c < w_; c++)
			cout << board[r][c] << "\t";

		cout << "\n";
	}
}



void copyBoard(int board[][w_], int newBoard[][w_])
{
	for (int r = 0; r < h_; r++)
	{
		for (int c = 0; c < w_; c++)
		{
			newBoard[r][c] = board[r][c];
		}
	}
}