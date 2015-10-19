#ifndef _BOARD
#define _BOARD

#include <iostream>
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <cmath>

using namespace std;

class Brain;

class Board
{
private:
	//width and height of the board
	int w_, h_;

	//board
	int** board;

	//integer values of computer, player, and "not" discs
	const int pDisc = 1, cDisc = 2, nDisc = 0;

	//stores the row in which the last disc was played
	int rowPlayed;

	//stores the column in which the last disc was played
	int colPlayed;

	//Values for SetConsoleTextAttribute()
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

public:
	//CONSTRUCTORS
	Board(int h, int w);
	Board();
	Board(Board& old); //copy constructor
	~Board();

	//overloading assignment operator
	void operator=(Board old)
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



	//returns true if move is valid, returns false if not
	int playMove(int col, int who, int &turn);

	//deletes the top disc in the given column
	bool unPlayMove(int col, int &turn);

	//prints the board
	void printBoard();

	//takes care of printing most of the screen
	void printScreen(Brain brain);
	void printScreen();

	//copies the board
	//void copyBoard(int newBoard[][w_]);

	//prints a given string of text in a different color then changes the text color back to white (overloaded for string, int, and char arguments)
	void printColor(string str, int color);
	void printColor(int i, int color);
	void printColor(char c, int color);

	//returns an appropriate symbol for the given index when given the board
	char getChar(int r, int c);

	//initializes the board
	void boardInit();

	//prints out which player is which color
	void printPlayerColors();

	//checks if a cell is in bounds of board[][]
	bool isValidCell(int r, int c);

	//player choosing his move and making move
	void playerMove(int &turn, int &rowPlayed, int &colPlayed);



	//ACCESSOR FUNCTIONS
	int** getBoard();
	int getDisc(char x);
	int getCell(int r, int c);
	int setCell(int r, int c, int who); //sets a cell with no logical checks

};

#endif