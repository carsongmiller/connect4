#include "board.h"

Board::Board(int w, int h)
{
	w_ = w;
	h_ = h;

	for (int r = 0; r < h_; r++)
	{
		for (int c = 0; c < w_; c++)
		{
			board[r][c] = nDisc;
		}
	}
}



Board::Board(int w, int h, int disc)
{
	w_ = w;
	h_ = h;

	for (int r = 0; r < h_; r++)
	{
		for (int c = 0; c < w_; c++)
		{
			board[r][c] = disc;
		}
	}
}



bool Board::playMove(int col, int who)
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



//Following code is pretty messy, but it works and makes a nice print out

void Board::printBoard()
{
	for (int i = 1; i <= w_; i++)
	{
		cout << "    ";
		printColor(i, GREEN);
		cout << "\t";
	}

	cout << "\n+";

	//First row of ----+----
	for (int i = 0; i < w_; ++i)
		cout << "-------+";

	cout << "\n";


	//rows of cells with discs in them	
	for (int r = 0; r < h_; r++)
	{
		//First Row of | with no discs
		cout << "|   ";

		for (int i = 0; i < w_; i++)
			cout << "    |   ";

		//Row with discs

		cout << "\n|   ";

		for (int c = 0; c < w_; c++)
		{
			//decide which disc to print
			if (board[r][c] == 0)
				cout << getChar(r, c);
			else if (board[r][c] == 1)
				printColor(getChar(r, c), LIGHTRED);
			else if (board[r][c] == 2)
				printColor(getChar(r, c), LIGHTBLUE);

			cout << "   |   ";
		}

		//Second Row of | with no discs
		cout << "\n|   ";

		for (int i = 0; i < w_; i++)
			cout << "    |   ";

		//Bottom of each cell ----+----
		cout << "\n+";

		for (int i = 0; i < w_; ++i)
			cout << "-------+";

		cout << "\n";
	}
}



char Board::getChar(int r, int c)
{
	if (board[r][c] == 0)
		return ' ';
	else if (board[r][c] == 1 || board[r][c] == 2)
		return 'O';
	else
		return 'X'; //if something goes wrong
}



void Board::printColor(string str, int color)
{
	SetConsoleTextAttribute(H, color);
	cout << str;
	SetConsoleTextAttribute(H, WHITE);
}

void Board::printColor(int i, int color)
{
	SetConsoleTextAttribute(H, color);
	cout << i;
	SetConsoleTextAttribute(H, WHITE);
}

void Board::printColor(char c, int color)
{
	SetConsoleTextAttribute(H, color);
	cout << c;
	SetConsoleTextAttribute(H, WHITE);
}

int** Board::getBoard()
{
	return board;
}

int Board::getW()
{
	return w_;
}

void Board::setW(int w)
{
	w_ = w;
}

int Board::getH()
{
	return h_;
}

void Board::setH(int h)
{
	h_ = h;
}

int Board::getDisc(char c)
{
	if(c == 'n')
		return nDisc;
	else if (c == 'p')
		return pDisc;
	else if (c == 'c')
		return cDisc;
}