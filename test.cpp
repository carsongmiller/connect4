#include <iostream>

using namespace std;

const int w_ = 7;
const int h_ = 6;

void copyBoard(int board[][w_], int newBoard[][w_])
{
	for(int r = 0; r < h_; r++)
	{
		for(int c = 0; c < w_; c++)
		{
			newBoard[r][c] = board[r][c];
		}
	}
}

int main()
{
	int board[h_][w_], newBoard[h_][w_];
	for(int r = 0; r < h_; r++)
	{
		for(int c = 0; c < w_; c++)
		{
			board[r][c] = r;
		}
	}
	copyBoard(board, newBoard);

	cout << newBoard[0][0] << "\t" << newBoard[1][1] << "\t" << newBoard[2][2] << endl;

	return 0;
}

