#include <iostream>
#include <windows.h>

using namespace std;

const int w_ = 7;
const int h_ = 6;
const int nDisc = 0;

void printBoard(int board[][w_]);
void printColor(string str, int color);
void printColor(int i, int color);
void printColor(char c, int color);

HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);

	const int BLACK = 0;
	const int BLUE = 1;
	const int GREEN = 2;
	const int CYAN = 3;
	const int RED = 4;
	const int MAGENTA = 5;
	const int BROWN = 6;
	const int LIGHTGRAY = 7;
	const int DARKGRAY = 8;
	const int LIGHTBLUE = 9;
	const int LIGHTGREEN = 10;
	const int LIGHTCYAN = 11;
	const int LIGHTRED = 12;
	const int LIGHTMAGENTA = 13;
	const int YELLOW = 14;
	const int WHITE = 15;



int main()
{
	int board[h_][w_];

	for (int r = 0; r < h_; r++)
	{
		for (int c = 0; c < w_; c++)
		{
			board[r][c] = nDisc;
		}
	}

	printBoard(board);
	return 0;
}



void printColor(string str, int color)
{
	SetConsoleTextAttribute(H, color);
	cout << str;
	SetConsoleTextAttribute(H, WHITE);
}

void printColor(int i, int color)
{
	SetConsoleTextAttribute(H, color);
	cout << i;
	SetConsoleTextAttribute(H, WHITE);
}

void printColor(char c, int color)
{
	SetConsoleTextAttribute(H, color);
	cout << c;
	SetConsoleTextAttribute(H, WHITE);
}

void printBoard(int board[][w_])
{
	for(int i = 1; i <= w_; i++)
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

				for (int c = 0; c < w_; c++)
					cout << "    |   ";

			//Row with discs

				cout << "\n|   ";

				for (int c = 0; c < w_; c++)
					cout << board[r][c] << "   |   ";

			//Second Row of | with no discs
				cout << "\n|   ";

				for (int c = 0; c < w_; c++)
					cout << "    |   ";

			//Bottom of each cell ----+----
				cout << "\n+";

				for (int i = 0; i < w_; ++i)
					cout << "-------+";

			cout << "\n";
		}
}

