#ifndef _BOARD
#define _BOARD

#include <string>
#include <iostream>
#include <windows.h>
#include <cstdlib>

class Brain;

class Board
{
private:

	//the main board
	int** board;

	//pointer to be used in minimax
	int** newBoard;
	
	//pDisc =  disc; cDisc = computer disc; nDisc = no disc
	const int nDisc = 0, pDisc = 1, cDisc = 2;
	//width and height variables
	int w_, h_;

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

	//copy constructor
	Board(Board& old);

	//returns true if move is valid, returns false if not (uses **board)
	bool playMove(int col, int who);

	//returns true if move is valid, returns false if not (uses **newBoard)
	bool playMove_NB(int col, int who);

	//prints the board
	void printBoard();

	//prints a given string of text in a different color then changes the text color back to white (overloaded for string, int, and char arguments)
	void printColor(std::string str, int color);
	void printColor(int i, int color);
	void printColor(char c, int color);

	//returns an appropriate symbol for the given index
	char getChar(int r, int c);

	//accessor functions:
	int** getBoard();
	int** getNewBoard();

	int getW(); //returns width of board
	void setW(int h); //sets width of board

	int getH(); //returns height of board
	void setH(int h); //sets height of board

	int getDisc(char c); //returns either 0, 1, or 2 given n, p, or c

	int getCell(int r, int c); //returns the value of a cell
	void setCell(int r, int c, int val); //sets the value of a cell

	void createNewBoard(); //copies board[][] into newBoard[][]

	int getCell_NB(int r, int c); //gets a cell from newBoard[][]
	void setCell_NB(int r, int c, int val); //sets a cell in newBoard[][]

	void NB_delete(); //makes newBoard[][] = NULL

};
#endif