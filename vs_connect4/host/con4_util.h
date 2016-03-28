#include "stdafx.h"

#ifndef CON4_UTIL_H
#define CON4_UTIL_H

#include <iostream>
#include <fstream>
#include <windows.h>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <cmath>
#include <limits>
#include <ctime>

#define w_				7
#define h_				6

#define pDisc			1
#define cDisc			2
#define nDisc			0

//Values for SetConsoleTextAttribute()

#define BLACK			0
#define BLUE			1
#define GREEN			2
#define CYAN			3
#define RED				4
#define MAGENTA			5
#define BROWN			6
#define LIGHTGRAY		7
#define DARKGRAY		8
#define LIGHTBLUE		9
#define LIGHTGREEN		10
#define LIGHTCYAN		11
#define LIGHTRED		12
#define LIGHTMAGENTA	13
#define YELLOW			14
#define WHITE			15

#define PLAYER_1_COLOR	YELLOW
#define PLAYER_2_COLOR	LIGHTRED
#define GAME_COLOR		LIGHTCYAN

using namespace std;



/*
---------------------------
BOARD MANIPULATION
---------------------------
*/

	//returns true if move is valid, returns false if not
	int playMove(int board[][w_], int col, int who);

	//deletes the top disc in the given column
	bool unPlayMove(int board[][w_], int col);

	//initializes the board
	void boardInit(int board[][w_]);

	//sets a cell with no logical checks
	int setCell(int board[][w_], int r, int c, int who);

	//player choosing his move and making move
	void playerTurn(int board[][w_], int &rowPlayed, int &colPlayed);

	//computer choosing and playing move
	void compTurn(int board[][w_], int &rowPlayed, int &colPlayed, int &turn, int MAX_DEPTH, int &cutCount);

/*
---------------------------
PRINT FUNCTIONS
---------------------------
*/

	//prints the board
	void printBoard(int board[][w_]);

	//overloaded for debuging
	void printBoard(int board[][w_], ofstream &output); 
						
	//takes care of printing most of the screen
	void printScreen(int board[][w_]);

	//prints a given string of text in a different color then changes the text color back to white (overloaded for string, int, and char arguments)
	void printColor(string str, int color);
	void printColor(string str, int color, ofstream &output); //overloaded for debuging
	void printColor(int i, int color);
	void printColor(int i, int color, ofstream &output); //overloaded for debuging
	void printColor(char c, int color);
	void printColor(char c, int color, ofstream &output); //overloaded for debuging

	//prints out which player is which color
	void printPlayerColors();

/*
---------------------------
OTHER HELPER FUNCTIONS
---------------------------
*/

	//returns an appropriate symbol for the given index when given the board
	char getChar(int board[][w_], int r, int c);

	//checks if a cell is in bounds of board[][]
	bool isValidCell(int board[][w_], int r, int c);

	//picks who goes first
	void preGame();

	//checks if the disc at board[r][c] has won
	bool winDetect(int board[][w_], int r, int c, int who);
	




/*
=========================================================

BOARD MANIPULATION

=========================================================
*/

int playMove(int board[][w_], int col, int who)
{
	for (int i = h_ - 1; i >= 0; i--)
	{
		if (board[i][col] == nDisc)
		{
			board[i][col] = who;
			return i;
		}
	}

	return -1;
}



bool unPlayMove(int board[][w_], int col)
{
	for (int i = h_ - 1; i > 0; i--)
	{
		if (board[i][col] != nDisc && board[i - 1][col] == nDisc)
		{
			board[i][col] = nDisc;
			return true;
		}
		else if (i == 1 && board[0][col] != nDisc)
		{
			board[0][col] = nDisc;
			return true;
		}
	}

	return false;
}



void playerTurn(int board[][w_], int &rowPlayed, int &colPlayed)
{
	//takes care of most of a human player's turn

	bool isValid;

	isValid = false;
	cout << "Your move! (enter column number): ";
	cin >> colPlayed;

	while (!isValid)
	{
		if (colPlayed > w_ || colPlayed < 1)
		{
			cout << "\nThat is not a valid column\n";

			cout << "Please choose a column between 1 and " << w_ << "\n\n";

			Sleep(1000);

			cout << "Your move! (enter column number): ";
			cin.clear();
			cin.ignore();
			cin >> colPlayed;
		}
		else
			break;
	}

	while (!isValid)
	{
		colPlayed--;

		rowPlayed = playMove(board, colPlayed, pDisc);

		if (rowPlayed == -1)
		{
			cout << "\nThere is no space in that column, choose a different one\n\n";

			Sleep(1000);

			cout << "Your move! (enter column number): ";
			cin.clear();
			cin.ignore();
			cin >> colPlayed;
		}

		else
			break;
	}
}



void boardInit(int board[][w_])
{
	for (int r = 0; r < h_; r++)
	{
		for (int c = 0; c < w_; c++)
		{
			board[r][c] = nDisc;
		}
	}
}



int setCell(int board[][w_], int r, int c, int who)
{
	int prev = board[r][c];
	if (isValidCell(board, r, c))
	{
		board[r][c] = who;
		return prev;
	}
	else
		return 0;
}



