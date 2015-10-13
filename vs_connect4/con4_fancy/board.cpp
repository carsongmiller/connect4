#include "board.h"
#include "brain.h"

using namespace std;

Board::Board(int w, int h)
{
	w_ = w;
	h_ = h;

	board = new int*[h_];
	for (int i=0; i<h_; i++)
		board[i] = new int[w_];


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

	board = new int*[h_];
	for (int i = 0; i<h_; i++)
		board[i] = new int[w_];

	for (int r = 0; r < h_; r++)
	{
		for (int c = 0; c < w_; c++)
		{
			board[r][c] = disc;
		}
	}
}



Board::Board(Board& old)
{
	w_ = old.getW();
	h_ = old.getH();
	
	board = new int*[h_];
	for (int i = 0; i < h_; i++)
		board[i] = new int[w_];

	for (int r = 0; r < h_; r++)
	{
		for (int c = 0; c < w_; c++)
		{
			board[r][c] = old.getCell(r, c);
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



bool Board::playMove_NB(int col, int who)
{
	bool open = false;
	for (int i = h_ - 1; i >= 0; i--)
	{
		if (newBoard[i][col] == 0)
		{
			open = true;
			newBoard[i][col] = who;
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



void Board::printColor(std::string str, int color)
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



int Board::getCell(int r, int c)
{
	return board[r][c];
}



void Board::setCell(int r, int c, int val)
{
	board[r][c] = val;
}



void Board::createNewBoard()
{
	newBoard = new int*[h_];
	for (int i = 0; i < h_; i++)
		newBoard[i] = new int[w_];
}



int Board::getCell_NB(int r, int c)
{
	return newBoard[r][c];
}



void Board::setCell_NB(int r, int c, int val)
{
	newBoard[r][c] = val;
}



void Board::NB_delete()
{
	for (int i = 0; i < w_; i++)
		delete[] newBoard[i];

	delete[] newBoard;
}