#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <iostream>
#include <windows.h>
#include <cstdlib>

class Board()
{
private:

	//the main board
	int board[h_][w_];
	
	//pDisc =  disc; cDisc = computer disc; nDisc = no disc
	const int nDisc = 0, pDisc = 1, cDisc = 2;
	//width and height variables
	const int w_, h_;

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
	//Board Constructors
	Board(int w, int h);
	Board(int w, int h, int disc);

	//returns true if move is valid, returns false if not
	bool playMove(int col, int who);

	//prints the board
	void printBoard();

	//prints a given string of text in a different color then changes the text color back to white (overloaded for string, int, and char arguments)
	void printColor(string str, int color);
	void printColor(int i, int color);
	void printColor(char c, int color);

	//returns an appropriate symbol for the given index when given the board
	char getChar(int r, int c);

	//accessor functions:
	int** getBoard();

	int getW();
	void setW(int h);

	int getH();
	void setH(int h);

	int getDisc(char c);

};

#endif