/*
=========================================================

PRINT FUNCTIONS

=========================================================
*/

void printScreen(int board[][w_])
{
	system("cls");
	//cout << "CONNECT 4!\n";
	printPlayerColors();
	printBoard(board);
	//cout << "MAX_DEPTH: " << MAX_DEPTH;
	cout << "\n";
}



void printBoard(int board[][w_])
{
	for (int i = 1; i <= w_; i++)
	{
		cout << "    ";
		printColor(i, GAME_COLOR);
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
				cout << getChar(board, r, c);
			else if (board[r][c] == pDisc)
				printColor(getChar(board, r, c), PLAYER_1_COLOR);
			else if (board[r][c] == cDisc)
				printColor(getChar(board, r, c), PLAYER_2_COLOR);

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



void printBoard(int board[][w_], ofstream &output) //overloaded for debuging
{
	for (int i = 1; i <= w_; i++)
	{
		output << "    ";
		printColor(i, GREEN, output);
		output << "\t";
	}

	output << "\n+";

	//First row of ----+----
	for (int i = 0; i < w_; ++i)
		output << "-------+";

	output << "\n";


	//rows of cells with discs in them	
	for (int r = 0; r < h_; r++)
	{
		//First Row of | with no discs
		output << "|   ";

		for (int i = 0; i < w_; i++)
			output << "    |   ";

		//Row with discs

		output << "\n|   ";

		for (int c = 0; c < w_; c++)
		{
			//decide which disc to print
			if (board[r][c] == nDisc)
				output << getChar(board, r, c);
			else if (board[r][c] == pDisc)
				printColor(getChar(board, r, c), LIGHTRED, output);
			else if (board[r][c] == cDisc)
				printColor(getChar(board, r, c), LIGHTBLUE, output);

			output << "   |   ";
		}

		//Second Row of | with no discs
		output << "\n|   ";

		for (int i = 0; i < w_; i++)
			output << "    |   ";

		//Bottom of each cell ----+----
		output << "\n+";

		for (int i = 0; i < w_; ++i)
			output << "-------+";

		output << "\n";
	}
}



void printPlayerColors()
{
	cout << "\n\t ";
	printColor("PLAYER 1", PLAYER_1_COLOR);
	for (int i = 0; i < w_ - 4; i++)
		cout << "\t";
	printColor("PLAYER 2", PLAYER_2_COLOR);
	cout << "\n\n";
}



void printColor(string str, int color, ofstream &output) //overloaded for debuging
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, color);
	output << str;
	SetConsoleTextAttribute(H, WHITE);
}



void printColor(string str, int color)
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, color);
	cout << str;
	SetConsoleTextAttribute(H, WHITE);
}



void printColor(int i, int color, ofstream &output) //overloaded for debuging
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, color);
	output << i;
	SetConsoleTextAttribute(H, WHITE);
}



void printColor(int i, int color)
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, color);
	cout << i;
	SetConsoleTextAttribute(H, WHITE);
}



void printColor(char c, int color, ofstream &output) //overloaded for debuging
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, color);
	output << c;
	SetConsoleTextAttribute(H, WHITE);
}



void printColor(char c, int color)
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, color);
	cout << c;
	SetConsoleTextAttribute(H, WHITE);
}



char getChar(int board[][w_], int r, int c)
{
	if (board[r][c] == nDisc)
		return ' ';

	else if (board[r][c] == pDisc || board[r][c] == cDisc)
		return 'O';

	else
		return 'X'; //something went wrong
}



bool isValidCell(int board[][w_], int r, int c)
{
	if (r >= 0 && r < h_ && c >= 0 && c < w_)
		return true;
	else
		return false;
}



void preGame()
{
	char first;

	printColor("CONNECT 4!", GAME_COLOR);

	cout << "Press enter to begin";
	cin.ignore();
}



bool winDetect(int board[][w_], int r, int c, int who)
{
	int consec;

	//checking for horizontal
	consec = 0;
	for (int i = c - 3; i <= c + 3; i++)
	{
		if (isValidCell(board, r, i) && board[r][i] == who)
		{
			if (++consec >= 4)
				return true;
		}
		else
			consec = 0;
	}

	//checking for vertical
	consec = 0;
	for (int i = r - 3; i <= r + 3; i++)
	{
		if (isValidCell(board, i, c) && board[i][c] == who)
		{
			if (++consec >= 4)
				return true;
		}
		else
			consec = 0;
	}


	//checking for diagonal down
	consec = 0;
	for (int i = -3; i <= 3; i++)
	{
		if (isValidCell(board, r + i, c + i) && board[r + i][c + i] == who)
		{
			if (++consec >= 4)
				return true;
		}
		else
			consec = 0;
	}

	//checking for diagonal up
	consec = 0;
	for (int i = -3; i <= 3; i++)
	{
		if (isValidCell(board, r - i, c + i) && board[r - i][c + i] == who)
		{
			if (++consec >= 4)
				return true;
		}
		else
			consec = 0;
	}

	return false;
}


#endif