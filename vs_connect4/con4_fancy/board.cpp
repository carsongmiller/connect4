#include "board.h"
#include "brain.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <cmath>

using namespace std;

Board::Board(int h, int w) //overloaded constructor
{
	w_ = w;
	h_ = h;
	boardInit();
}



Board::Board() //default constructor
{
	w_ = 7;
	h_ = 6;
	boardInit();
}



Board::Board(Board& old) //copy constructor
{
	this->w_ = old.w_;
	this->h_ = old.h_;

	this->board = new int*[h_];
	for (int i = 0; i < h_; i++)
		this->board[i] = new int[w_];

	for (int r = 0; r < h_; r++)
		for (int c = 0; c < w_; c++)
			this->board[r][c] = old.board[r][c];
}



Board::~Board() //destructor
{
	for (int i = 0; i < h_; i++)
		delete[] board[i];

	delete[] board;
}



int Board::playMove(int col, int who, int &turn)
{
	for (int i = h_ - 1; i >= 0; i--)
	{
		if (board[i][col] == nDisc)
		{
			board[i][col] = who;
			turn++;
			return i;
		}
	}

	return -1;
}



bool Board::unPlayMove(int col, int &turn)
{
	for (int i = h_ - 1; i > 0; i--)
	{
		if (board[i][col] != nDisc && board[i - 1][col] == nDisc)
		{
			board[i][col] = nDisc;
			turn--;
			return true;
		}
		else if (i == 1 && board[0][col] != nDisc)
		{
			board[0][col] = nDisc;
			turn--;
			return true;
		}
	}

	return false;
}



char Board::getChar(int r, int c) //returns the character representing the disc at the given index of board[][]
{
	if (board[r][c] == nDisc)
		return ' ';

	else if (board[r][c] == pDisc || board[r][c] == cDisc)
		return 'O';

	else
		return 'X'; //something went wrong
}



/*void Board::copyBoard(int newBoard[][w_])
{
for (int r = 0; r < h_; r++)
{
for (int c = 0; c < w_; c++)
{
newBoard[r][c] = board[r][c];
}
}
}*/



void Board::playerMove(int &turn, int &rowPlayed, int &colPlayed) //takes care of most of a player's turn
{
	bool isValid;

	isValid = false;
	cout << "Where would you like to go? (enter column number): ";
	cin >> colPlayed;

	while (!isValid)
	{
		if (colPlayed > w_ || colPlayed < 1)
		{
			cout << "There is no column " << colPlayed << ". Please choose a column between 1 and " << w_ << "\n\n";

			cout << "Where would you like to go? (enter column number): ";
			cin >> colPlayed;
		}
		else
			break;
	}

	while (!isValid)
	{
		rowPlayed = playMove(colPlayed - 1, pDisc, turn);

		if (rowPlayed == -1)
		{
			cout << "There is no space in that column, choose a different one\n\n";

			cout << "Where would you like to go? (enter column number): ";
			cin >> colPlayed;
		}

		else
			break;
	}
}



/*

		PRINT FUNCTIONS

*/



void Board::printScreen(Brain brain) //if you want to print the scores for each column underneath the board
{
	system("cls");
	cout << "CONNECT 4!\n";
	printPlayerColors();
	printBoard();
	brain.printScore();
	cout << "\n";
}



void Board::printScreen() //if you don't want to see scores for columns
{
	system("cls");
	cout << "CONNECT 4!\n";
	printPlayerColors();
	printBoard();
	cout << "\n";
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
			if (board[r][c] == nDisc)
				cout << getChar(r, c);
			else if (board[r][c] == pDisc)
				printColor(getChar(r, c), LIGHTRED);
			else if (board[r][c] == cDisc)
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



void Board::printPlayerColors()
{
	cout << "Player: ";
	printColor('O', LIGHTRED);
	cout << endl;
	cout << "Computer: ";
	printColor('O', LIGHTBLUE);
	cout << endl;
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



void Board::boardInit()
{
	board = new int*[h_];
	for (int i = 0; i < h_; i++)
		board[i] = new int[w_];

	for (int r = 0; r < h_; r++)
	{
		for (int c = 0; c < w_; c++)
		{
			board[r][c] = nDisc;
		}
	}
}



bool Board::isValidCell(int r, int c)
{
	if (r >= 0 && r < h_ && c >= 0 && c < w_)
		return true;
	else
		return false;
}



int Board::setCell(int r, int c, int who)
{
	int prev = board[r][c];
	if (isValidCell(r, c))
	{
		board[r][c] = who;
		return prev;
	}
	else
		return 0;
}



int** Board::getBoard()
{
	return board;
}



int Board::getDisc(char x)
{
	if (x == 'c') return cDisc;
	else if (x == 'p') return pDisc;
	else if (x == 'n') return nDisc;
	else return -1;
}



int Board::getCell(int r, int c)
{
	if (isValidCell(r, c)) return board[r][c];
}